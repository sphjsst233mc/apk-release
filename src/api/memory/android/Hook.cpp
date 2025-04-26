

#include "api/memory/Hook.h"
#include "api/memory/android/Memory.h"

#include <cstdint>
#include <string>
#include <vector>

#include "dobby.h"

// Define RT_SUCCESS if not already defined
#ifndef RT_SUCCESS
#define RT_SUCCESS 0
#endif

#include <android/log.h>

#define LOGI(...)                                                              \
  __android_log_print(ANDROID_LOG_INFO, "LeviLogger", __VA_ARGS__)

#include <mutex>
#include <set>
#include <type_traits>
#include <unordered_map>

namespace memory {

struct HookElement {
  FuncPtr detour{};
  FuncPtr *originalFunc{};
  HookPriority priority{};
  int id{};

  bool operator<(const HookElement &other) const {
    if (priority != other.priority)
      return priority < other.priority;
    return id < other.id;
  }
};

struct HookData {
  FuncPtr target{};
  FuncPtr origin{};
  FuncPtr start{};
  int hookId{};
  std::set<HookElement> hooks{};

  void updateCallList() {
    FuncPtr *last = nullptr;
    for (auto &item : this->hooks) {
      if (last == nullptr) {
        this->start = item.detour;
        last = item.originalFunc;
        *last = this->origin;
      } else {
        *last = item.detour;
        last = item.originalFunc;
      }
    }

    if (last == nullptr) {
      this->start = this->origin;
    } else {
      *last = this->origin;
    }
  }

  int incrementHookId() { return ++hookId; }
};

std::unordered_map<FuncPtr, std::shared_ptr<HookData>> &getHooks() {
  static std::unordered_map<FuncPtr, std::shared_ptr<HookData>> hooks;
  return hooks;
}

static std::mutex hooksMutex{};

int hook(FuncPtr target, FuncPtr detour, FuncPtr *originalFunc,
         HookPriority priority, bool suspendThreads) {
  std::lock_guard lock(hooksMutex);

  auto it = getHooks().find(target);
  if (it != getHooks().end()) {
    auto hookData = it->second;
    hookData->hooks.insert(
        {detour, originalFunc, priority, hookData->incrementHookId()});
    hookData->updateCallList();

    DobbyHook(target, hookData->start, &hookData->origin);
    return 0;
  }

  auto hookData = std::make_shared<HookData>();
  hookData->target = target;
  hookData->origin = nullptr;
  hookData->start = detour;
  hookData->hooks.insert(
      {detour, originalFunc, priority, hookData->incrementHookId()});

  if (DobbyHook(target, detour, &hookData->origin) != RT_SUCCESS) {
    return -1;
  }

  hookData->updateCallList();
  getHooks().emplace(target, hookData);
  return 0;
}

bool unhook(FuncPtr target, FuncPtr detour, bool suspendThreads) {
  std::lock_guard lock(hooksMutex);

  if (!target)
    return false;

  auto hookDataIter = getHooks().find(target);
  if (hookDataIter == getHooks().end())
    return false;

  auto &hookData = hookDataIter->second;

  for (auto it = hookData->hooks.begin(); it != hookData->hooks.end(); ++it) {
    if (it->detour == detour) {
      hookData->hooks.erase(it);
      hookData->updateCallList();

      if (hookData->hooks.empty()) {
        DobbyDestroy(target);
        getHooks().erase(target);
      } else {
        DobbyHook(target, hookData->start, &hookData->origin);
      }

      return true;
    }
  }

  return false;
}

void unhookAll() {
  std::lock_guard lock(hooksMutex);

  for (auto &[target, hookData] : getHooks()) {
    DobbyDestroy(target);
  }

  getHooks().clear();
}

uintptr_t getLibBase(const char *libName) {
  FILE *fp = fopen("/proc/self/maps", "r");
  if (!fp)
    return 0;

  uintptr_t base = 0;
  char line[512];

  while (fgets(line, sizeof(line), fp)) {
    if (strstr(line, libName)) {
      uintptr_t temp;
      if (sscanf(line, "%lx-%*lx", &temp) == 1) {
        base = temp;
        break;
      }
    }
  }

  fclose(fp);
  return base;
}

size_t getLibSize(const char *libName) {
  FILE *fp = fopen("/proc/self/maps", "r");
  if (!fp)
    return 0;

  size_t totalSize = 0;
  char line[512];

  while (fgets(line, sizeof(line), fp)) {
    if (strstr(line, libName)) {
      uintptr_t start = 0, end = 0;
      if (sscanf(line, "%lx-%lx", &start, &end) == 2) {
        totalSize += (end - start);
      }
    }
  }

  fclose(fp);
  return totalSize;
}

FuncPtr resolveIdentifier(const char *identifier) {
  static bool initialized = false;
  static uintptr_t base = 0;
  static size_t size = 0;

  if (!initialized) {
    base = getLibBase("libminecraftpe.so");
    size = getLibSize("libminecraftpe.so");

    LOGI("libminecraftpe base = 0x%lx, size = 0x%zx", base, size);

    initialized = true;
  }

  if (base == 0 || size == 0) {
    LOGI("Failed to find libminecraftpe.so");
    return nullptr;
  }

  uintptr_t result = resolveSignature(base, size, identifier);
  if (result) {
    LOGI("[resolveIdentifier] Resolved identifier [%s] to address 0x%lx",
         identifier, result);
    return reinterpret_cast<FuncPtr>(result);
  } else {
    LOGI("[resolveIdentifier] Failed to resolve signature for [%s]",
         identifier);
    return nullptr;
  }
}

FuncPtr resolveIdentifier(std::initializer_list<const char *> identifiers) {
  for (const auto &identifier : identifiers) {
    FuncPtr result = resolveIdentifier(identifier);
    if (result != nullptr) {
      return result;
    }
  }
  return nullptr;
}

} // namespace memory