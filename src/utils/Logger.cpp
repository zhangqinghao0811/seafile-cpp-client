#include "utils/Logger.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

Logger::~Logger()
{
    close();
}

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

bool Logger::initialize(const std::string& filename, LogLevel level)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (m_initialized) {
        close();
    }
    
    m_filename = filename;
    m_currentLevel = level;
    
    // 打开日志文件
    m_logFile.open(filename, std::ios::out | std::ios::app);
    if (!m_logFile.is_open()) {
        std::cerr << "无法打开日志文件: " << filename << std::endl;
        return false;
    }
    
    m_initialized = true;
    
    // 写入启动日志
    writeLog(LogLevel::INFO, "日志系统初始化完成");
    
    return true;
}

void Logger::setLevel(LogLevel level)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_currentLevel = level;
}

void Logger::debug(const std::string& message)
{
    log(LogLevel::DEBUG_LEVEL, message);
}

void Logger::info(const std::string& message)
{
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message)
{
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message)
{
    log(LogLevel::ERROR_LEVEL, message);
}

void Logger::log(LogLevel level, const std::string& message)
{
    if (!m_initialized || level < m_currentLevel) {
        return;
    }
    
    writeLog(level, message);
}

void Logger::flush()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_logFile.is_open()) {
        m_logFile.flush();
    }
}

void Logger::close()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_logFile.is_open()) {
        writeLog(LogLevel::INFO, "日志系统关闭");
        m_logFile.close();
    }
    m_initialized = false;
}

std::string Logger::getLevelString(LogLevel level) const
{
    switch (level) {
        case LogLevel::DEBUG_LEVEL: return "DEBUG";
        case LogLevel::INFO:        return "INFO ";
        case LogLevel::WARNING:     return "WARN ";
        case LogLevel::ERROR_LEVEL: return "ERROR";
        default:                    return "UNKN ";
    }
}

std::string Logger::getCurrentTimeString() const
{
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    return ss.str();
}

void Logger::writeLog(LogLevel level, const std::string& message)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_logFile.is_open()) {
        return;
    }
    
    // 格式: [时间] [级别] 消息
    m_logFile << "[" << getCurrentTimeString() << "] "
              << "[" << getLevelString(level) << "] "
              << message << std::endl;
    
    // 对于错误级别，同时输出到控制台
    if (level >= LogLevel::ERROR_LEVEL) {
        std::cerr << "[" << getCurrentTimeString() << "] "
                  << "[" << getLevelString(level) << "] "
                  << message << std::endl;
    }
    
    // 立即刷新缓冲区（对于重要日志）
    if (level >= LogLevel::WARNING) {
        m_logFile.flush();
    }
}
