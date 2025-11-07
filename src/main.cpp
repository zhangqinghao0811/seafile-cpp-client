#include "App.h"
#include "utils/Logger.h"

// wxWidgets应用程序入口点
wxIMPLEMENT_APP(App);

int main(int argc, char* argv[])
{
    // 初始化日志系统
    Logger::getInstance().initialize("seafile-client.log");
    Logger::getInstance().info("应用程序启动");
    
    // 启动wxWidgets应用程序
    return wxEntry(argc, argv);
}

