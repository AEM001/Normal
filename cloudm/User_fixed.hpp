#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <stdexcept>
#include <memory>

// 用户角色枚举
enum class UserRole {
    STUDENT,
    TEACHER,
    ADMIN
};

// 用户状态枚举
enum class UserStatus {
    ACTIVE,
    SUSPENDED
};

// 枚举转字符串函数
inline const char* UserRoleToString(UserRole role) {
    switch(role) {
        case UserRole::STUDENT: return "学生";
        case UserRole::TEACHER: return "教师";
        case UserRole::ADMIN: return "管理员";
        default: return "未知角色";
    }
}

inline const char* UserStatusToString(UserStatus status) {
    switch(status) {
        case UserStatus::ACTIVE: return "激活";
        case UserStatus::SUSPENDED: return "暂停";
        default: return "未知状态";
    }
}

/**
 * @class User
 * @brief 表示系统中的用户。
 *
 * 包含用户的基本信息和账户操作。
 */
class User {
private:
    std::string userId;
    std::string username;
    std::string passwordHash; // 存储密码哈希而非明文
    UserRole role;
    UserStatus status;
    double accountBalance;

public:
    // 构造函数
    User(std::string id, std::string name, std::string pwdHash, UserRole r, 
         UserStatus stat = UserStatus::ACTIVE, double balance = 0.0)
        : userId(id), username(name), passwordHash(pwdHash), role(r), 
          status(stat), accountBalance(balance) {}
    
    // 为反序列化添加默认构造函数
    User() : userId(""), username(""), passwordHash(""), role(UserRole::STUDENT), 
             status(UserStatus::ACTIVE), accountBalance(0.0) {}
    
    // 获取器
    std::string getUserId() const { return userId; }
    std::string getUsername() const { return username; }
    UserRole getRole() const { return role; }
    UserStatus getStatus() const { return status; }
    double getAccountBalance() const { return accountBalance; }
    
    // 设置器
    void setUsername(const std::string& name) { username = name; }
    void setPasswordHash(const std::string& hash) { passwordHash = hash; }
    void setRole(UserRole r) { role = r; }
    void setStatus(UserStatus stat) { status = stat; }
    
    // 验证密码
    bool verifyPassword(const std::string& password) const {
        // 简单实现，实际应使用安全的哈希比较
        return passwordHash == password;
    }
    
    // 账户操作
    void deposit(double amount) {
        if (amount <= 0) {
            throw std::invalid_argument("充值金额必须为正数");
        }
        accountBalance += amount;
    }
    
    bool withdraw(double amount) {
        if (amount <= 0) {
            throw std::invalid_argument("提取金额必须为正数");
        }
        if (accountBalance < amount) {
            return false; // 余额不足
        }
        accountBalance -= amount;
        return true;
    }
    
    // 序列化/反序列化方法（用于数据持久化）
    void serialize(std::ostream& os) const {
        os.write(userId.c_str(), userId.size() + 1);
        os.write(username.c_str(), username.size() + 1);
        os.write(passwordHash.c_str(), passwordHash.size() + 1);
        os.write(reinterpret_cast<const char*>(&role), sizeof(UserRole));
        os.write(reinterpret_cast<const char*>(&status), sizeof(UserStatus));
        os.write(reinterpret_cast<const char*>(&accountBalance), sizeof(double));
    }
    
    void deserialize(std::istream& is) {
        char buffer[256];
        is.getline(buffer, 256, '\0');
        userId = buffer;
        
        is.getline(buffer, 256, '\0');
        username = buffer;
        
        is.getline(buffer, 256, '\0');
        passwordHash = buffer;
        
        is.read(reinterpret_cast<char*>(&role), sizeof(UserRole));
        is.read(reinterpret_cast<char*>(&status), sizeof(UserStatus));
        is.read(reinterpret_cast<char*>(&accountBalance), sizeof(double));
    }
};

/**
 * @class UserCollection
 * @brief 管理系统中所有用户的集合。
 *
 * 提供添加、查找、列出用户的功能。
 */
class UserCollection {
private:
    std::vector<User*> users;

public:
    // 析构函数释放所有用户对象
    ~UserCollection() {
        for (auto user : users) {
            delete user;
        }
        users.clear();
    }

    // 添加用户到集合
    bool addUser(User* user) {
        // 检查用户ID和用户名是否已存在
        for (auto u : users) {
            if (u->getUserId() == user->getUserId() || u->getUsername() == user->getUsername()) {
                return false; // 用户ID或用户名已存在
            }
        }
        users.push_back(user);
        return true;
    }

    // 根据ID查找用户
    User* findUserById(const std::string& id) const {
        for (auto user : users) {
            if (user->getUserId() == id) {
                return user;
            }
        }
        return nullptr; // 未找到用户
    }

    // 根据用户名查找用户
    User* findUserByUsername(const std::string& username) const {
        for (auto user : users) {
            if (user->getUsername() == username) {
                return user;
            }
        }
        return nullptr; // 未找到用户
    }

    // 获取所有用户
    const std::vector<User*>& getAllUsers() const {
        return users;
    }

