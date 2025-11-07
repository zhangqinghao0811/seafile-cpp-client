cmake_minimum_required(VERSION 3.16)
project(SeafileClient VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 查找依赖库
find_package(wxWidgets REQUIRED COMPONENTS core base net)
find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)

# 检查wxWidgets
if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    message(STATUS "Found wxWidgets: ${wxWidgets_VERSION}")
else()
    message(FATAL_ERROR "wxWidgets not found!")
endif()

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party)

# 源文件
set(SOURCES
    src/main.cpp
    src/App.cpp
    
    # 框架
    src/frames/MainFrame.cpp
    
    # 面板
    src/panels/FileListPanel.cpp
    src/panels/DownloadPanel.cpp
    src/panels/MembershipPanel.cpp
    
    # 对话框
    src/dialogs/LoginDialog.cpp
    src/dialogs/PaymentDialog.cpp
    
    # 网络
    src/network/APIClient.cpp
    src/network/HttpClient.cpp
    
    # 管理器
    src/managers/AuthManager.cpp
    src/managers/ConfigManager.cpp
    
    # 下载
    src/download/FileDownloader.cpp
    src/download/DownloadManager.cpp
    
    # 工具类
    src/utils/Logger.cpp
    src/utils/Utils.cpp
    src/utils/ErrorHandler.cpp
    
    # 模型
    src/models/FileInfo.cpp
)

# 头文件
set(HEADERS
    include/App.h
    
    # 框架
    include/frames/MainFrame.h
    
    # 面板
    include/panels/FileListPanel.h
    include/panels/DownloadPanel.h
    include/panels/MembershipPanel.h
    
    # 对话框
    include/dialogs/LoginDialog.h
    include/dialogs/PaymentDialog.h
    
    # 网络
    include/network/APIClient.h
    include/network/HttpClient.h
    
    # 管理器
    include/managers/AuthManager.h
    include/managers/ConfigManager.h
    
    # 下载
    include/download/FileDownloader.h
    include/download/DownloadManager.h
    
    # 工具类
    include/utils/Logger.h
    include/utils/Utils.h
    include/utils/ErrorHandler.h
    
    # 模型
    include/models/FileInfo.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 链接库
target_link_libraries(${PROJECT_NAME} 
    ${wxWidgets_LIBRARIES}
    ${CURL_LIBRARIES}
)

# 编译器特定设置
if(WIN32)
    # Windows特定设置
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    
    # 添加Windows资源文件（如果有的话）
    if(EXISTS "${CMAKE_SOURCE_DIR}/resources/app.rc")
        target_sources(${PROJECT_NAME} PRIVATE resources/app.rc)
    endif()
    
elseif(APPLE)
    # macOS特定设置
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/resources/Info.plist"
    )
    
else()
    # Linux特定设置
    # 可以在这里添加Linux特定的配置
endif()

# 安装规则
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# 复制配置文件
install(FILES config/client.conf.example
    DESTINATION etc/${PROJECT_NAME}
    RENAME client.conf
)

# 复制资源文件
install(DIRECTORY resources/
    DESTINATION share/${PROJECT_NAME}/resources
    FILES_MATCHING PATTERN "*"
)

# 打包设置
set(CPACK_PACKAGE_NAME "SeafileClient")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Seafile会员制文件下载客户端")
set(CPACK_PACKAGE_VENDOR "Your Company")

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
else()
    set(CPACK_GENERATOR "DEB;RPM")
endif()

include(CPack)

# 开发者选项
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# 调试信息
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")

