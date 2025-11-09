/**
 * @file MainFrame.cpp
 * @brief 主窗口类实现
 */

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
    : wxFrame(nullptr, wxID_ANY, "Seafile 客户端", wxDefaultPosition, wxSize(1000, 700))
    , m_notebook(nullptr)
    , m_fileListPanel(nullptr)
    , m_downloadPanel(nullptr)
    , m_membershipPanel(nullptr)
    , m_menuBar(nullptr)
    , m_toolBar(nullptr)
    , m_statusBar(nullptr)
    , m_fileMenu(nullptr)
    , m_actionMenu(nullptr)
    , m_helpMenu(nullptr)
{
    Logger::getInstance().info("正在创建主窗口...");

    // 设置窗口图标
    SetIcon(wxIcon("app_icon", wxBITMAP_TYPE_ICO_RESOURCE));

    // 创建界面组件
    SetupMenuBar();
    SetupToolBar();
    SetupStatusBar();
    CreateMainComponents();

    // 初始化界面状态
    UpdateUIState();

    // 居中显示
    Center();

    Logger::getInstance().info("主窗口创建完成");
}

MainFrame::~MainFrame()
{
    Logger::getInstance().info("主窗口正在销毁...");
}

void MainFrame::SetupMenuBar()
{
    // 创建菜单栏
    m_menuBar = new wxMenuBar();

    // 文件菜单
    m_fileMenu = new wxMenu();
    m_fileMenu->Append(ID_LOGIN, "登录(&L)\tCtrl+L", "登录到服务器");
    m_fileMenu->Append(ID_LOGOUT, "注销(&O)\tCtrl+O", "从服务器注销");
    m_fileMenu->AppendSeparator();
    m_fileMenu->Append(wxID_EXIT, "退出(&X)\tAlt+F4", "退出应用程序");

    // 操作菜单
    m_actionMenu = new wxMenu();
    m_actionMenu->Append(ID_REFRESH, "刷新(&R)\tF5", "刷新文件列表");
    m_actionMenu->AppendSeparator();
    m_actionMenu->Append(ID_SETTINGS, "设置(&S)\tCtrl+,", "打开设置对话框");

    // 帮助菜单
    m_helpMenu = new wxMenu();
    m_helpMenu->Append(wxID_ABOUT, "关于(&A)", "关于此应用程序");

    // 添加菜单到菜单栏
    m_menuBar->Append(m_fileMenu, "文件(&F)");
    m_menuBar->Append(m_actionMenu, "操作(&A)");
    m_menuBar->Append(m_helpMenu, "帮助(&H)");

    SetMenuBar(m_menuBar);
}

void MainFrame::SetupToolBar()
{
    // 使用基类方法创建工具栏
    m_toolBar = wxFrame::CreateToolBar(wxTB_HORIZONTAL | wxTB_TEXT);

    // 添加工具栏按钮
    m_toolBar->AddTool(ID_LOGIN, "登录", wxArtProvider::GetBitmap(wxART_NORMAL_FILE), "登录到服务器");
    m_toolBar->AddTool(ID_LOGOUT, "注销", wxArtProvider::GetBitmap(wxART_CROSS_MARK), "从服务器注销");
    m_toolBar->AddSeparator();
    m_toolBar->AddTool(ID_REFRESH, "刷新", wxArtProvider::GetBitmap(wxART_REDO), "刷新文件列表");
    m_toolBar->AddSeparator();
    m_toolBar->AddTool(ID_SETTINGS, "设置", wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE), "打开设置");

    m_toolBar->Realize();
}

void MainFrame::SetupStatusBar()
{
    // 使用基类方法创建状态栏
    m_statusBar = wxFrame::CreateStatusBar(3);
    
    // 设置状态栏各部分的宽度
    int widths[] = {-1, 150, 100};
    m_statusBar->SetStatusWidths(3, widths);

    // 设置初始状态
    m_statusBar->SetStatusText("就绪", 0);
    m_statusBar->SetStatusText("未登录", 1);
    m_statusBar->SetStatusText("", 2);
}

void MainFrame::CreateMainComponents()
{
    // 创建主选项卡控件
    m_notebook = new wxNotebook(this, wxID_ANY);

    // 创建各个面板
    m_fileListPanel = new FileListPanel(m_notebook);
    m_downloadPanel = new DownloadPanel(m_notebook);
    m_membershipPanel = new MembershipPanel(m_notebook);

    // 添加选项卡页面
    m_notebook->AddPage(m_fileListPanel, "文件列表", true);
    m_notebook->AddPage(m_downloadPanel, "下载管理", false);
    m_notebook->AddPage(m_membershipPanel, "会员中心", false);

    // 创建主布局
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_notebook, 1, wxEXPAND | wxALL, 5);

    SetSizer(mainSizer);
    Layout();
}

void MainFrame::UpdateUIState()
{
    AuthManager& authManager = GetApp().GetAuthManager();
    bool isLoggedIn = authManager.IsLoggedIn();
    bool isMember = authManager.IsMember();

    // 更新菜单状态
    m_fileMenu->Enable(ID_LOGIN, !isLoggedIn);
    m_fileMenu->Enable(ID_LOGOUT, isLoggedIn);
    m_actionMenu->Enable(ID_REFRESH, isLoggedIn && isMember);

    // 更新工具栏状态
    m_toolBar->EnableTool(ID_LOGIN, !isLoggedIn);
    m_toolBar->EnableTool(ID_LOGOUT, isLoggedIn);
    m_toolBar->EnableTool(ID_REFRESH, isLoggedIn && isMember);

    // 更新状态栏
    if (isLoggedIn) {
        wxString userInfo = authManager.GetCurrentUser();
        if (isMember) {
            userInfo += " (会员)";
        } else {
            userInfo += " (普通用户)";
        }
        m_statusBar->SetStatusText(userInfo, 1);
    } else {
        m_statusBar->SetStatusText("未登录", 1);
    }

    // 更新选项卡可用性
    m_notebook->GetPage(0)->Enable(isLoggedIn && isMember); // 文件列表
    m_notebook->GetPage(1)->Enable(isLoggedIn && isMember); // 下载管理
    m_notebook->GetPage(2)->Enable(isLoggedIn);             // 会员中心
}

// 事件处理函数
void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("Seafile 客户端 v1.0\n\n基于 wxWidgets 开发的文件下载客户端",
                 "关于", wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnLogin(wxCommandEvent& WXUNUSED(event))
{
    LoginDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK) {
        UpdateUIState();
        m_statusBar->SetStatusText("登录成功", 0);
        Logger::getInstance().info("用户登录成功");
    }
}

void MainFrame::OnLogout(wxCommandEvent& WXUNUSED(event))
{
    AuthManager& authManager = GetApp().GetAuthManager();
    authManager.Logout();
    UpdateUIState();
    m_statusBar->SetStatusText("已注销", 0);
    Logger::getInstance().info("用户已注销");
}

void MainFrame::OnRefresh(wxCommandEvent& WXUNUSED(event))
{
    m_statusBar->SetStatusText("正在刷新...", 0);
    
    // 刷新文件列表
    if (m_fileListPanel) {
        m_fileListPanel->RefreshFileList();
    }
    
    m_statusBar->SetStatusText("刷新完成", 0);
    Logger::getInstance().info("文件列表已刷新");
}

void MainFrame::OnSettings(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("设置对话框尚未实现", "设置", wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnClose(wxCloseEvent& event)
{
    Logger::getInstance().info("应用程序正在关闭...");
    
    // 保存窗口状态
    // TODO: 实现窗口状态保存
    
    event.Skip();
}

