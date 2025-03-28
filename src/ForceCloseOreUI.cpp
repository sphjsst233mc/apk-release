
#include "ForceCloseOreUI.h"

#include <string>
#include <unordered_map>

#include <memory/Hook.h>

#if __arm__
#include <unistd.h>
extern "C" int __wrap_getpagesize() {
    return sysconf(_SC_PAGESIZE);
}
#endif

class OreUIConfig {
public:
    void* mUnknown1;
    void* mUnknown2;
    std::function<bool()> mUnknown3;
    std::function<bool()> mUnknown4;
};

class OreUi {
public:
    std::unordered_map<std::string, OreUIConfig> mConfigs;
};



SKY_AUTO_STATIC_HOOK(
    Hook1,
    memory::HookPriority::Normal,
#if __arm__
    { "" },
#elif __aarch64__
    { "? ? ? D1 ? ? ? A9 ? ? ? 91 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? D5 F5 03 03 2A ? ? ? F9 F6 03 02 2A F3 03 01 AA F4 03 00 AA ? ? ? F8 ? ? ? F9 ? ? ? B4 9F 00 08 EB" },
#endif
    void,
    OreUi& a1,
    int64_t a2,
    bool a3,
    bool a4,
    void* a5,
    void* a6
) { 
    origin(a1, a2, a3, a4, a5, a6);
    for (auto& data : a1.mConfigs) {
        data.second.mUnknown3 = []() { return false; };
        data.second.mUnknown4 = []() { return false; };
    }
} 