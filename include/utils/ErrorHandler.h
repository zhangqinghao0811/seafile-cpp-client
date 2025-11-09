/**
 * @file ErrorHandler.h
 * @brief 错误处理器定义
 * 
 * 提供统一的错误处理和异常管理功能
 */

#pragma once

#include <string>
#include <exception>

/**
 * @brief 错误处理器类
 * 
 * 单例模式实现的错误处理系统
 */
class ErrorHandler
{
public:
    /**
     * @brief 获取错误处理器实例
     * @return 错误处理器实例引用
     */
    static ErrorHandler& getInstance();

    /**
     * @brief 处理异常
     * @param e 异常对象
     */
    void handleException(const std::exception& e);

    /**
     * @brief 处理错误消息
     * @param message 错误消息
     * @param showDialog 是否显示对话框
     */
    void handleError(const std::string& message, bool showDialog = true);

private:
    ErrorHandler() = default;
    ~ErrorHandler() = default;

    // 禁用拷贝构造和赋值
    ErrorHandler(const ErrorHandler&) = delete;
    ErrorHandler& operator=(const ErrorHandler&) = delete;
};
