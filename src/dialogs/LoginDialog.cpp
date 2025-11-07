#include "dialogs/LoginDialog.h"
#include "App.h"
#include "managers/AuthManager.h"
#include "utils/Logger.h"

LoginDialog::LoginDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "用户登录", wxDefaultPosition, wxSize(350, 200))
    , m_usernameCtrl(nullptr)
    , m_passwordCtrl(nullptr)
    , m_loginButton(nullptr)
    , m_cancelButton(nullptr)
    , m_rememberCheckBox(nullptr)
{
    CreateControls();
    BindEvents();
    
    // 居中显示
    Center();
}

LoginDialog::~LoginDialog()
{
}

void LoginDialog::CreateControls()
{
    wxPanel* panel = new wxPanel(this);
    
    // 创建控件
    wxStaticText* usernameLabel = new wxStaticText(panel, wxID_ANY, "用户名:");
    m_usernameCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    
    wxStaticText* passwordLabel = new wxStaticText(panel, wxID_ANY, "密码:");
    m_passwordCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1), 
                                   wxTE_PASSWORD);
    
    m_rememberCheckBox = new wxCheckBox(panel, wxID_ANY, "记住密码");
    
    m_loginButton = new wxButton(panel, wxID_OK, "登录");
    m_cancelButton = new wxButton(panel, wxID_CANCEL, "取消");
    
    // 设置默认按钮
    m_loginButton->SetDefault();
    
    // 布局
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, 2, 10, 10);
    gridSizer->Add(usernameLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_usernameCtrl, 1, wxEXPAND);
    gridSizer->Add(passwordLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_passwordCtrl, 1, wxEXPAND);
    gridSizer->AddGrowableCol(1);
    
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(m_loginButton, 0, wxRIGHT, 5);
    buttonSizer->Add(m_cancelButton, 0);
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(gridSizer, 0, wxEXPAND | wxALL, 20);
    mainSizer->Add(m_rememberCheckBox, 0, wxLEFT | wxRIGHT | wxBOTTOM, 20);
    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxLEFT | wxRIGHT | wxBOTTOM, 20);
    
    panel->SetSizer(mainSizer);
    
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    topSizer->Add(panel, 1, wxEXPAND);
    SetSizer(topSizer);
}

void LoginDialog::BindEvents()
{
    m_loginButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &LoginDialog::OnLogin, this);
    m_cancelButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &LoginDialog::OnCancel, this);
    
    // 回车键登录
    m_passwordCtrl->Bind(wxEVT_TEXT_ENTER, &LoginDialog::OnLogin, this);
}

void LoginDialog::OnLogin(wxCommandEvent& WXUNUSED(event))
{
    wxString username = m_usernameCtrl->GetValue().Trim();
    wxString password = m_passwordCtrl->GetValue();
    
    if (username.IsEmpty()) {
        wxMessageBox("请输入用户名", "错误", wxOK | wxICON_ERROR, this);
        m_usernameCtrl->SetFocus();
        return;
    }
    
    if (password.IsEmpty()) {
        wxMessageBox("请输入密码", "错误", wxOK | wxICON_ERROR, this);
        m_passwordCtrl->SetFocus();
        return;
    }
    
    // 禁用按钮，显示登录中状态
    m_loginButton->Enable(false);
    m_loginButton->SetLabel("登录中...");
    
    // 尝试登录
    AuthManager& authManager = GetApp().GetAuthManager();
    bool success = authManager.login(username.ToStdString(), password.ToStdString());
    
    if (success) {
        Logger::getInstance().info("用户登录成功: " + username.ToStdString());
        
        // TODO: 如果选择了记住密码，保存到配置
        if (m_rememberCheckBox->GetValue()) {
            // 保存用户名和加密后的密码
        }
        
        EndModal(wxID_OK);
    } else {
        Logger::getInstance().warning("用户登录失败: " + username.ToStdString());
        wxMessageBox("登录失败，请检查用户名和密码", "登录失败", wxOK | wxICON_ERROR, this);
        
        // 恢复按钮状态
        m_loginButton->Enable(true);
        m_loginButton->SetLabel("登录");
        m_passwordCtrl->SetFocus();
        m_passwordCtrl->SelectAll();
    }
}

void LoginDialog::OnCancel(wxCommandEvent& WXUNUSED(event))
{
    EndModal(wxID_CANCEL);
}

