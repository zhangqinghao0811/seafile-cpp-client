# Seafile C++ Client

[![Build Status](https://github.com/zhangqinghao0811/seafile-cpp-client/workflows/Build%20and%20Test/badge.svg)](https://github.com/zhangqinghao0811/seafile-cpp-client/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-blue.svg)](https://github.com/zhangqinghao0811/seafile-cpp-client)

基于 wxWidgets 的跨平台 Seafile 会员制文件下载客户端。

## ✨ 功能特性

🔐 **会员制访问控制**
- 用户登录认证系统
- 严格的会员权限验证
- 非会员无法访问文件功能

📁 **文件管理**
- 会员专用文件列表浏览
- 文件信息展示（名称、大小、修改时间、类型）
- 双击文件开始下载

🖥️ **跨平台支持**
- Windows 10/11
- Linux (Ubuntu, CentOS, Fedora)
- macOS 10.15+

⚡ **现代化架构**
- C++17 标准
- 模块化设计
- 线程安全的日志系统
- 灵活的配置管理

## 🏗️ 技术架构

### 核心组件

- **主应用框架**: `App.h/cpp`, `MainFrame.h/cpp`
- **用户认证**: `AuthManager`, `LoginDialog`
- **配置管理**: `ConfigManager` (支持 INI 格式)
- **日志系统**: `Logger` (多级别，线程安全)
- **界面组件**: 文件列表、下载管理、会员中心面板

### 依赖库

- **wxWidgets 3.1.0+** - GUI 框架
- **libcurl 7.60.0+** - HTTP 网络通信
- **CMake 3.16+** - 构建系统

## 🚀 快速开始

### 系统要求

- CMake 3.16 或更高版本
- C++17 兼容的编译器
- wxWidgets 3.1.0 或更高版本
- libcurl 7.60.0 或更高版本

### 安装依赖

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake
sudo apt install libwxgtk3.0-gtk3-dev libcurl4-openssl-dev
```

#### CentOS/RHEL/Fedora
```bash
# CentOS/RHEL
sudo yum groupinstall "Development Tools"
sudo yum install cmake3 wxGTK3-devel libcurl-devel

# Fedora
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake wxGTK3-devel libcurl-devel
```

#### macOS
```bash
# 使用 Homebrew
brew install cmake wxwidgets curl
```

#### Windows
```batch
# 使用 vcpkg
vcpkg install wxwidgets:x64-windows curl:x64-windows
vcpkg integrate install
```

### 编译项目

#### Linux/macOS
```bash
# 克隆项目
git clone https://github.com/zhangqinghao0811/seafile-cpp-client.git
cd seafile-cpp-client

# 使用构建脚本
./scripts/build.sh

# 或手动构建
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

#### Windows
```batch
# 克隆项目
git clone https://github.com/zhangqinghao0811/seafile-cpp-client.git
cd seafile-cpp-client

# 使用构建脚本
scripts\build.bat --vcpkg C:\path\to\vcpkg

# 或手动构建
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build . --config Release
```

### 运行程序

```bash
# Linux/macOS
cd build && ./SeafileClient

# Windows
cd build\Release && SeafileClient.exe
```

## ⚙️ 配置

程序首次运行时会自动创建配置文件：

- **Windows**: `%APPDATA%\SeafileClient\client.conf`
- **Linux**: `~/.config/SeafileClient/client.conf`
- **macOS**: `~/Library/Application Support/SeafileClient/client.conf`

配置示例请参考 [`config/client.conf.example`](config/client.conf.example)。

## 📖 文档

- [详细编译指南](docs/BUILD.md) - 包含各平台编译说明和故障排除
- [API 文档](docs/API.md) - 代码接口文档
- [开发指南](docs/DEVELOPMENT.md) - 开发环境设置和贡献指南

## 🔄 与服务端集成

本客户端设计用于与以下服务端架构配合：

- **Seafile 后端** - 文件存储和管理
- **FastAPI 服务** - 用户认证、会员管理、充值功能
- **数据库** - 用户和会员信息存储
- **缓存** - 会话管理

## 🛠️ 开发

### 项目结构

```
seafile-cpp-client/
├── CMakeLists.txt          # CMake 配置
├── README.md               # 项目说明
├── LICENSE                 # MIT 许可证
├── .gitignore             # Git 忽略文件
├── include/               # 头文件
│   ├── App.h             # 主应用程序类
│   ├── frames/           # 窗口框架
│   ├── managers/         # 管理器类
│   ├── dialogs/          # 对话框
│   ├── panels/           # 界面面板
│   └── utils/            # 工具类
├── src/                  # 源代码
├── config/               # 配置文件
├── docs/                 # 文档
├── scripts/              # 构建脚本
├── resources/            # 资源文件
└── .github/              # GitHub Actions
```

### 构建选项

```bash
# 调试模式
cmake .. -DCMAKE_BUILD_TYPE=Debug

# 启用测试
cmake .. -DBUILD_TESTS=ON

# 自定义安装路径
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
```

### 代码风格

项目使用 clang-format 进行代码格式化：

```bash
# 格式化所有代码
find src include -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

## 🤝 贡献

欢迎贡献代码！请遵循以下步骤：

1. Fork 本仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 创建 Pull Request

## 📋 待办事项

- [ ] 实现网络通信模块 (HTTP 客户端)
- [ ] 完善文件下载功能 (断点续传、多线程)
- [ ] 添加会员充值系统
- [ ] 实现文件预览功能
- [ ] 支持批量下载
- [ ] 添加自动更新功能
- [ ] 国际化支持 (i18n)
- [ ] 主题系统

## 📄 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件。

## 🙏 致谢

- [wxWidgets](https://www.wxwidgets.org/) - 跨平台 GUI 框架
- [libcurl](https://curl.se/libcurl/) - HTTP 客户端库
- [CMake](https://cmake.org/) - 跨平台构建系统
- [Seafile](https://www.seafile.com/) - 文件同步和共享平台

## 📞 联系方式

- 作者: zhangqinghao0811
- 邮箱: zhangqinghao0811@163.com
- GitHub: [@zhangqinghao0811](https://github.com/zhangqinghao0811)

---

⭐ 如果这个项目对您有帮助，请给个 Star！

