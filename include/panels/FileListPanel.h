#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>

/**
 * @brief 文件列表面板
 * 
 * 显示服务器文件列表，仅会员可见
 */
class FileListPanel : public wxPanel
{
public:
    explicit FileListPanel(wxWindow* parent);
    virtual ~FileListPanel();

    /**
     * @brief 更新界面状态
     */
    void UpdateUIState();
    
    /**
     * @brief 刷新文件列表
     */
    void RefreshFileList();
    
    /**
     * @brief 清空文件列表
     */
    void ClearFileList();

private:
    void CreateControls();
    void BindEvents();
    
    // 事件处理
    void OnItemSelected(wxListEvent& event);
    void OnItemActivated(wxListEvent& event);
    void OnRightClick(wxListEvent& event);

private:
    wxListCtrl* m_fileList;
    wxButton* m_refreshButton;
    wxStaticText* m_statusText;

    wxDECLARE_EVENT_TABLE();
};

