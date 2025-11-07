# 编译指南

本文档详细说明如何在不同平台上编译 Seafile 客户端。

## 系统要求

### 通用要求
- CMake 3.16 或更高版本
- C++17 兼容的编译器
- wxWidgets 3.1.0 或更高版本
- libcurl 7.60.0 或更高版本

### 平台特定要求

#### Windows
- Visual Studio 2019 或更高版本，或者 MinGW-w64
- vcpkg 包管理器（推荐）

#### Linux
- GCC 7.0 或更高版本，或者 Clang 6.0 或更高版本
- 开发工具包：build-essential

#### macOS
- Xcode 10 或更高版本
- Homebrew 包管理器（推荐）

## 依赖安装

### Windows (使用 vcpkg)

```bash
# 1. 安装 vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat

# 2. 安装依赖包
./vcpkg install wxwidgets:x64-windows
./vcpkg install curl:x64-windows

# 3. 集成到 Visual Studio
./vcpkg integrate install
```

### Linux (Ubuntu/Debian)

```bash
# 更新包列表
sudo apt update

# 安装编译工具
sudo apt install build-essential cmake git

# 安装 wxWidgets 开发包
sudo apt install libwxgtk3.0-gtk3-dev

# 安装 libcurl 开发包
sudo apt install libcurl4-openssl-dev

# 可选：安装其他依赖
sudo apt install libssl-dev zlib1g-dev
```

### Linux (CentOS/RHEL/Fedora)

```bash
# CentOS/RHEL
sudo yum groupinstall "Development Tools"
sudo yum install cmake3 wxGTK3-devel libcurl-devel

# Fedora
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake wxGTK3-devel libcurl-devel
```

### macOS (使用 Homebrew)

```bash
# 安装 Homebrew（如果尚未安装）
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 安装依赖
brew install cmake
brew install wxwidgets
brew install curl
```

## 编译步骤

### 1. 获取源代码

```bash
git clone https://github.com/yourusername/seafile-client.git
cd seafile-client
```

### 2. 创建构建目录

```bash
mkdir build
cd build
```

### 3. 配置项目

#### Windows (Visual Studio)

```bash
# 使用 vcpkg
cmake .. -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

# 或者手动指定路径
cmake .. -DwxWidgets_ROOT_DIR="C:/path/to/wxwidgets" -DCURL_ROOT="C:/path/to/curl"
```

#### Linux/macOS

```bash
# 标准配置
cmake ..

# 指定构建类型
cmake .. -DCMAKE_BUILD_TYPE=Release

# 自定义安装路径
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
```

### 4. 编译项目

#### Windows

```bash
# 使用 MSBuild
cmake --build . --config Release

# 或者在 Visual Studio 中打开 .sln 文件
```

#### Linux/macOS

```bash
# 使用所有可用的 CPU 核心
make -j$(nproc)  # Linux
make -j$(sysctl -n hw.ncpu)  # macOS

# 或者使用 cmake
cmake --build . -j
```

### 5. 安装（可选）

```bash
# Linux/macOS
sudo make install

# Windows
cmake --build . --target install
```

## 构建选项

### CMake 选项

```bash
# 启用调试模式
cmake .. -DCMAKE_BUILD_TYPE=Debug

# 启用测试
cmake .. -DBUILD_TESTS=ON

# 自定义安装前缀
cmake .. -DCMAKE_INSTALL_PREFIX=/opt/seafile-client

# 静态链接（如果支持）
cmake .. -DBUILD_SHARED_LIBS=OFF
```

### 环境变量

```bash
# 指定 wxWidgets 路径
export wxWidgets_ROOT_DIR=/path/to/wxwidgets

# 指定 CURL 路径
export CURL_ROOT=/path/to/curl

# 设置编译器
export CC=gcc-9
export CXX=g++-9
```

## 故障排除

### 常见问题

#### 1. 找不到 wxWidgets

**错误信息：**
```
Could NOT find wxWidgets (missing: wxWidgets_LIBRARIES wxWidgets_INCLUDE_DIRS)
```

