#include "panels/FileListPanel.h"
#include "App.h"
#include "managers/AuthManager.h"
#include "utils/Logger.h"

wxBEGIN_EVENT_TABLE(FileListPanel, wxPanel)
    EVT_LIST_ITEM_SELECTED(wxID_ANY, FileListPanel::OnItemSelected)
    EVT_LIST_ITEM_ACTIVATED(wxID_ANY, FileListPanel::OnItemActivated)
    EVT_LIST_ITEM_RIGHT_CLICK(wxID_ANY, FileListPanel::OnRightClick)
wxEND_EVENT_TABLE()

FileListPanel::FileListPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
    , m_fileList(nullptr)
    , m_refreshButton(nullptr)
    , m_statusText(nullptr)
{
    CreateControls();
    BindEvents();
    UpdateUIState();
}

FileListPanel::~FileListPanel()
{
}

void FileListPanel::CreateControls()
{
    // 创建文件列表控件
    m_fileList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                               wxLC_REPORT | wxLC_SINGLE_SEL);
    
    // 添加列
    m_fileList->AppendColumn("文件名", wxLIST_FORMAT_LEFT, 300);
    m_fileList->AppendColumn("大小", wxLIST_FORMAT_RIGHT, 100);
    m_fileList->AppendColumn("修改时间", wxLIST_FORMAT_LEFT, 150);
    m_fileList->AppendColumn("类型", wxLIST_FORMAT_LEFT, 100);
    
    // 创建按钮
    m_refreshButton = new wxButton(this, wxID_ANY, "刷新列表");
    
    // 创建状态文本
    m_statusText = new wxStaticText(this, wxID_ANY, "请先登录并成为会员");
    
    // 布局
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(m_refreshButton, 0, wxALL, 5);
    buttonSizer->AddStretchSpacer();
    buttonSizer->Add(m_statusText, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    
    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(m_fileList, 1, wxEXPAND | wxALL, 5);
    
    SetSizer(mainSizer);
}

void FileListPanel::BindEvents()
{
    m_refreshButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent&) {
        RefreshFileList();
    });
}

void FileListPanel::UpdateUIState()
{
    AuthManager& authManager = GetApp().GetAuthManager();
    bool canAccess = authManager.isLoggedIn() && authManager.isMember();
    
    m_refreshButton->Enable(canAccess);
    m_fileList->Enable(canAccess);
    
    if (!authManager.isLoggedIn()) {
        m_statusText->SetLabel("请先登录");
    } else if (!authManager.isMember()) {
        m_statusText->SetLabel("请购买会员以访问文件");
    } else {
        m_statusText->SetLabel("就绪");
    }
}

void FileListPanel::RefreshFileList()
{
    Logger::getInstance().info("刷新文件列表");
    
    AuthManager& authManager = GetApp().GetAuthManager();
    if (!authManager.isLoggedIn() || !authManager.isMember()) {
        wxMessageBox("请先登录并成为会员", "提示", wxOK | wxICON_INFORMATION);
        return;
    }
    
    // TODO: 实现实际的文件列表获取逻辑
    // 这里是占位符实现
    ClearFileList();
    
    // 添加示例数据
    long index = m_fileList->InsertItem(0, "示例文件1.txt");
    m_fileList->SetItem(index, 1, "1.2 KB");
    m_fileList->SetItem(index, 2, "2024-01-01 12:00:00");
    m_fileList->SetItem(index, 3, "文本文件");
    
    index = m_fileList->InsertItem(1, "示例文件2.pdf");
    m_fileList->SetItem(index, 1, "2.5 MB");
    m_fileList->SetItem(index, 2, "2024-01-02 15:30:00");
    m_fileList->SetItem(index, 3, "PDF文档");
    
    m_statusText->SetLabel("文件列表已更新");
}

void FileListPanel::ClearFileList()
{
    m_fileList->DeleteAllItems();
}

void FileListPanel::OnItemSelected(wxListEvent& event)
{
    long selectedIndex = event.GetIndex();
    wxString filename = m_fileList->GetItemText(selectedIndex);
    Logger::getInstance().info("选中文件: " + filename.ToStdString());
}

void FileListPanel::OnItemActivated(wxListEvent& event)
{
    long selectedIndex = event.GetIndex();
    wxString filename = m_fileList->GetItemText(selectedIndex);
    
    // TODO: 实现文件下载逻辑
    wxMessageBox("开始下载文件: " + filename, "下载", wxOK | wxICON_INFORMATION);
}

void FileListPanel::OnRightClick(wxListEvent& event)
{
    // TODO: 实现右键菜单
    wxMenu menu;
    menu.Append(wxID_ANY, "下载");
    menu.Append(wxID_ANY, "查看详情");
    
    PopupMenu(&menu);
}

