#!/bin/bash

# Seafile Client 构建脚本 (Linux/macOS)
# 使用方法: ./scripts/build.sh [选项]

set -e  # 遇到错误时退出

# 默认配置
BUILD_TYPE="Release"
BUILD_DIR="build"
INSTALL_PREFIX="/usr/local"
ENABLE_TESTS="OFF"
CLEAN_BUILD="false"
VERBOSE="false"
JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo "4")

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 打印带颜色的消息
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 显示帮助信息
show_help() {
    cat << EOF
Seafile Client 构建脚本

使用方法: $0 [选项]

选项:
    -h, --help              显示此帮助信息
    -t, --type TYPE         构建类型 (Debug|Release|RelWithDebInfo) [默认: Release]
    -d, --dir DIR           构建目录 [默认: build]
    -p, --prefix PREFIX     安装前缀 [默认: /usr/local]
    -j, --jobs JOBS         并行编译任务数 [默认: 自动检测]
    --enable-tests          启用测试构建
    --clean                 清理构建目录
    --verbose               详细输出
    --install               编译后自动安装

示例:
    $0                      # 使用默认设置构建
    $0 --type Debug         # 构建调试版本
    $0 --clean --verbose    # 清理构建并显示详细输出
    $0 --enable-tests       # 构建并启用测试
EOF
}

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -t|--type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -d|--dir)
            BUILD_DIR="$2"
            shift 2
            ;;
        -p|--prefix)
            INSTALL_PREFIX="$2"
            shift 2
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        --enable-tests)
            ENABLE_TESTS="ON"
            shift
            ;;
        --clean)
            CLEAN_BUILD="true"
            shift
            ;;
        --verbose)
            VERBOSE="true"
            shift
            ;;
        --install)
            DO_INSTALL="true"
            shift
            ;;
        *)
            print_error "未知选项: $1"
            show_help
            exit 1
            ;;
    esac
done

# 检查系统和依赖
check_dependencies() {
    print_info "检查系统依赖..."
    
    # 检查 CMake
    if ! command -v cmake &> /dev/null; then
        print_error "CMake 未安装，请先安装 CMake"
        exit 1
    fi
    
    CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
    print_info "CMake 版本: $CMAKE_VERSION"
    
    # 检查编译器
    if command -v g++ &> /dev/null; then
        GCC_VERSION=$(g++ --version | head -n1)
        print_info "编译器: $GCC_VERSION"
    elif command -v clang++ &> /dev/null; then
        CLANG_VERSION=$(clang++ --version | head -n1)
        print_info "编译器: $CLANG_VERSION"
    else
        print_error "未找到 C++ 编译器 (g++ 或 clang++)"
        exit 1
    fi
    
    # 检查 pkg-config
    if ! command -v pkg-config &> /dev/null; then
        print_warning "pkg-config 未安装，可能影响依赖库检测"
    fi
    
    # 检查 wxWidgets
    if pkg-config --exists wxwidgets; then
        WX_VERSION=$(pkg-config --modversion wxwidgets)
        print_info "wxWidgets 版本: $WX_VERSION"
    else
        print_warning "无法通过 pkg-config 检测 wxWidgets，请确保已正确安装"
    fi
    
    # 检查 libcurl
    if pkg-config --exists libcurl; then
        CURL_VERSION=$(pkg-config --modversion libcurl)
        print_info "libcurl 版本: $CURL_VERSION"
    else
        print_warning "无法通过 pkg-config 检测 libcurl，请确保已正确安装"
    fi
}

# 安装依赖（如果需要）
install_dependencies() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        if command -v apt-get &> /dev/null; then
            print_info "检测到 Ubuntu/Debian 系统"
            print_info "如需安装依赖，请运行:"
            echo "sudo apt update"
            echo "sudo apt install build-essential cmake libwxgtk3.0-gtk3-dev libcurl4-openssl-dev"
        elif command -v yum &> /dev/null; then
            print_info "检测到 CentOS/RHEL 系统"
            print_info "如需安装依赖，请运行:"
            echo "sudo yum groupinstall \"Development Tools\""
            echo "sudo yum install cmake3 wxGTK3-devel libcurl-devel"
        elif command -v dnf &> /dev/null; then
            print_info "检测到 Fedora 系统"
            print_info "如需安装依赖，请运行:"
            echo "sudo dnf groupinstall \"Development Tools\""
            echo "sudo dnf install cmake wxGTK3-devel libcurl-devel"
        fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        print_info "检测到 macOS 系统"
        if command -v brew &> /dev/null; then
            print_info "如需安装依赖，请运行:"
            echo "brew install cmake wxwidgets curl"
        else
            print_info "建议安装 Homebrew: https://brew.sh/"
        fi
    fi
}

