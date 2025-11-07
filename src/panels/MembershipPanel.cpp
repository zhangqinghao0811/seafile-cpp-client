#include "panels/MembershipPanel.h"

MembershipPanel::MembershipPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
    , m_placeholderText(nullptr)
{
    CreateControls();
}

MembershipPanel::~MembershipPanel()
{
}

void MembershipPanel::CreateControls()
{
    m_placeholderText = new wxStaticText(this, wxID_ANY, "会员中心功能开发中...");
    
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->AddStretchSpacer();
    sizer->Add(m_placeholderText, 0, wxALIGN_CENTER);
    sizer->AddStretchSpacer();
    
    SetSizer(sizer);
}

void MembershipPanel::UpdateUIState()
{
    // TODO: 实现状态更新
}

