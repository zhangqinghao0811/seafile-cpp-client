#include "App.h"
#include "frames/MainFrame.h"
#include "managers/AuthManager.h"
#include "managers/ConfigManager.h"
#include "utils/Logger.h"
#include "utils/ErrorHandler.h"

App::App()
    : m_mainFrame(nullptr)
    , m_authManager(nullptr)
    , m_configManager(nullptr)
{
}

App::~App()
{
    Logger::getInstance().info("应用程序退出");
}

bool App::OnInit()
{
    Logger::getInstance().info("开始初始化应用程序");
    
    // 设置应用程序名称
    SetAppName("SeafileClient");
    SetAppDisplayName("Seafile客户端");
    SetVendorName("Your Company");
    
    try {
        // 初始化配置
        if (!InitializeConfig()) {
            Logger::getInstance().error("配置初始化失败");
            return false;
        }
        
        // 初始化管理器
        if (!InitializeManagers()) {
            Logger::getInstance().error("管理器初始化失败");
            return false;
        }
        
        // 创建主窗口
        if (!CreateMainWindow()) {
            Logger::getInstance().error("主窗口创建失败");
            return false;
        }
        
        Logger::getInstance().info("应用程序初始化完成");
        return true;
        
    } catch (const std::exception& e) {
        Logger::getInstance().error("应用程序初始化异常: " + std::string(e.what()));
        ErrorHandler::getInstance().handleException(e);
        return false;
    }
}

int App::OnExit()
{
    Logger::getInstance().info("应用程序正在退出");
    
    // 清理资源
    m_authManager.reset();
    m_configManager.reset();
    
    return wxApp::OnExit();
}

bool App::InitializeConfig()
{
    try {
        m_configManager = std::make_unique<ConfigManager>();
        
        // 加载配置文件
        if (!m_configManager->loadConfig()) {
            Logger::getInstance().warning("配置文件加载失败，使用默认配置");
            // 创建默认配置
            m_configManager->createDefaultConfig();
        }
        
        Logger::getInstance().info("配置初始化成功");
        return true;
        
    } catch (const std::exception& e) {
        Logger::getInstance().error("配置初始化异常: " + std::string(e.what()));
        return false;
    }
}

bool App::InitializeManagers()
{
    try {
        // 初始化认证管理器
        m_authManager = std::make_unique<AuthManager>(*m_configManager);
        
        // 尝试从本地存储恢复登录状态
        m_authManager->restoreSession();
        
        Logger::getInstance().info("管理器初始化成功");
        return true;
        
    } catch (const std::exception& e) {
        Logger::getInstance().error("管理器初始化异常: " + std::string(e.what()));
        return false;
    }
}

bool App::CreateMainWindow()
{
    try {
        // 创建主窗口
        m_mainFrame = new MainFrame();
        
        // 设置为顶级窗口
        SetTopWindow(m_mainFrame);
        
        // 显示主窗口
        m_mainFrame->Show(true);
        
        Logger::getInstance().info("主窗口创建成功");
        return true;
        
    } catch (const std::exception& e) {
        Logger::getInstance().error("主窗口创建异常: " + std::string(e.what()));
        return false;
    }
}

void App::ShowError(const wxString& message, const wxString& title)
{
    Logger::getInstance().error(message.ToStdString());
    wxMessageBox(message, title, wxOK | wxICON_ERROR);
}

void App::ShowInfo(const wxString& message, const wxString& title)
{
    Logger::getInstance().info(message.ToStdString());
    wxMessageBox(message, title, wxOK | wxICON_INFORMATION);
}

