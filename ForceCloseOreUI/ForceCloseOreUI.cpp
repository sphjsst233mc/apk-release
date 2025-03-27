#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdlib>

#include <string>

#include "frida-gum.h"
#include "ForceCloseOreUI.h"

#include <cmath>
#include <unordered_map>
#include <android/log.h>

extern "C" double frexp(double x, int* exp) {
    return std::frexp(x, exp);
}

#if __aarch64__
extern "C" long double frexpl(long double x, int* exp) {
    return std::frexp(x, exp);
}
#endif

//==========================================================================================================================================

typedef struct _LoaderInvocationListener LoaderInvocationListener;
struct _LoaderInvocationListener {
    GObject parent;
};

static void loader_invocation_listener_iface_init(gpointer g_iface, gpointer iface_data);
static void loader_invocation_listener_on_enter(GumInvocationListener* listener, GumInvocationContext* ic);
static void loader_invocation_listener_on_leave(GumInvocationListener* listener, GumInvocationContext* ic);

#define LOADER_TYPE_INVOCATION_LISTENER (loader_invocation_listener_get_type())
G_DECLARE_FINAL_TYPE(LoaderInvocationListener, loader_invocation_listener, LOADER, INVOCATION_LISTENER, GObject)
G_DEFINE_TYPE_EXTENDED(LoaderInvocationListener,
    loader_invocation_listener,
    G_TYPE_OBJECT,
    0,
    G_IMPLEMENT_INTERFACE(GUM_TYPE_INVOCATION_LISTENER, loader_invocation_listener_iface_init))

//==========================================================================================================================================

enum class HookId : int {
    OREUI_REG
};

class OreUIConfig {
public:
    gpointer mUnknown1;
    gpointer mUnknown2;
    std::function<bool()> mIsSelected;
    std::function<bool()> mIsSupported;
};

class OreUi {
public:
    std::unordered_map<std::string, OreUIConfig> mConfigs;
};

GumAddress minecraftpeBaseAddr;

GumAddress oreui_reg;
GumAddress ClientInstance_Init;


GumInterceptor* interceptor;
GumInvocationListener* listener;

OreUi* GlobalPtr = nullptr;

void __attribute__((constructor)) init() {
    gum_init(); 
    minecraftpeBaseAddr = gum_module_find_base_address("libminecraftpe.so");
    GumModuleMap* moduleMap = gum_module_map_new();
    const GumModuleDetails* minecraftpeDetails = gum_module_map_find(moduleMap, minecraftpeBaseAddr);

#if __arm__
    ClientInstance_Init = FindSignatures(minecraftpeDetails, "");
    if (ClientInstance_Init) {
        ClientInstance_Init += 1;
    }
#elif __aarch64__
    oreui_reg = FindSignatures(minecraftpeDetails, "FF 03 02 D1 FD 7B 04 A9 FD 03 01 91 F8 5F 05 A9 F6 57 06 A9 F4 4F 07 A9 57 D0 3B D5 F5 03 03 2A");
#endif

    g_object_unref(moduleMap);

    interceptor = gum_interceptor_obtain();
    listener = (GumInvocationListener*)g_object_new(LOADER_TYPE_INVOCATION_LISTENER, NULL);

    if (!oreui_reg ) {
        return;
    }
    gum_interceptor_begin_transaction(interceptor);

    gum_interceptor_attach(interceptor,
        GSIZE_TO_POINTER(oreui_reg),
        listener,
        GSIZE_TO_POINTER(HookId::OREUI_REG));

    gum_interceptor_end_transaction(interceptor);
}

void __attribute__((destructor)) dispose() {

    gum_interceptor_detach(interceptor, listener);

    g_object_unref(listener);
    g_object_unref(interceptor);

    gum_deinit();
}

static void loader_invocation_listener_class_init(LoaderInvocationListenerClass* klass) {
    (void)LOADER_IS_INVOCATION_LISTENER;
}

static void loader_invocation_listener_iface_init(gpointer g_iface, gpointer iface_data) {
    GumInvocationListenerInterface* iface = (GumInvocationListenerInterface*)g_iface;

    iface->on_enter = loader_invocation_listener_on_enter;
    iface->on_leave = loader_invocation_listener_on_leave;
}

static void loader_invocation_listener_init(LoaderInvocationListener* self) {
}


#define LOG_TAG "ForceCloseOreUI"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


static void loader_invocation_listener_on_enter(GumInvocationListener* listener, GumInvocationContext* ic) {
    HookId hookId = (HookId)GUM_IC_GET_FUNC_DATA(ic, uintptr_t);
    switch (hookId) {
    case HookId::OREUI_REG: {
        if (!GlobalPtr) {
            GlobalPtr = (OreUi*)gum_invocation_context_get_nth_argument(ic, 0);      
        }
    }
    }
}

static void loader_invocation_listener_on_leave(GumInvocationListener* listener, GumInvocationContext* ic) {
    HookId hookId = (HookId)GUM_IC_GET_FUNC_DATA(ic, uintptr_t);
    switch (hookId) {
    case HookId::OREUI_REG: {
        if (GlobalPtr) {
            for (auto& data : GlobalPtr->mConfigs) {
                data.second.mIsSelected = []() { return false; };
                data.second.mIsSupported = []() { return false; };
            }
        }
        break;
    }
    }
}