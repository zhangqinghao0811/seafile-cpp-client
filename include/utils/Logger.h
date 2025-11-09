/**
 * @file Logger.h
 * @brief 日志系统定义
 * 
 * 提供线程安全的日志记录功能，支持多个日志级别
 */

#pragma once

#include <string>
#include <fstream>
#include <mutex>

/**
 * @brief 日志级别枚举
 */
enum class LogLevel {
    DEBUG_LEVEL = 0,  ///< 调试信息
    INFO = 1,         ///< 一般信息
    WARNING = 2,      ///< 警告信息
    ERROR_LEVEL = 3   ///< 错误信息（重命名以避免 Windows ERROR 宏冲突）
};

/**
 * @brief 日志系统类
 * 
 * 单例模式实现的线程安全日志系统
 */
class Logger
{
public:
    /**
     * @brief 获取日志系统实例
     * @return 日志系统实例引用
     */
    static Logger& getInstance();

    /**
     * @brief 初始化日志系统
     * @param logFile 日志文件路径
     * @param level 日志级别
     * @return true 成功，false 失败
     */
    bool initialize(const std::string& logFile, LogLevel level = LogLevel::INFO);

    /**
     * @brief 记录调试信息
     * @param message 日志消息
     */
    void debug(const std::string& message);

    /**
     * @brief 记录一般信息
     * @param message 日志消息
     */
    void info(const std::string& message);

    /**
     * @brief 记录警告信息
     * @param message 日志消息
     */
    void warning(const std::string& message);

    /**
     * @brief 记录错误信息
     * @param message 日志消息
     */
    void error(const std::string& message);

    /**
     * @brief 设置日志级别
     * @param level 新的日志级别
     */
    void setLevel(LogLevel level);

    /**
     * @brief 刷新日志缓冲区
     */
    void flush();

    /**
     * @brief 关闭日志系统
     */
    void close();

private:
    Logger() = default;
    ~Logger();

    // 禁用拷贝构造和赋值
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    /**
     * @brief 写入日志
     * @param level 日志级别
     * @param message 日志消息
     */
    void writeLog(LogLevel level, const std::string& message);



    /**
     * @brief 获取日志级别字符串
     * @param level 日志级别
     * @return 日志级别字符串
     */
    std::string getLevelString(LogLevel level) const;

    /**
     * @brief 获取当前时间戳字符串
     * @return 格式化的时间戳字符串
     */
    std::string getCurrentTimeString() const;

private:
    /**
     * @brief 内部日志记录方法
     * @param level 日志级别
     * @param message 日志消息
     */
    void log(LogLevel level, const std::string& message);

    std::ofstream m_logFile;        ///< 日志文件流
    LogLevel m_currentLevel;        ///< 当前日志级别
    std::mutex m_mutex;             ///< 线程同步互斥锁
    bool m_initialized;             ///< 初始化标志
    std::string m_filename;         ///< 日志文件名
};