**解决方案：**
```bash
# Linux: 安装开发包
sudo apt install libwxgtk3.0-gtk3-dev

# Windows: 设置环境变量
set wxWidgets_ROOT_DIR=C:\path\to\wxwidgets

# macOS: 使用 Homebrew
brew install wxwidgets
```

#### 2. 找不到 libcurl

**错误信息：**
```
Could NOT find CURL (missing: CURL_LIBRARY CURL_INCLUDE_DIR)
```

**解决方案：**
```bash
# Linux
sudo apt install libcurl4-openssl-dev

# Windows (vcpkg)
vcpkg install curl:x64-windows

# macOS
brew install curl
```

#### 3. C++17 支持问题

**错误信息：**
```
error: 'std::filesystem' has not been declared
```

**解决方案：**
```bash
# 确保使用支持 C++17 的编译器
# GCC 7+, Clang 6+, MSVC 2017+

# 显式指定编译器
cmake .. -DCMAKE_CXX_COMPILER=g++-9
```

#### 4. 链接错误

**错误信息：**
```
undefined reference to 'wxEntry'
```

**解决方案：**
```bash
# 确保正确链接 wxWidgets 库
# 检查 CMakeLists.txt 中的 target_link_libraries
```

### 调试技巧

#### 1. 详细输出

```bash
# CMake 详细输出
cmake .. --debug-output

# Make 详细输出
make VERBOSE=1
```

#### 2. 检查依赖

```bash
# Linux: 检查动态库依赖
ldd ./SeafileClient

# macOS: 检查动态库依赖
otool -L ./SeafileClient

# Windows: 使用 Dependency Walker
```

#### 3. 环境检查

```bash
# 检查 CMake 版本
cmake --version

# 检查编译器版本
gcc --version
g++ --version

# 检查 pkg-config
pkg-config --list-all | grep wx
```

## 打包

### 创建安装包

```bash
# 配置 CPack
cmake .. -DCPACK_GENERATOR="DEB;RPM"  # Linux
cmake .. -DCPACK_GENERATOR="NSIS"     # Windows
cmake .. -DCPACK_GENERATOR="DragNDrop" # macOS

# 构建项目
cmake --build .

# 创建包
cpack
```

### 创建便携版

```bash
# 复制可执行文件和依赖库到单独目录
mkdir portable
cp SeafileClient portable/
cp -r resources portable/
cp config/client.conf.example portable/client.conf

# Linux: 复制动态库
ldd SeafileClient | grep "=> /" | awk '{print $3}' | xargs -I '{}' cp '{}' portable/

# 创建启动脚本
echo '#!/bin/bash' > portable/run.sh
echo 'export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH' >> portable/run.sh
echo './SeafileClient' >> portable/run.sh
chmod +x portable/run.sh
```

## 开发环境设置

### IDE 配置

#### Visual Studio Code

创建 `.vscode/settings.json`：
```json
{
    "cmake.configureArgs": [
        "-DCMAKE_BUILD_TYPE=Debug",
        "-DBUILD_TESTS=ON"
    ],
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
}
```

#### CLion

1. 打开项目根目录
2. CLion 会自动检测 CMakeLists.txt
3. 配置 CMake 选项：File → Settings → Build → CMake

### 代码格式化

```bash
# 安装 clang-format
sudo apt install clang-format  # Linux
brew install clang-format     # macOS

# 格式化代码
find src include -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

## 持续集成

### GitHub Actions 示例

创建 `.github/workflows/build.yml`：
```yaml
name: Build

on: [push, pull_request]

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, windows-latest, macos-latest]

    steps:
    - uses: actions/checkout@v2
    
    - name: Install dependencies (Ubuntu)
      if: matrix.os == 'ubuntu-latest'
      run: |
        sudo apt update
        sudo apt install libwxgtk3.0-gtk3-dev libcurl4-openssl-dev
    
    - name: Install dependencies (macOS)
      if: matrix.os == 'macos-latest'
      run: |
        brew install wxwidgets curl
    
    - name: Configure CMake
      run: cmake -B build -DCMAKE_BUILD_TYPE=Release
    
    - name: Build
      run: cmake --build build --config Release
```

这个构建指南应该能帮助您在各种平台上成功编译 Seafile 客户端。如果遇到问题，请检查依赖版本和环境配置。

