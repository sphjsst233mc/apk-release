#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <unordered_map>

#include <api/memory/Hook.h>

#include <nlohmann/json.hpp>

#if __arm__
#include <unistd.h>
extern "C" int __wrap_getpagesize() { return sysconf(_SC_PAGESIZE); }
#endif

class OreUIConfig {
public:
  void *mUnknown1;
  void *mUnknown2;
  std::function<bool()> mUnknown3;
  std::function<bool()> mUnknown4;
};

class OreUi {
public:
  std::unordered_map<std::string, OreUIConfig> mConfigs;
};

// clang-format off
#if __arm__
#define PATTERN 
   {""}

#elif __aarch64__
#define PATTERN                                                                           \
  {                                                                                       \
    "? ? ? D1 ? ? ? A9 ? ? ? 91 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? A9 E8 03 03 AA" \
  }

#elif _WIN32
#define PATTERN                                                                                                          \
  {                                                                                                                      \
    "40 53 55 56 57 41 54 41 55 41 56 41 57 48 83 EC 78 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 49 8B E9 4C 89 44 24"    \
  }                                                                                                                      \

#endif

namespace{
  std::string dirPath = "mods/ForceCloseOreUI/";
  std::string filePath = dirPath + "config.json";
  nlohmann::json outputJson;

// clang-format on
SKY_AUTO_STATIC_HOOK(Hook2, memory::HookPriority::Normal, PATTERN, void,
                     void *a1, void *a2, void *a3, void *a4, void *a5, void *a6,
                     void *a7, void *a8, void *a9, void *a10, OreUi &a11,
                     void *a12) {

  if (!std::filesystem::exists(filePath)) {
    for (auto &data : a11.mConfigs) {
      outputJson[data.first] = false;
    }
    std::filesystem::create_directories(dirPath);
    std::ofstream outFile(filePath);
    outFile << outputJson.dump(4);
    outFile.close();
  } else {
    std::ifstream inFile(filePath);
    inFile >> outputJson;
    inFile.close();
  }

  for (auto &data : a11.mConfigs) {
    bool value = false;
    if (outputJson.contains(data.first) &&
        outputJson[data.first].is_boolean()) {
      value = outputJson[data.first];
    }
    data.second.mUnknown3 = [value]() { return value; };
    data.second.mUnknown4 = [value]() { return value; };
  }
  origin(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
}
} // namespace