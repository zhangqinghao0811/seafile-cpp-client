#pragma once

#include <wx/wx.h>

/**
 * @brief 下载管理面板
 */
class DownloadPanel : public wxPanel
{
public:
    explicit DownloadPanel(wxWindow* parent);
    virtual ~DownloadPanel();

    void UpdateUIState();

private:
    void CreateControls();

private:
    wxStaticText* m_placeholderText;
};

