/**
 * @file ErrorHandler.cpp
 * @brief 错误处理器实现
 */

#include "utils/ErrorHandler.h"
#include "utils/Logger.h"
#include <wx/wx.h>

ErrorHandler& ErrorHandler::getInstance()
{
    static ErrorHandler instance;
    return instance;
}

void ErrorHandler::handleException(const std::exception& e)
{
    std::string message = "发生异常: ";
    message += e.what();
    
    // 记录到日志
    Logger::getInstance().error(message);
    
    // 显示错误对话框
    wxMessageBox(wxString::FromUTF8(message.c_str()), 
                 "错误", 
                 wxOK | wxICON_ERROR);
}

void ErrorHandler::handleError(const std::string& message, bool showDialog)
{
    // 记录到日志
    Logger::getInstance().error(message);
    
    // 显示错误对话框（如果需要）
    if (showDialog) {
        wxMessageBox(wxString::FromUTF8(message.c_str()), 
                     "错误", 
                     wxOK | wxICON_ERROR);
    }
}
