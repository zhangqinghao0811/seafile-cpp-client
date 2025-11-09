/**
 * @file App.h
 * @brief 主应用程序类定义
 * 
 * 定义了应用程序的主类，负责初始化和管理整个应用程序的生命周期
 */

#pragma once

#include <wx/wx.h>
#include <wx/app.h>
#include <memory>

// 前向声明
class MainFrame;
class AuthManager;
class ConfigManager;

/**
 * @brief 主应用程序类
 * 
 * 继承自 wxApp，负责应用程序的初始化、配置管理和主窗口创建
 */
class App : public wxApp
{
public:
    /**
     * @brief 应用程序初始化
     * @return true 初始化成功，false 初始化失败
     */
    virtual bool OnInit() override;

    /**
     * @brief 应用程序退出清理
     * @return 退出代码
     */
    virtual int OnExit() override;

    /**
     * @brief 获取认证管理器
     * @return 认证管理器引用
     */
    AuthManager& GetAuthManager() { return *m_authManager; }

    /**
     * @brief 获取配置管理器
     * @return 配置管理器引用
     */
    ConfigManager& GetConfigManager() { return *m_configManager; }

    /**
     * @brief 显示错误消息
     * @param message 错误消息
     * @param title 对话框标题
     */
    void ShowErrorMessage(const wxString& message, const wxString& title = "错误");

    /**
     * @brief 显示信息消息
     * @param message 信息消息
     * @param title 对话框标题
     */
    void ShowInfoMessage(const wxString& message, const wxString& title = "信息");

private:
    /**
     * @brief 初始化配置系统
     * @return true 成功，false 失败
     */
    bool InitializeConfig();

    /**
     * @brief 初始化管理器
     * @return true 成功，false 失败
     */
    bool InitializeManagers();

    /**
     * @brief 创建主窗口
     * @return true 成功，false 失败
     */
    bool CreateMainWindow();

private:
    MainFrame* m_mainFrame;                           ///< 主窗口指针
    std::unique_ptr<AuthManager> m_authManager;       ///< 认证管理器
    std::unique_ptr<ConfigManager> m_configManager;   ///< 配置管理器
};

/**
 * @brief 获取应用程序实例
 * @return 应用程序实例引用
 */
inline App& GetApp() 
{
    return static_cast<App&>(wxGetApp());
}

// 声明应用程序实例
wxDECLARE_APP(App);