    // 持久化方法
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行写入: " + filename);
        }
        
        // 写入用户数量
        size_t count = users.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));
        
        // 逐个写入用户
        for (auto user : users) {
            user->serialize(file);
        }
        
        file.close();
    }
    
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行读取: " + filename);
        }
        
        // 清空当前用户
        for (auto user : users) {
            delete user;
        }
        users.clear();
        
        // 读取用户数量
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
        
        // 逐个读取用户
        for (size_t i = 0; i < count; ++i) {
            User* user = new User();
            user->deserialize(file);
            users.push_back(user);
        }
        
        file.close();
    }
};

// 创建预设用户集合的辅助函数
inline UserCollection createDefaultUserCollection() {
    UserCollection collection;
    
    // 添加管理员用户
    collection.addUser(new User("U001", "admin", "admin123", UserRole::ADMIN));
    
    // 添加教师用户
    collection.addUser(new User("U002", "teacher1", "teacher123", UserRole::TEACHER, UserStatus::ACTIVE, 1000.0));
    collection.addUser(new User("U003", "teacher2", "teacher123", UserRole::TEACHER, UserStatus::ACTIVE, 1000.0));
    
    // 添加学生用户
    collection.addUser(new User("U004", "student1", "student123", UserRole::STUDENT, UserStatus::ACTIVE, 500.0));
    collection.addUser(new User("U005", "student2", "student123", UserRole::STUDENT, UserStatus::ACTIVE, 500.0));
    
    return collection;
}

/**
 * @class UserManager
 * @brief 管理系统中的所有用户。
 *
 * 负责用户的注册、登录、查找和数据持久化。
 */
class UserManager {
private:
    std::vector<User*> users;
    std::string dataFilePath; // 用户数据文件路径
    int nextUserId;

public:
    UserManager(const std::string& filePath) : dataFilePath(filePath), nextUserId(6) {}
    
    ~UserManager() { 
        saveUsers(); 
        for (auto user : users) {
            delete user;
        }
        users.clear();
    }

    // 加载用户数据
    void loadUsers() {
        try {
            std::ifstream file(dataFilePath, std::ios::binary);
            if (!file) {
                std::cerr << "警告: 无法打开用户数据文件，将创建新文件。\n";
                // 创建默认用户
                users = createDefaultUserCollection().getAllUsers();
                saveUsers(); // 立即保存默认用户
                return;
            }

            // 清空当前用户
            for (auto user : users) {
                delete user;
            }
            users.clear();

            // 读取下一个可用的用户ID
            file.read(reinterpret_cast<char*>(&nextUserId), sizeof(int));

            // 读取用户数量
            size_t count;
            file.read(reinterpret_cast<char*>(&count), sizeof(size_t));

            // 逐个读取用户
            for (size_t i = 0; i < count; ++i) {
                User* user = new User();
                user->deserialize(file);
                users.push_back(user);
            }

            file.close();
        } catch (const std::exception& e) {
            std::cerr << "加载用户数据时发生错误: " << e.what() << "\n";
            // 创建默认用户
            users = createDefaultUserCollection().getAllUsers();
            saveUsers(); // 立即保存默认用户
        }
    }

    // 保存用户数据
    void saveUsers() const {
        try {
            // 确保data目录存在
            #ifdef _WIN32
                _mkdir("data");
            #else
                mkdir("data", 0777);
            #endif
            
            std::ofstream file(dataFilePath, std::ios::binary);
            if (!file) {
                throw std::runtime_error("无法打开文件进行写入: " + dataFilePath);
            }

            // 写入下一个可用的用户ID
            file.write(reinterpret_cast<const char*>(&nextUserId), sizeof(int));

            // 写入用户数量
            size_t count = users.size();
            file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));

            // 逐个写入用户
            for (auto user : users) {
                user->serialize(file);
            }

            file.close();
        } catch (const std::exception& e) {
            std::cerr << "保存用户数据时发生错误: " << e.what() << "\n";
        }
    }

    // 注册新用户
    bool registerUser(const std::string& username, const std::string& password, UserRole role) {
        // 检查用户名是否已存在
        for (auto user : users) {
            if (user->getUsername() == username) {
                return false; // 用户名已存在
            }
        }

        // 生成用户ID
        char idStr[5];
        sprintf(idStr, "U%03d", nextUserId++);
        std::string userId = idStr;

        // 创建新用户（简单实现，实际应哈希密码）
        User* newUser = new User(userId, username, password, role);
        users.push_back(newUser);

        return true;
    }

    // 用户登录
    User* loginUser(const std::string& username, const std::string& password) {
        for (auto user : users) {
            if (user->getUsername() == username && user->verifyPassword(password)) {
                return user;
            }
        }
        return nullptr; // 登录失败
    }

    // 根据ID查找用户
    User* findUserById(const std::string& id) const {
        for (auto user : users) {
            if (user->getUserId() == id) {
                return user;
            }
        }
        return nullptr; // 未找到用户
    }

    // 根据用户名查找用户
    User* findUserByUsername(const std::string& username) const {
        for (auto user : users) {
            if (user->getUsername() == username) {
                return user;
            }
        }
        return nullptr; // 未找到用户
    }

    // 获取所有用户
    const std::vector<User*>& getAllUsers() const {
        return users;
    }
};

#endif // USER_HPP
