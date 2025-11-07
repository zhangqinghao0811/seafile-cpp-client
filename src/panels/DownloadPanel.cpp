#include "panels/DownloadPanel.h"

DownloadPanel::DownloadPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
    , m_placeholderText(nullptr)
{
    CreateControls();
}

DownloadPanel::~DownloadPanel()
{
}

void DownloadPanel::CreateControls()
{
    m_placeholderText = new wxStaticText(this, wxID_ANY, "下载管理功能开发中...");
    
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->AddStretchSpacer();
    sizer->Add(m_placeholderText, 0, wxALIGN_CENTER);
    sizer->AddStretchSpacer();
    
    SetSizer(sizer);
}

void DownloadPanel::UpdateUIState()
{
    // TODO: 实现状态更新
}

