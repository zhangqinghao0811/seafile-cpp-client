/**
 * @file MainFrame.h
 * @brief 主窗口类定义
 * 
 * 定义应用程序的主窗口，包含菜单栏、工具栏、状态栏和主要界面组件
 */

#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>

// 前向声明
class FileListPanel;
class DownloadPanel;
class MembershipPanel;

/**
 * @brief 主窗口类
 * 
 * 应用程序的主窗口，继承自 wxFrame
 */
class MainFrame : public wxFrame
{
public:
    /**
     * @brief 构造函数
     */
    MainFrame();

    /**
     * @brief 析构函数
     */
    virtual ~MainFrame();

    /**
     * @brief 更新界面状态
     * 
     * 根据用户登录状态和会员状态更新界面元素的可用性
     */
    void UpdateUIState();

private:
    /**
     * @brief 创建菜单栏
     */
    void SetupMenuBar();

    /**
     * @brief 创建工具栏
     */
    void SetupToolBar();

    /**
     * @brief 创建状态栏
     */
    void SetupStatusBar();

    /**
     * @brief 创建主要界面组件
     */
    void CreateMainComponents();

    // 事件处理函数
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnLogin(wxCommandEvent& event);
    void OnLogout(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

private:
    // 界面组件
    wxNotebook* m_notebook;           ///< 主选项卡控件
    FileListPanel* m_fileListPanel;   ///< 文件列表面板
    DownloadPanel* m_downloadPanel;   ///< 下载管理面板
    MembershipPanel* m_membershipPanel; ///< 会员中心面板

    // 菜单和工具栏
    wxMenuBar* m_menuBar;             ///< 菜单栏
    wxToolBar* m_toolBar;             ///< 工具栏
    wxStatusBar* m_statusBar;         ///< 状态栏

    // 菜单项
    wxMenu* m_fileMenu;               ///< 文件菜单
    wxMenu* m_actionMenu;             ///< 操作菜单
    wxMenu* m_helpMenu;               ///< 帮助菜单

    // 事件ID
    enum {
        ID_LOGIN = wxID_HIGHEST + 1,
        ID_LOGOUT,
        ID_REFRESH,
        ID_SETTINGS
    };

    wxDECLARE_EVENT_TABLE();
};