# 清理构建目录
clean_build() {
    if [[ "$CLEAN_BUILD" == "true" ]]; then
        print_info "清理构建目录: $BUILD_DIR"
        rm -rf "$BUILD_DIR"
    fi
}

# 配置项目
configure_project() {
    print_info "配置项目..."
    
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    CMAKE_ARGS=(
        "-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
        "-DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX"
        "-DBUILD_TESTS=$ENABLE_TESTS"
    )
    
    if [[ "$VERBOSE" == "true" ]]; then
        CMAKE_ARGS+=("--debug-output")
    fi
    
    print_info "CMake 参数: ${CMAKE_ARGS[*]}"
    
    if ! cmake .. "${CMAKE_ARGS[@]}"; then
        print_error "CMake 配置失败"
        exit 1
    fi
    
    cd ..
}

# 编译项目
build_project() {
    print_info "开始编译项目..."
    print_info "构建类型: $BUILD_TYPE"
    print_info "并行任务数: $JOBS"
    
    cd "$BUILD_DIR"
    
    BUILD_ARGS=("--build" "." "--config" "$BUILD_TYPE" "-j" "$JOBS")
    
    if [[ "$VERBOSE" == "true" ]]; then
        BUILD_ARGS+=("--verbose")
    fi
    
    if ! cmake "${BUILD_ARGS[@]}"; then
        print_error "编译失败"
        exit 1
    fi
    
    cd ..
    print_success "编译完成"
}

# 运行测试
run_tests() {
    if [[ "$ENABLE_TESTS" == "ON" ]]; then
        print_info "运行测试..."
        cd "$BUILD_DIR"
        
        if ! ctest --output-on-failure; then
            print_warning "部分测试失败"
        else
            print_success "所有测试通过"
        fi
        
        cd ..
    fi
}

# 安装项目
install_project() {
    if [[ "$DO_INSTALL" == "true" ]]; then
        print_info "安装项目到: $INSTALL_PREFIX"
        cd "$BUILD_DIR"
        
        if [[ "$INSTALL_PREFIX" == "/usr/local" ]] || [[ "$INSTALL_PREFIX" == "/usr" ]]; then
            if ! sudo cmake --build . --target install; then
                print_error "安装失败"
                exit 1
            fi
        else
            if ! cmake --build . --target install; then
                print_error "安装失败"
                exit 1
            fi
        fi
        
        cd ..
        print_success "安装完成"
    fi
}

# 显示构建信息
show_build_info() {
    print_success "构建完成！"
    echo
    print_info "构建信息:"
    echo "  构建类型: $BUILD_TYPE"
    echo "  构建目录: $BUILD_DIR"
    echo "  可执行文件: $BUILD_DIR/SeafileClient"
    echo
    print_info "运行程序:"
    echo "  cd $BUILD_DIR && ./SeafileClient"
    echo
    if [[ "$DO_INSTALL" != "true" ]]; then
        print_info "安装程序:"
        echo "  $0 --install"
        echo "  或者: cd $BUILD_DIR && sudo make install"
    fi
}

# 主函数
main() {
    print_info "Seafile Client 构建脚本"
    print_info "=============================="
    
    # 检查依赖
    check_dependencies
    
    # 显示依赖安装提示
    install_dependencies
    
    # 清理构建目录
    clean_build
    
    # 配置项目
    configure_project
    
    # 编译项目
    build_project
    
    # 运行测试
    run_tests
    
    # 安装项目
    install_project
    
    # 显示构建信息
    show_build_info
}

# 运行主函数
main "$@"

