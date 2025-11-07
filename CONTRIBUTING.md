# 贡献指南

感谢您对 Seafile C++ Client 项目的关注！我们欢迎各种形式的贡献。

## 🤝 如何贡献

### 报告问题

如果您发现了 bug 或有功能建议，请：

1. 检查 [Issues](https://github.com/zhangqinghao0811/seafile-cpp-client/issues) 确保问题未被报告
2. 创建新的 Issue，包含：
   - 清晰的标题和描述
   - 重现步骤（如果是 bug）
   - 期望的行为
   - 实际的行为
   - 系统信息（操作系统、编译器版本等）
   - 相关的日志或截图

### 提交代码

1. **Fork 仓库**
   ```bash
   git clone https://github.com/your-username/seafile-cpp-client.git
   cd seafile-cpp-client
   ```

2. **创建功能分支**
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. **进行更改**
   - 遵循项目的代码风格
   - 添加必要的测试
   - 更新相关文档

4. **提交更改**
   ```bash
   git add .
   git commit -m "feat: add your feature description"
   ```

5. **推送分支**
   ```bash
   git push origin feature/your-feature-name
   ```

6. **创建 Pull Request**
   - 提供清晰的 PR 标题和描述
   - 链接相关的 Issues
   - 确保所有检查通过

## 📝 代码风格

### C++ 代码规范

- 使用 4 个空格缩进
- 类名使用 PascalCase：`AuthManager`
- 函数名使用 camelCase：`getUserInfo()`
- 变量名使用 camelCase：`userName`
- 成员变量使用 `m_` 前缀：`m_userName`
- 常量使用全大写：`MAX_RETRY_COUNT`

### 文件组织

- 头文件放在 `include/` 目录
- 源文件放在 `src/` 目录
- 保持头文件和源文件的目录结构一致

### 注释规范

使用 Doxygen 风格的注释：

```cpp
/**
 * @brief 用户认证管理器
 * 
 * 负责处理用户登录、登出和会话管理
 */
class AuthManager {
public:
    /**
     * @brief 用户登录
     * @param username 用户名
     * @param password 密码
     * @return true 登录成功，false 登录失败
     */
    bool login(const std::string& username, const std::string& password);
};
```

## 🧪 测试

### 运行测试

```bash
# 构建时启用测试
cmake .. -DBUILD_TESTS=ON
make -j
ctest --output-on-failure
```

### 添加测试

- 单元测试放在 `tests/unit/` 目录
- 集成测试放在 `tests/integration/` 目录
- 使用 Google Test 框架

## 📚 文档

### 更新文档

- API 变更需要更新相关的头文件注释
- 新功能需要更新 README.md
- 重大变更需要更新 docs/ 目录下的文档

### 文档风格

- 使用 Markdown 格式
- 中英文混排时注意空格
- 代码块指定语言类型

## 🔄 开发流程

### 分支策略

- `main/master` - 稳定版本
- `develop` - 开发版本
- `feature/*` - 功能分支
- `bugfix/*` - 修复分支
- `hotfix/*` - 紧急修复

### 提交信息格式

使用 [Conventional Commits](https://www.conventionalcommits.org/) 格式：

```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

类型：
- `feat`: 新功能
- `fix`: 修复 bug
- `docs`: 文档更新
- `style`: 代码格式化
- `refactor`: 重构
- `test`: 测试相关
- `chore`: 构建过程或辅助工具的变动

示例：
```
feat(auth): add remember password functionality

- Add checkbox to login dialog
- Store encrypted password in config
- Auto-fill login form on startup

Closes #123
```

## 🚀 发布流程

### 版本号规则

使用 [Semantic Versioning](https://semver.org/)：

- `MAJOR.MINOR.PATCH`
- MAJOR: 不兼容的 API 变更
- MINOR: 向后兼容的功能性新增
- PATCH: 向后兼容的问题修正

### 发布检查清单

- [ ] 所有测试通过
- [ ] 文档已更新
- [ ] CHANGELOG.md 已更新
- [ ] 版本号已更新
- [ ] 创建 Git tag
- [ ] 构建发布包

## 🛠️ 开发环境设置

### 推荐工具

- **IDE**: Visual Studio Code, CLion, Visual Studio
- **调试器**: GDB, LLDB, Visual Studio Debugger
- **静态分析**: clang-tidy, cppcheck
- **格式化**: clang-format

### VS Code 配置

创建 `.vscode/settings.json`：

```json
{
    "cmake.configureArgs": [
        "-DCMAKE_BUILD_TYPE=Debug",
        "-DBUILD_TESTS=ON"
    ],
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
    "files.associations": {
        "*.h": "cpp",
        "*.cpp": "cpp"
    }
}
```

## 📋 代码审查

### 审查要点

- 代码逻辑正确性
- 性能考虑
- 内存管理
- 异常处理
- 线程安全
- 代码风格一致性
- 测试覆盖率

### 审查流程

1. 自动化检查（CI/CD）
2. 同行代码审查
3. 维护者最终审查
4. 合并到主分支

## 🎯 优先级指南

### 高优先级

- 安全漏洞修复
- 崩溃问题修复
- 数据丢失问题
- 核心功能 bug

### 中优先级

- 性能优化
- 用户体验改进
- 新功能开发
- 代码重构

### 低优先级

- 代码风格调整
- 文档完善
- 测试补充
- 工具改进

## 📞 联系方式

如有任何问题，请通过以下方式联系：

- GitHub Issues: [项目 Issues](https://github.com/zhangqinghao0811/seafile-cpp-client/issues)
- 邮箱: zhangqinghao0811@163.com

感谢您的贡献！🎉

