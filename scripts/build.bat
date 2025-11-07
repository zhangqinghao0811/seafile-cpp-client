@echo off
REM Seafile Client 构建脚本 (Windows)
REM 使用方法: scripts\build.bat [选项]

setlocal enabledelayedexpansion

REM 默认配置
set BUILD_TYPE=Release
set BUILD_DIR=build
set ENABLE_TESTS=OFF
set CLEAN_BUILD=false
set VERBOSE=false
set VCPKG_ROOT=
set GENERATOR=

REM 检查参数
:parse_args
if "%~1"=="" goto :check_environment
if "%~1"=="--help" goto :show_help
if "%~1"=="-h" goto :show_help
if "%~1"=="--type" (
    set BUILD_TYPE=%~2
    shift
    shift
    goto :parse_args
)
if "%~1"=="--dir" (
    set BUILD_DIR=%~2
    shift
    shift
    goto :parse_args
)
if "%~1"=="--vcpkg" (
    set VCPKG_ROOT=%~2
    shift
    shift
    goto :parse_args
)
if "%~1"=="--generator" (
    set GENERATOR=%~2
    shift
    shift
    goto :parse_args
)
if "%~1"=="--enable-tests" (
    set ENABLE_TESTS=ON
    shift
    goto :parse_args
)
if "%~1"=="--clean" (
    set CLEAN_BUILD=true
    shift
    goto :parse_args
)
if "%~1"=="--verbose" (
    set VERBOSE=true
    shift
    goto :parse_args
)
echo 未知选项: %~1
goto :show_help

:show_help
echo Seafile Client 构建脚本 (Windows)
echo.
echo 使用方法: %~nx0 [选项]
echo.
echo 选项:
echo     -h, --help              显示此帮助信息
echo     --type TYPE             构建类型 (Debug^|Release^|RelWithDebInfo) [默认: Release]
echo     --dir DIR               构建目录 [默认: build]
echo     --vcpkg PATH            vcpkg 根目录路径
echo     --generator GEN         CMake 生成器 (如: "Visual Studio 16 2019")
echo     --enable-tests          启用测试构建
echo     --clean                 清理构建目录
echo     --verbose               详细输出
echo.
echo 示例:
echo     %~nx0                                    # 使用默认设置构建
echo     %~nx0 --type Debug                       # 构建调试版本
echo     %~nx0 --vcpkg C:\vcpkg                   # 指定 vcpkg 路径
echo     %~nx0 --generator "Visual Studio 16 2019" # 指定生成器
goto :eof

:check_environment
echo [INFO] Seafile Client 构建脚本 (Windows)
echo [INFO] ======================================

REM 检查 CMake
cmake --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] CMake 未安装或不在 PATH 中
    echo [INFO] 请从 https://cmake.org/download/ 下载并安装 CMake
    exit /b 1
)

for /f "tokens=3" %%i in ('cmake --version ^| findstr /C:"cmake version"') do (
    echo [INFO] CMake 版本: %%i
)

REM 检查 Visual Studio
if "%GENERATOR%"=="" (
    REM 自动检测 Visual Studio
    if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022" (
        set GENERATOR=Visual Studio 17 2022
        echo [INFO] 检测到 Visual Studio 2022
    ) else if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019" (
        set GENERATOR=Visual Studio 16 2019
        echo [INFO] 检测到 Visual Studio 2019
    ) else (
        echo [WARNING] 未检测到 Visual Studio，将使用默认生成器
    )
)

REM 检查 vcpkg
if "%VCPKG_ROOT%"=="" (
    if defined VCPKG_ROOT (
        echo [INFO] 使用环境变量中的 vcpkg: %VCPKG_ROOT%
    ) else (
        echo [WARNING] 未指定 vcpkg 路径，请确保依赖库已正确安装
        echo [INFO] 建议安装 vcpkg 并使用 --vcpkg 参数指定路径
    )
) else (
    if not exist "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" (
        echo [ERROR] vcpkg 路径无效: %VCPKG_ROOT%
        exit /b 1
    )
    echo [INFO] 使用 vcpkg: %VCPKG_ROOT%
)

:clean_build
if "%CLEAN_BUILD%"=="true" (
    echo [INFO] 清理构建目录: %BUILD_DIR%
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
)

:configure_project
echo [INFO] 配置项目...

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

REM 构建 CMake 参数
set CMAKE_ARGS=-DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DBUILD_TESTS=%ENABLE_TESTS%

if not "%GENERATOR%"=="" (
    set CMAKE_ARGS=%CMAKE_ARGS% -G "%GENERATOR%"
)

if not "%VCPKG_ROOT%"=="" (
    set CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
)

if "%VERBOSE%"=="true" (
    set CMAKE_ARGS=%CMAKE_ARGS% --debug-output
)

echo [INFO] CMake 参数: %CMAKE_ARGS%

cmake .. %CMAKE_ARGS%
if errorlevel 1 (
    echo [ERROR] CMake 配置失败
    cd ..
    exit /b 1
)

cd ..

:build_project
echo [INFO] 开始编译项目...
echo [INFO] 构建类型: %BUILD_TYPE%

cd "%BUILD_DIR%"

set BUILD_ARGS=--build . --config %BUILD_TYPE%

if "%VERBOSE%"=="true" (
    set BUILD_ARGS=%BUILD_ARGS% --verbose
)

cmake %BUILD_ARGS%
if errorlevel 1 (
    echo [ERROR] 编译失败
    cd ..
    exit /b 1
)

cd ..
echo [SUCCESS] 编译完成

:run_tests
if "%ENABLE_TESTS%"=="ON" (
    echo [INFO] 运行测试...
    cd "%BUILD_DIR%"
    
    ctest --output-on-failure -C %BUILD_TYPE%
    if errorlevel 1 (
        echo [WARNING] 部分测试失败
    ) else (
        echo [SUCCESS] 所有测试通过
    )
    
    cd ..
)

:show_build_info
echo.
echo [SUCCESS] 构建完成！
echo.
echo [INFO] 构建信息:
echo   构建类型: %BUILD_TYPE%
echo   构建目录: %BUILD_DIR%
echo   可执行文件: %BUILD_DIR%\%BUILD_TYPE%\SeafileClient.exe
echo.
echo [INFO] 运行程序:
echo   cd %BUILD_DIR%\%BUILD_TYPE% ^&^& SeafileClient.exe
echo.

:install_info
echo [INFO] 安装依赖库 (如果需要):
echo.
echo 1. 安装 vcpkg:
echo    git clone https://github.com/Microsoft/vcpkg.git
echo    cd vcpkg
echo    .\bootstrap-vcpkg.bat
echo.
echo 2. 安装依赖:
echo    .\vcpkg install wxwidgets:x64-windows
echo    .\vcpkg install curl:x64-windows
echo.
echo 3. 集成到 Visual Studio:
echo    .\vcpkg integrate install
echo.
echo 4. 重新运行构建脚本:
echo    %~nx0 --vcpkg C:\path\to\vcpkg

goto :eof

