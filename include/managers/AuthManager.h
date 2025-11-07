#pragma once

#include <string>
#include <memory>

class ConfigManager;

/**
 * @brief 认证管理器类
 * 
 * 负责用户认证、会员状态管理、Token 存储等功能
 */
class AuthManager {
public:
    explicit AuthManager(ConfigManager& configManager);
    ~AuthManager();

    /**
     * @brief 用户登录
     * @param username 用户名
     * @param password 密码
     * @return true 登录成功，false 登录失败
     */
    bool login(const std::string& username, const std::string& password);
    
    /**
     * @brief 用户登出
     */
    void logout();
    
    /**
     * @brief 检查是否已登录
     * @return true 已登录，false 未登录
     */
    bool isLoggedIn() const;
    
    /**
     * @brief 检查是否为会员
     * @return true 是会员，false 不是会员
     */
    bool isMember() const;
    
    /**
     * @brief 获取当前用户名
     * @return std::string 用户名
     */
    std::string getCurrentUsername() const;
    
    /**
     * @brief 获取认证Token
     * @return std::string Token字符串
     */
    std::string getAuthToken() const;
    
    /**
     * @brief 恢复会话（从本地存储）
     * @return true 恢复成功，false 恢复失败
     */
    bool restoreSession();
    
    /**
     * @brief 保存会话到本地存储
     */
    void saveSession();

private:
    ConfigManager& m_configManager;
    std::string m_username;
    std::string m_authToken;
    bool m_isLoggedIn;
    bool m_isMember;
};

