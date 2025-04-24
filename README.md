# ForceCloseOreUI

强制关闭 OreUI

## 使用方法

1. 从 Minecraft APK 中提取 **libminecraftpe.so** 文件
2. 从[发布页面](https://github.com/Yejdhi/ForceCloseOreUI-Android/releases/tag/Release)下载或手动编译 **.so** 文件
3. 查看 [视频教程](暂且无链接) 了解操作流程
4. 针对 64 位 (arm64-v8a) 版本 Minecraft 运行以下命令：

```bash
patchelf --add-needed libForceCloseOreUI-arm64.so libminecraftpe.so
```

5. 将修改后的 **libminecraftpe.so** 和 **libForceCloseOreUI.so** 文件放回 APK 中
6. 签名并安装 APK
