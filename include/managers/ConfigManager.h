#pragma once

#include <string>
#include <map>

/**
 * @brief 配置管理器类
 * 
 * 负责应用程序配置的加载、保存和管理
 */
class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager();

    /**
     * @brief 加载配置文件
     * @param filename 配置文件名（可选，使用默认路径）
     * @return true 加载成功，false 加载失败
     */
    bool loadConfig(const std::string& filename = "");
    
    /**
     * @brief 保存配置文件
     * @param filename 配置文件名（可选，使用默认路径）
     * @return true 保存成功，false 保存失败
     */
    bool saveConfig(const std::string& filename = "");
    
    /**
     * @brief 创建默认配置
     */
    void createDefaultConfig();
    
    /**
     * @brief 获取字符串配置值
     * @param section 配置节
     * @param key 配置键
     * @param defaultValue 默认值
     * @return std::string 配置值
     */
    std::string getString(const std::string& section, const std::string& key, 
                         const std::string& defaultValue = "") const;
    
    /**
     * @brief 设置字符串配置值
     * @param section 配置节
     * @param key 配置键
     * @param value 配置值
     */
    void setString(const std::string& section, const std::string& key, 
                   const std::string& value);
    
    /**
     * @brief 获取整数配置值
     * @param section 配置节
     * @param key 配置键
     * @param defaultValue 默认值
     * @return int 配置值
     */
    int getInt(const std::string& section, const std::string& key, 
               int defaultValue = 0) const;
    
    /**
     * @brief 设置整数配置值
     * @param section 配置节
     * @param key 配置键
     * @param value 配置值
     */
    void setInt(const std::string& section, const std::string& key, int value);
    
    /**
     * @brief 获取布尔配置值
     * @param section 配置节
     * @param key 配置键
     * @param defaultValue 默认值
     * @return bool 配置值
     */
    bool getBool(const std::string& section, const std::string& key, 
                 bool defaultValue = false) const;
    
    /**
     * @brief 设置布尔配置值
     * @param section 配置节
     * @param key 配置键
     * @param value 配置值
     */
    void setBool(const std::string& section, const std::string& key, bool value);
    
    /**
     * @brief 获取配置文件路径
     * @return std::string 配置文件完整路径
     */
    std::string getConfigFilePath() const;

private:
    /**
     * @brief 获取配置键的完整名称
     * @param section 配置节
     * @param key 配置键
     * @return std::string 完整键名
     */
    std::string getFullKey(const std::string& section, const std::string& key) const;
    
    /**
     * @brief 获取应用程序数据目录
     * @return std::string 数据目录路径
     */
    std::string getAppDataDir() const;

private:
    std::map<std::string, std::string> m_config;  ///< 配置数据存储
    std::string m_configFilePath;                 ///< 配置文件路径
};

