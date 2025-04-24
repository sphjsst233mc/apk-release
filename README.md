# ForceCloseOreUI

强制关闭 OreUI\
**太好啦！！！jsonUI有救啦！！！**

## 使用方法

1. 可直接在 [下载](https://github.com/sphjsst233mc/apk-release/releases) 获取打包好的文件
2. 查看 [视频教程](暂且无链接) 了解操作流程
3. 从[发布页面](https://github.com/Yejdhi/ForceCloseOreUI-Android/releases/tag/Release)下载或手动编译 **.so** 文件
4. 从 Minecraft APK 中提取 **libminecraftpe.so** 文件
5. **仅适用于 (arm64-v8a) 版本**，运行以下命令进行修改：

```bash
patchelf --add-needed libForceCloseOreUI-arm64.so libminecraftpe.so
```

**注意：当前仅支持 64 位架构，无 (arm32-v7a) 版本支持**

6. 将修改后的 **libminecraftpe.so** 和 **libForceCloseOreUI.so** 文件放回 APK 中
7. 签名并安装 APK
