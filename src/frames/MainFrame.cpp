#include "frames/MainFrame.h"
#include "panels/FileListPanel.h"
#include "panels/DownloadPanel.h"
#include "panels/MembershipPanel.h"
#include "dialogs/LoginDialog.h"
#include "App.h"
#include "managers/AuthManager.h"
#include "utils/Logger.h"

// 事件表
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_MENU(ID_LOGIN, MainFrame::OnLogin)
    EVT_MENU(ID_LOGOUT, MainFrame::OnLogout)
    EVT_MENU(ID_REFRESH, MainFrame::OnRefresh)
    EVT_MENU(ID_SETTINGS, MainFrame::OnSettings)
    EVT_CLOSE(MainFrame::OnClose)
wxEND_EVENT_TABLE()

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "Seafile客户端", wxDefaultPosition, wxSize(1000, 700))
    , m_notebook(nullptr)
    , m_fileListPanel(nullptr)
    , m_downloadPanel(nullptr)
    , m_membershipPanel(nullptr)
    , m_menuBar(nullptr)
    , m_toolBar(nullptr)
    , m_statusBar(nullptr)
{
    Logger::getInstance().info("创建主窗口");
    
    // 设置窗口图标（如果有的话）
    // SetIcon(wxIcon("app_icon", wxBITMAP_TYPE_ICO_RESOURCE));
    
    // 创建界面组件
    CreateMenuBar();
    CreateToolBar();
    CreateStatusBar();
    CreateMainPanel();
    
    // 绑定事件
    BindEvents();
    
    // 更新界面状态
    UpdateUIState();
    
    // 居中显示
    Center();
    
    Logger::getInstance().info("主窗口创建完成");
}

MainFrame::~MainFrame()
{
    Logger::getInstance().info("销毁主窗口");
}

void MainFrame::CreateMenuBar()
{
    m_menuBar = new wxMenuBar();
    
    // 文件菜单
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(ID_LOGIN, "登录(&L)\tCtrl+L", "登录到服务器");
    fileMenu->Append(ID_LOGOUT, "登出(&O)\tCtrl+O", "从服务器登出");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "退出(&X)\tCtrl+Q", "退出应用程序");
    
    // 操作菜单
    wxMenu* actionMenu = new wxMenu();
    actionMenu->Append(ID_REFRESH, "刷新(&R)\tF5", "刷新文件列表");
    actionMenu->AppendSeparator();
    actionMenu->Append(ID_SETTINGS, "设置(&S)\tCtrl+,", "打开设置对话框");
    
    // 帮助菜单
    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, "关于(&A)", "关于此应用程序");
    
    // 添加到菜单栏
    m_menuBar->Append(fileMenu, "文件(&F)");
    m_menuBar->Append(actionMenu, "操作(&A)");
    m_menuBar->Append(helpMenu, "帮助(&H)");
    
    SetMenuBar(m_menuBar);
}

void MainFrame::CreateToolBar()
{
    m_toolBar = CreateToolBar(wxTB_HORIZONTAL | wxTB_TEXT);
    
    // 添加工具按钮
    m_toolBar->AddTool(ID_LOGIN, "登录", wxNullBitmap, "登录到服务器");
    m_toolBar->AddTool(ID_LOGOUT, "登出", wxNullBitmap, "从服务器登出");
    m_toolBar->AddSeparator();
    m_toolBar->AddTool(ID_REFRESH, "刷新", wxNullBitmap, "刷新文件列表");
    
    m_toolBar->Realize();
}

void MainFrame::CreateStatusBar()
{
    m_statusBar = CreateStatusBar(3);
    
    // 设置状态栏字段宽度
    int widths[] = {-1, 150, 100};
    m_statusBar->SetStatusWidths(3, widths);
    
    // 设置初始状态
    m_statusBar->SetStatusText("就绪", 0);
    m_statusBar->SetStatusText("未登录", 1);
    m_statusBar->SetStatusText("非会员", 2);
}

void MainFrame::CreateMainPanel()
{
    // 创建主选项卡控件
    m_notebook = new wxNotebook(this, wxID_ANY);
    
    // 创建各个面板
    m_fileListPanel = new FileListPanel(m_notebook);
    m_downloadPanel = new DownloadPanel(m_notebook);
    m_membershipPanel = new MembershipPanel(m_notebook);
    
    // 添加到选项卡
    m_notebook->AddPage(m_fileListPanel, "文件列表", true);
    m_notebook->AddPage(m_downloadPanel, "下载管理", false);
    m_notebook->AddPage(m_membershipPanel, "会员中心", false);
    
    // 创建主布局
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_notebook, 1, wxEXPAND | wxALL, 5);
    
    SetSizer(mainSizer);
}

