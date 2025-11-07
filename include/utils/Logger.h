#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <memory>

/**
 * @brief 日志级别枚举
 */
enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3
};

/**
 * @brief 日志管理器类（单例模式）
 * 
 * 提供线程安全的日志记录功能，支持不同日志级别和文件输出
 */
class Logger {
public:
    /**
     * @brief 获取Logger单例实例
     * @return Logger& Logger实例引用
     */
    static Logger& getInstance();
    
    /**
     * @brief 初始化日志系统
     * @param filename 日志文件名
     * @param level 最低日志级别
     * @return true 初始化成功，false 初始化失败
     */
    bool initialize(const std::string& filename, LogLevel level = LogLevel::INFO);
    
    /**
     * @brief 设置日志级别
     * @param level 日志级别
     */
    void setLevel(LogLevel level);
    
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
     * @brief 记录指定级别的日志
     * @param level 日志级别
     * @param message 日志消息
     */
    void log(LogLevel level, const std::string& message);
    
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
    
    // 禁用拷贝构造和赋值操作
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    /**
     * @brief 获取日志级别字符串
     * @param level 日志级别
     * @return std::string 级别字符串
     */
    std::string getLevelString(LogLevel level) const;
    
    /**
     * @brief 获取当前时间字符串
     * @return std::string 时间字符串
     */
    std::string getCurrentTimeString() const;
    
    /**
     * @brief 写入日志到文件
     * @param level 日志级别
     * @param message 日志消息
     */
    void writeLog(LogLevel level, const std::string& message);

private:
    std::ofstream m_logFile;        ///< 日志文件流
    LogLevel m_currentLevel;        ///< 当前日志级别
    std::mutex m_mutex;             ///< 线程同步互斥锁
    bool m_initialized;             ///< 初始化标志
    std::string m_filename;         ///< 日志文件名
};

