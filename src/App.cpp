/**
 * @file App.cpp
 * @brief 主应用程序类实现
 */

#include "App.h"
#include "frames/MainFrame.h"
#include "managers/AuthManager.h"
#include "managers/ConfigManager.h"
#include "utils/Logger.h"
#include "utils/ErrorHandler.h"

// 实现应用程序实例
wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    try {
        // 初始化配置系统
        if (!InitializeConfig()) {
            wxMessageBox("配置系统初始化失败", "错误", wxOK | wxICON_ERROR);
            return false;
        }

        // 初始化日志系统
        std::string logPath = m_configManager->GetLogPath();
        if (!Logger::getInstance().initialize(logPath)) {
            wxMessageBox("日志系统初始化失败", "错误", wxOK | wxICON_ERROR);
            return false;
        }

        Logger::getInstance().info("应用程序启动...");

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
    }
    catch (const std::exception& e) {
        ErrorHandler::getInstance().handleException(e);
        return false;
    }
}

int App::OnExit()
{
    Logger::getInstance().info("应用程序正在退出...");

    // 清理资源
    m_authManager.reset();
    m_configManager.reset();

    // 关闭日志系统
    Logger::getInstance().close();

    return wxApp::OnExit();
}

void App::ShowErrorMessage(const wxString& message, const wxString& title)
{
    wxMessageBox(message, title, wxOK | wxICON_ERROR);
    Logger::getInstance().error(message.ToStdString());
}

void App::ShowInfoMessage(const wxString& message, const wxString& title)
{
    wxMessageBox(message, title, wxOK | wxICON_INFORMATION);
    Logger::getInstance().info(message.ToStdString());
}

bool App::InitializeConfig()
{
    try {
        m_configManager = std::make_unique<ConfigManager>();
        return m_configManager->Initialize();
    }
    catch (const std::exception& e) {
        wxMessageBox(wxString::Format("配置管理器初始化失败: %s", e.what()),
                     "错误", wxOK | wxICON_ERROR);
        return false;
    }
}

bool App::InitializeManagers()
{
    try {
        // 创建认证管理器
        m_authManager = std::make_unique<AuthManager>();
        
        // 设置服务器配置
        std::string serverUrl = m_configManager->GetServerUrl();
        if (!serverUrl.empty()) {
            m_authManager->SetServerUrl(serverUrl);
        }

        return true;
    }
    catch (const std::exception& e) {
        ShowErrorMessage(wxString::Format("管理器初始化失败: %s", e.what()));
        return false;
    }
}

bool App::CreateMainWindow()
{
    try {
        m_mainFrame = new MainFrame();
        m_mainFrame->Show(true);
        SetTopWindow(m_mainFrame);
        return true;
    }
    catch (const std::exception& e) {
        ShowErrorMessage(wxString::Format("主窗口创建失败: %s", e.what()));
        return false;
    }
}

