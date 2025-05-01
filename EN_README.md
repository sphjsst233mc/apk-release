# ForceCloseOreUI

Force Close OreUI

**Awesome!!! JSON UI is saved!!!**

## Usage Guide

1. Download pre-compiled APK directly from [APK Releases](https://github.com/sphjsst233mc/apk-release/releases)
2. Watch [Video Tutorial](https://b23.tv/7acXsHh) or [Video Tutorial 2](暂缺链接) for visual guidance
3. Download or manually compile **.so** files from [Release Page](https://github.com/Yejdhi/ForceCloseOreUI-Android/releases/tag/Release)
4. Extract **libminecraftpe.so** from the Minecraft APK
5. **Exclusive to arm64-v8a version** - Execute the following command:

```bash
patchelf --add-needed libForceCloseOreUI-arm64.so libminecraftpe.so
```

**Warning: Only supports 64-bit architecture. NO arm32-v7a support**
 
 
 6. Reinsert modified **libminecraftpe.so** and **libForceCloseOreUI.so** into APK
 7. Sign and install the modified APK