void MainFrame::BindEvents()
{
    // 可以在这里绑定自定义事件
}

void MainFrame::UpdateUIState()
{
    AuthManager& authManager = GetApp().GetAuthManager();
    
    bool isLoggedIn = authManager.isLoggedIn();
    bool isMember = authManager.isMember();
    
    // 更新菜单状态
    UpdateMenuState();
    
    // 更新工具栏状态
    UpdateToolBarState();
    
    // 更新状态栏
    if (isLoggedIn) {
        m_statusBar->SetStatusText("已登录: " + authManager.getCurrentUsername(), 1);
        m_statusBar->SetStatusText(isMember ? "会员用户" : "普通用户", 2);
    } else {
        m_statusBar->SetStatusText("未登录", 1);
        m_statusBar->SetStatusText("非会员", 2);
    }
    
    // 更新面板状态
    if (m_fileListPanel) {
        m_fileListPanel->UpdateUIState();
    }
    if (m_downloadPanel) {
        m_downloadPanel->UpdateUIState();
    }
    if (m_membershipPanel) {
        m_membershipPanel->UpdateUIState();
    }
}

void MainFrame::UpdateMenuState()
{
    AuthManager& authManager = GetApp().GetAuthManager();
    bool isLoggedIn = authManager.isLoggedIn();
    
    // 更新菜单项状态
    m_menuBar->Enable(ID_LOGIN, !isLoggedIn);
    m_menuBar->Enable(ID_LOGOUT, isLoggedIn);
    m_menuBar->Enable(ID_REFRESH, isLoggedIn && authManager.isMember());
}

void MainFrame::UpdateToolBarState()
{
    AuthManager& authManager = GetApp().GetAuthManager();
    bool isLoggedIn = authManager.isLoggedIn();
    
    // 更新工具栏按钮状态
    m_toolBar->EnableTool(ID_LOGIN, !isLoggedIn);
    m_toolBar->EnableTool(ID_LOGOUT, isLoggedIn);
    m_toolBar->EnableTool(ID_REFRESH, isLoggedIn && authManager.isMember());
}

void MainFrame::ShowLoginDialog()
{
    LoginDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK) {
        // 登录成功，更新界面状态
        UpdateUIState();
        UpdateStatusBar("登录成功");
        
        // 如果是会员，自动刷新文件列表
        if (GetApp().GetAuthManager().isMember()) {
            RefreshFileList();
        }
    }
}

void MainFrame::RefreshFileList()
{
    if (m_fileListPanel) {
        m_fileListPanel->RefreshFileList();
        UpdateStatusBar("文件列表已刷新");
    }
}

void MainFrame::UpdateStatusBar(const wxString& message, int field)
{
    if (m_statusBar) {
        m_statusBar->SetStatusText(message, field);
    }
}

// 事件处理函数
void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("Seafile会员制文件下载客户端\n\n"
                 "版本: 1.0.0\n"
                 "基于wxWidgets开发",
                 "关于",
                 wxOK | wxICON_INFORMATION,
                 this);
}

void MainFrame::OnLogin(wxCommandEvent& WXUNUSED(event))
{
    ShowLoginDialog();
}

void MainFrame::OnLogout(wxCommandEvent& WXUNUSED(event))
{
    AuthManager& authManager = GetApp().GetAuthManager();
    
    if (wxMessageBox("确定要登出吗？", "确认", wxYES_NO | wxICON_QUESTION, this) == wxYES) {
        authManager.logout();
        UpdateUIState();
        UpdateStatusBar("已登出");
        
        // 清空文件列表
        if (m_fileListPanel) {
            m_fileListPanel->ClearFileList();
        }
    }
}

void MainFrame::OnRefresh(wxCommandEvent& WXUNUSED(event))
{
    RefreshFileList();
}

void MainFrame::OnSettings(wxCommandEvent& WXUNUSED(event))
{
    // TODO: 实现设置对话框
    wxMessageBox("设置功能尚未实现", "提示", wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnClose(wxCloseEvent& event)
{
    Logger::getInstance().info("用户关闭主窗口");
    
    // 可以在这里添加关闭前的确认对话框
    if (event.CanVeto()) {
        if (wxMessageBox("确定要退出应用程序吗？", "确认退出", 
                        wxYES_NO | wxICON_QUESTION, this) != wxYES) {
            event.Veto();
            return;
        }
    }
    
    // 保存窗口状态
    // TODO: 保存窗口位置和大小到配置文件
    
    Destroy();
}

