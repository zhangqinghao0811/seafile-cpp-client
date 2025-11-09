#include "managers/ConfigManager.h"
#include "utils/Logger.h"
#include <fstream>
#include <sstream>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#elif defined(__APPLE__)
#include <CoreServices/CoreServices.h>
#else
#include <unistd.h>
#include <pwd.h>
#endif

ConfigManager::ConfigManager()
{
    m_configFilePath = getConfigFilePath();
    Logger::getInstance().info("配置管理器初始化，配置文件路径: " + m_configFilePath);
}

ConfigManager::~ConfigManager()
{
    Logger::getInstance().info("配置管理器销毁");
}

bool ConfigManager::loadConfig(const std::string& filename)
{
    std::string configFile = filename.empty() ? m_configFilePath : filename;
    
    Logger::getInstance().info("加载配置文件: " + configFile);
    
    std::ifstream file(configFile);
    if (!file.is_open()) {
        Logger::getInstance().warning("无法打开配置文件: " + configFile);
        return false;
    }
    
    std::string line;
    std::string currentSection;
    
    while (std::getline(file, line)) {
        // 去除首尾空白字符
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        // 跳过空行和注释
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }
        
        // 处理节标题 [section]
        if (line[0] == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }
        
        // 处理键值对 key=value
        size_t equalPos = line.find('=');
        if (equalPos != std::string::npos) {
            std::string key = line.substr(0, equalPos);
            std::string value = line.substr(equalPos + 1);
            
            // 去除键值的空白字符
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            
            std::string fullKey = getFullKey(currentSection, key);
            m_config[fullKey] = value;
        }
    }
    
    Logger::getInstance().info("配置文件加载完成，共加载 " + std::to_string(m_config.size()) + " 个配置项");
    return true;
}

bool ConfigManager::saveConfig(const std::string& filename)
{
    std::string configFile = filename.empty() ? m_configFilePath : filename;
    
    Logger::getInstance().info("保存配置文件: " + configFile);
    
    // 确保目录存在
    std::filesystem::path configPath(configFile);
    std::filesystem::create_directories(configPath.parent_path());
    
    std::ofstream file(configFile);
    if (!file.is_open()) {
        Logger::getInstance().error("无法创建配置文件: " + configFile);
        return false;
    }
    
    // 按节分组写入配置
    std::map<std::string, std::map<std::string, std::string>> sections;
    
    for (const auto& pair : m_config) {
        size_t dotPos = pair.first.find('.');
        if (dotPos != std::string::npos) {
            std::string section = pair.first.substr(0, dotPos);
            std::string key = pair.first.substr(dotPos + 1);
            sections[section][key] = pair.second;
        }
    }
    
    // 写入文件
    file << "# Seafile Client Configuration File\n";
    file << "# Generated automatically, do not edit manually\n\n";
    
    for (const auto& section : sections) {
        file << "[" << section.first << "]\n";
        for (const auto& keyValue : section.second) {
            file << keyValue.first << "=" << keyValue.second << "\n";
        }
        file << "\n";
    }
    
    Logger::getInstance().info("配置文件保存完成");
    return true;
}

void ConfigManager::createDefaultConfig()
{
    Logger::getInstance().info("创建默认配置");
    
    // 服务器配置
    setString("server", "base_url", "https://your-server.com");
    setString("server", "api_endpoint", "/api/v1");
    
    // 客户端配置
    setBool("client", "auto_login", false);
    setBool("client", "remember_password", false);
    setString("client", "download_directory", "~/Downloads/Seafile");
    
    // 网络配置
    setInt("network", "timeout", 30);
    setInt("network", "retry_count", 3);
    setInt("network", "max_concurrent_downloads", 3);
    
    // 界面配置
    setString("ui", "language", "zh_CN");
    setString("ui", "theme", "default");
    
    // 保存默认配置
    saveConfig();
}

std::string ConfigManager::getString(const std::string& section, const std::string& key, 
                                   const std::string& defaultValue) const
{
    std::string fullKey = getFullKey(section, key);
    auto it = m_config.find(fullKey);
    return (it != m_config.end()) ? it->second : defaultValue;
}

void ConfigManager::setString(const std::string& section, const std::string& key, 
                            const std::string& value)
{
    std::string fullKey = getFullKey(section, key);
    m_config[fullKey] = value;
}

int ConfigManager::getInt(const std::string& section, const std::string& key, 
                        int defaultValue) const
{
    std::string value = getString(section, key);
    if (value.empty()) {
        return defaultValue;
    }
    
    try {
        return std::stoi(value);
    } catch (const std::exception&) {
        return defaultValue;
    }
}

void ConfigManager::setInt(const std::string& section, const std::string& key, int value)
{
    setString(section, key, std::to_string(value));
}

bool ConfigManager::getBool(const std::string& section, const std::string& key, 
                          bool defaultValue) const
{
    std::string value = getString(section, key);
    if (value.empty()) {
        return defaultValue;
    }
    
    // 支持多种布尔值表示
    if (value == "true" || value == "1" || value == "yes" || value == "on") {
        return true;
    } else if (value == "false" || value == "0" || value == "no" || value == "off") {
        return false;
    }
    
    return defaultValue;
}

void ConfigManager::setBool(const std::string& section, const std::string& key, bool value)
{
    setString(section, key, value ? "true" : "false");
}

std::string ConfigManager::getConfigFilePath() const
{
    return getAppDataDir() + "/client.conf";
}

std::string ConfigManager::getFullKey(const std::string& section, const std::string& key) const
{
    return section + "." + key;
}

std::string ConfigManager::getAppDataDir() const
{
    std::string appDataDir;
    
#ifdef _WIN32
    // Windows: %APPDATA%/SeafileClient
    char* appData = nullptr;
    size_t len = 0;
    if (_dupenv_s(&appData, &len, "APPDATA") == 0 && appData != nullptr) {
        appDataDir = std::string(appData) + "/SeafileClient";
        free(appData);
    } else {
        appDataDir = "./config";
    }
#elif defined(__APPLE__)
    // macOS: ~/Library/Application Support/SeafileClient
    const char* home = getenv("HOME");
    if (home) {
        appDataDir = std::string(home) + "/Library/Application Support/SeafileClient";
    } else {
        appDataDir = "./config";
    }
#else
    // Linux: ~/.config/SeafileClient
    const char* configHome = getenv("XDG_CONFIG_HOME");
    if (configHome) {
        appDataDir = std::string(configHome) + "/SeafileClient";
    } else {
        const char* home = getenv("HOME");
        if (home) {
            appDataDir = std::string(home) + "/.config/SeafileClient";
        } else {
            appDataDir = "./config";
        }
    }
#endif
    
    return appDataDir;
}

std::string ConfigManager::getLogPath() const
{
    return getAppDataDir() + "/seafile-client.log";
}

std::string ConfigManager::getServerUrl() const
{
    return getString("server", "base_url", "");
}

bool ConfigManager::initialize()
{
    // 尝试加载现有配置
    if (!loadConfig()) {
        // 如果加载失败，创建默认配置
        createDefaultConfig();
    }
    return true;
}
