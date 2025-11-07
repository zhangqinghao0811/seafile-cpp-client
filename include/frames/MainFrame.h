#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/statusbr.h>
#include <memory>

class FileListPanel;
class DownloadPanel;
class MembershipPanel;
class AuthManager;

/**
 * @brief 主窗口类
 * 
 * 应用程序的主界面，包含菜单栏、工具栏、状态栏和主要功能面板
 */
class MainFrame : public wxFrame
{
public:
    MainFrame();
    virtual ~MainFrame();

    /**
     * @brief 更新用户界面状态
     * 根据用户登录状态和会员状态更新界面
     */
    void UpdateUIState();
    
    /**
     * @brief 显示登录对话框
     */
    void ShowLoginDialog();
    
    /**
     * @brief 刷新文件列表
     */
    void RefreshFileList();
    
    /**
     * @brief 更新状态栏信息
     * @param message 状态消息
     * @param field 状态栏字段索引
     */
    void UpdateStatusBar(const wxString& message, int field = 0);

private:
    // 事件处理函数
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnLogin(wxCommandEvent& event);
    void OnLogout(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    
    // 界面创建函数
    void CreateMenuBar();
    void CreateToolBar();
    void CreateStatusBar();
    void CreateMainPanel();
    
    // 界面更新函数
    void UpdateMenuState();
    void UpdateToolBarState();
    
    // 事件绑定
    void BindEvents();

private:
    // 界面组件
    wxNotebook* m_notebook;              ///< 主选项卡控件
    FileListPanel* m_fileListPanel;      ///< 文件列表面板
    DownloadPanel* m_downloadPanel;      ///< 下载面板
    MembershipPanel* m_membershipPanel;  ///< 会员面板
    
    // 菜单和工具栏
    wxMenuBar* m_menuBar;               ///< 菜单栏
    wxToolBar* m_toolBar;               ///< 工具栏
    wxStatusBar* m_statusBar;           ///< 状态栏
    
    // 菜单项ID
    enum MenuIds {
        ID_LOGIN = wxID_HIGHEST + 1,
        ID_LOGOUT,
        ID_REFRESH,
        ID_SETTINGS
    };

    wxDECLARE_EVENT_TABLE();
};

