#pragma once

#include <wx/wx.h>

/**
 * @brief 会员中心面板
 */
class MembershipPanel : public wxPanel
{
public:
    explicit MembershipPanel(wxWindow* parent);
    virtual ~MembershipPanel();

    void UpdateUIState();

private:
    void CreateControls();

private:
    wxStaticText* m_placeholderText;
};

