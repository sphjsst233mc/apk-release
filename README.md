# ForceCloseOreUI

强制关闭 OreUI\
**太好啦！！！jsonUI有救啦！！！**

English\
[EN_README](https://github.com/sphjsst233mc/apk-release/blob/master/EN_README.md)

## 使用方法

1. 可直接在 [下载](https://github.com/sphjsst233mc/apk-release/releases) 获取打包好的文件
2. 查看 [视频教程](https://b23.tv/7acXsHh) 了解操作流程
3. 从[发布页面](https://github.com/Yejdhi/ForceCloseOreUI-Android/releases)下载或手动编译 **.so** 文件
4. 从 Minecraft APK 中提取 **libminecraftpe.so** 文件
5. **仅适用于 (arm64-v8a) 版本**，运行以下命令进行修改：

```bash
patchelf --add-needed libForceCloseOreUI-arm64.so libminecraftpe.so
```

**注意：当前仅支持 64 位架构，无 (arm32-v7a) 版本支持**

6. 将修改后的 **libminecraftpe.so** 和 **libForceCloseOreUI.so** 文件放回 APK 中
7. 签名并安装 APK

---

# Minecraft 去oreUI改包教程

（推荐配合视频操作更直观）

> **准备工具：**
> 1. MT管理器（下载地址：[链接](https://mt2.cn/download)）
> 2. Termux（下载地址：[链接](https://f-droid.org/packages/com.termux/)
> 备用下载地址：[链接](https://bbk.endyun.ltd/alist/！打包教程(点击此处查看)/版本/com.termux_1001.apk)）
> 3. libForceCloseOreUI.so（下载地址：[链接](https://bbk.endyun.ltd/alist/！打包教程(点击此处查看)/版本/1.0.0/libForceCloseOreUI.so)
> 备用下载地址：[链接](https://gitcode.com/yinghuaji/apk-release/releases/download/版本/libForceCloseOreUI.so))
> 4. 已下载的 libForceCloseOreUI.so 文件和 Minecraft 安装包

## 文件操作步骤
1. 在管理器主页上方可直接查看路径，默认是：`/storage/emulated/0/`
2. 打开MT管理器 → 进入手机存储根目录：`/storage/emulated/0/`
3. 找到 `Download` 文件夹 → 定位下载好的 Minecraft 安装包
4. 点击安装包 → 再点击查看 → 进入 `lib/arm64-v8a/` → 找到 `libminecraftpe.so`
5. 在另一个窗口中 → 新建英文文件夹（如：`minecraft`）
6. 长按 `libminecraftpe.so` → 解压到新建的文件夹
7. 退出安装包窗口，将 `libForceCloseOreUI.so` 也放入该文件夹

## Termux 操作
1. **首次运行需要更新**  
   ```bash
   pkg update -y && pkg upgrade -y && pkg install -y patchelf
   ```
2. 遇到确认提示时输入 `y` 回车（出现 `[Y/I/N/O/D/Z]` 时输入 `y`）
3. **进入目标目录（注意大小写）**  
   ```bash
   cd /storage/emulated/0/minecraft
   ```
4. **若提示权限不足**  
   ```bash
   termux-setup-storage
   ```  
   随后输入 `y` 并同意系统弹窗授权
5. 然后重新执行第三步
6. **执行关键命令（无提示即成功）**  
   ```bash
   patchelf --add-needed libForceCloseOreUI.so libminecraftpe.so
   ```

> **注意：**
> - 命令执行后需等待进度完成
> - 绿色路径提示表示目录正确

## 重新打包安装
1. 在MT管理器中将修改后的两个.so文件，两个全往右滑：
   - 在另一个窗口中再次打开，拖进 Minecraft 安装包的 `lib/arm64-v8a/`
   - 添加并确定
2. 退出安装包界面 → 点击安装
3. 若出现签名冲突：
   - 备份存档/皮肤/模组到其他文件夹
   - 卸载旧版 → 安装修改版

## 常见问题
- **路径错误** → 检查文件夹名称是否一致
- **命令无反应** → 等待1-2分钟观察提示
- **文件丢失** → 重新操作解压步骤
- **无法访问** → Termux是否获得存储权限
- **命令问题** → 命令是否完整复制
