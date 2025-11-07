#pragma once

#include <wx/wx.h>

/**
 * @brief 登录对话框
 */
class LoginDialog : public wxDialog
{
public:
    explicit LoginDialog(wxWindow* parent);
    virtual ~LoginDialog();

private:
    void CreateControls();
    void BindEvents();
    
    void OnLogin(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

private:
    wxTextCtrl* m_usernameCtrl;
    wxTextCtrl* m_passwordCtrl;
    wxButton* m_loginButton;
    wxButton* m_cancelButton;
    wxCheckBox* m_rememberCheckBox;
};

