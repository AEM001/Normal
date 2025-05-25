#ifndef USER_HPP
#define USER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include "Resource.hpp"

// 用户角色枚举
enum class UserRole {
    STUDENT,
    TEACHER,
    ADMIN
};

// 用户状态枚举
enum class UserStatus {
    ACTIVE,
    SUSPENDED // 因违规被管理员暂停使用
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
        case UserStatus::ACTIVE: return "正常";
        case UserStatus::SUSPENDED: return "暂停";
        default: return "未知状态";
    }
}

/**
 * @class User
 * @brief 系统中所有用户的基类。
 *
 * 存储用户通用信息，如ID、用户名、密码、角色和状态。
 * 提供基本功能如登录、个人资料管理和余额查询。
 */
class User {
protected:
    std::string userId;
    std::string username;
    std::string password; 
    double accountBalance;
    UserStatus status;
    UserRole role;

public:
    // 构造函数
    User(std::string id, std::string name, std::string pwd, UserRole r, double balance = 0.0, UserStatus stat = UserStatus::ACTIVE)
        : userId(id), username(name), password(pwd), role(r), accountBalance(balance), status(stat) {}
    
    // 为反序列化添加默认构造函数
    User() : userId(""), username(""), password(""), role(UserRole::STUDENT), accountBalance(0.0), status(UserStatus::ACTIVE) {}

    virtual ~User() = default;

    // 获取器
    std::string getUserId() const { return userId; }
    std::string getUsername() const { return username; }
    virtual UserRole getRole() const { return role; }
    double getAccountBalance() const { return accountBalance; }
    UserStatus getStatus() const { return status; }

    // 设置器
    void setUsername(const std::string& newName) { username = newName; }
    void setPassword(const std::string& newPassword) { password = newPassword; }
    void setStatus(UserStatus newStatus) { status = newStatus; }

    // 账户余额管理
    virtual void deposit(double amount) = 0;
    virtual bool withdraw(double amount) = 0; // 成功返回true

    // 身份验证
    bool verifyPassword(const std::string& pwd) const { return pwd == password; }

    // 显示用户特定菜单/信息的纯虚函数
    virtual void displayDashboard() const = 0;

    // 序列化/反序列化方法（用于数据持久化）
    virtual void serialize(std::ostream& os) const {
        // 写入基本属性
        os.write(userId.c_str(), userId.size() + 1);
        os.write(username.c_str(), username.size() + 1);
        os.write(password.c_str(), password.size() + 1);
        os.write(reinterpret_cast<const char*>(&accountBalance), sizeof(double));
        os.write(reinterpret_cast<const char*>(&status), sizeof(UserStatus));
    }
    
    virtual void deserialize(std::istream& is) {
        // 读取基本属性
        char buffer[256];
        is.getline(buffer, 256, '\0');
        userId = buffer;
        
        is.getline(buffer, 256, '\0');
        username = buffer;
        
        is.getline(buffer, 256, '\0');
        password = buffer;
        
        is.read(reinterpret_cast<char*>(&accountBalance), sizeof(double));
        is.read(reinterpret_cast<char*>(&status), sizeof(UserStatus));
    }
};

/**
 * @class Student
 * @brief 表示学生用户。
 * 
 * 继承自User。具有特定的配额或优先级。
 */
class Student : public User {
public:
    Student(std::string id, std::string name, std::string password)
         : User(id, name, password, UserRole::STUDENT) {} 

    // 空构造函数用于反序列化
    Student() : User("", "", "", UserRole::STUDENT) {}
    
    UserRole getRole() const override { return UserRole::STUDENT; }
    
    void deposit(double amount) override {
        if (amount <= 0) {
            throw std::invalid_argument("充值金额必须为正数");
        }
        accountBalance += amount;
    }
    
    bool withdraw(double amount) override {
        if (amount <= 0) {
            throw std::invalid_argument("提取金额必须为正数");
        }
        if (amount > accountBalance) {
            return false; // 余额不足
        }
        accountBalance -= amount;
        return true;
    }
    
    void displayDashboard() const override {
        std::cout << "===== 学生控制面板 =====\n";
        std::cout << "ID: " << userId << "\n";
        std::cout << "用户名: " << username << "\n";
        std::cout << "当前余额: " << accountBalance << "\n";
        std::cout << "状态: " << (status == UserStatus::ACTIVE ? "活跃" : "已暂停") << "\n";
        std::cout << "可用功能:\n";
        std::cout << "1. 浏览资源\n";
        std::cout << "2. 租用资源\n";
        std::cout << "3. 查看租赁历史\n";
        std::cout << "4. 查看账户余额\n";
        std::cout << "5. 退出\n";
    }

    // 序列化/反序列化方法
    void serialize(std::ostream& os) const override {
        User::serialize(os);
    }
    
    void deserialize(std::istream& is) override {
        User::deserialize(is);
    }
};

/**
 * @class Teacher
 * @brief 表示教师用户。
 * 
 * 继承自User。可能具有更高的资源分配优先级。
 */
class Teacher : public User {
public:
    Teacher(std::string id, std::string name, std::string password, double balance = 0.0)
        : User(id, name, password, UserRole::TEACHER, balance) {}
    
    // 空构造函数用于反序列化
    Teacher() : User("", "", "", UserRole::TEACHER) {}
    
    UserRole getRole() const override { return UserRole::TEACHER; }
    
    void deposit(double amount) override {
        if (amount <= 0) {
            throw std::invalid_argument("充值金额必须为正数");
        }
        accountBalance += amount;
    }
    
    bool withdraw(double amount) override {
        if (amount <= 0) {
            throw std::invalid_argument("提取金额必须为正数");
        }
        if (amount > accountBalance) {
            return false; // 余额不足
        }
        accountBalance -= amount;
        return true;
    }
    
    void displayDashboard() const override {
        std::cout << "===== 教师控制面板 =====\n";
        std::cout << "ID: " << userId << "\n";
        std::cout << "用户名: " << username << "\n";
        std::cout << "当前余额: " << accountBalance << "\n";
        std::cout << "状态: " << (status == UserStatus::ACTIVE ? "活跃" : "已暂停") << "\n";
        std::cout << "可用功能:\n"; 
        std::cout << "1. 浏览资源\n";
        std::cout << "2. 租用资源\n";
        std::cout << "3. 查看租赁历史\n";
        std::cout << "4. 查看账户余额\n";
        std::cout << "5. 退出\n";
    }

    // 序列化/反序列化方法
    void serialize(std::ostream& os) const override {
        User::serialize(os);
    }
    
    void deserialize(std::istream& is) override {
        User::deserialize(is);
    }
};

/**
 * @class Admin
 * @brief 表示系统管理员。
 *
 * 继承自User。具有管理用户、资源和审核租赁请求的权限。
 * 管理员账户是预设的或由其他管理员添加的。
 */
class Admin : public User {
public:
    Admin(std::string id, std::string name, std::string password)
        : User(id, name, password, UserRole::ADMIN) {}
    
    // 空构造函数用于反序列化
    Admin() : User("", "", "", UserRole::ADMIN) {}
    
    UserRole getRole() const override { return UserRole::ADMIN; }
    
    void displayDashboard() const override {
        std::cout << "===== 管理员控制面板 =====\n";
        std::cout << "ID: " << userId << "\n";
        std::cout << "用户名: " << username << "\n";
        std::cout << "当前余额: " << accountBalance << "\n";
        std::cout << "状态: " << (status == UserStatus::ACTIVE ? "活跃" : "已暂停") << "\n";
        std::cout << "可用功能:\n";
        std::cout << "1. 管理用户\n";
        std::cout << "2. 管理资源\n";
        std::cout << "3. 管理租赁请求\n";
        std::cout << "4. 设置计费标准\n";
        std::cout << "5. 查看所有租赁记录\n";
        std::cout << "6. 查看所有账单\n";
    }

    // 管理员特定功能
    void manageUser(User& user, UserStatus newStatus) {
        user.setStatus(newStatus);
        std::cout << "用户 " << user.getUsername() << " 状态已更新为 " 
                  << (newStatus == UserStatus::ACTIVE ? "活跃" : "已暂停") << std::endl;
    }
    
    // 资源管理功能
    void addResource(ResourceCollection& collection, Resource* resource) {
        collection.addResource(resource);
        std::cout << "已添加新资源: " << resource->getResourceName() << " (ID: " << resource->getResourceId() << ")" << std::endl;
        
        // 保存更新后的资源集合到文件
        try {
            collection.saveToFile("data/resources.dat");
            std::cout << "资源数据已保存到文件" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "保存资源数据失败: " << e.what() << std::endl;
        }
    }
    
    void modifyResource(ResourceCollection& collection, const std::string& resourceId, 
                        const std::string& newName, double newRate) {
        Resource* resource = collection.findResourceById(resourceId);
        if (resource) {
            resource->setResourceName(newName);
            resource->setHourlyRate(newRate);
            std::cout << "资源 " << resourceId << " 已更新" << std::endl;
            
            // 保存更新后的资源集合到文件
            try {
                collection.saveToFile("data/resources.dat");
                std::cout << "资源数据已保存到文件" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "保存资源数据失败: " << e.what() << std::endl;
            }
        } else {
            std::cout << "未找到资源 " << resourceId << std::endl;
        }
    }
    
    void deleteResource(ResourceCollection& collection, const std::string& resourceId) {
        std::vector<Resource*>& resources = const_cast<std::vector<Resource*>&>(collection.getAllResources());
        for (auto it = resources.begin(); it != resources.end(); ++it) {
            if ((*it)->getResourceId() == resourceId) {
                delete *it;
                resources.erase(it);
                std::cout << "资源 " << resourceId << " 已删除" << std::endl;
                
                // 保存更新后的资源集合到文件
                try {
                    collection.saveToFile("data/resources.dat");
                    std::cout << "资源数据已保存到文件" << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "保存资源数据失败: " << e.what() << std::endl;
                }
                return;
            }
        }
        std::cout << "未找到资源 " << resourceId << std::endl;
    }
    
    void loadResourceData(ResourceCollection& collection) {
        try {
            collection.loadFromFile("data/resources.dat");
            std::cout << "已从文件加载资源数据" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "加载资源数据失败: " << e.what() << std::endl;
        }
    }
    
    void setBillingRate(ResourceCollection& collection, ResourceType type, double newRate) {
        std::vector<Resource*> resources = collection.getResourcesByType(type);
        for (auto resource : resources) {
            resource->setHourlyRate(newRate);
        }
        std::cout << "已更新所有 " << (type == ResourceType::CPU ? "CPU" : "GPU") 
                  << " 资源的计费标准为 " << newRate << " 元/小时" << std::endl;
        
        // 保存更新后的资源集合到文件
        try {
            collection.saveToFile("data/resources.dat");
            std::cout << "资源数据已保存到文件" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "保存资源数据失败: " << e.what() << std::endl;
        }
    }
    
    // 租赁请求管理功能
    void approveRentalRequest(const std::string& requestId) {
        // 实现租赁请求批准逻辑
        std::cout << "已批准租赁请求 " << requestId << std::endl;
    }
    
    void rejectRentalRequest(const std::string& requestId) {
        // 实现租赁请求拒绝逻辑
        std::cout << "已拒绝租赁请求 " << requestId << std::endl;
    }

    // 实现纯虚函数
    void deposit(double amount) override {
        if (amount <= 0) {
            throw std::invalid_argument("充值金额必须为正数");
        }
        accountBalance += amount;
        std::cout << "管理员账户充值 " << amount << " 元成功。当前余额: " << accountBalance << " 元。" << std::endl;
    }

    bool withdraw(double amount) override {
        if (amount <= 0) {
            throw std::invalid_argument("提取金额必须为正数");
        }
        if (amount > accountBalance) {
            std::cout << "提现失败，余额不足。" << std::endl;
            return false;
        }
        accountBalance -= amount;
        std::cout << "管理员账户提现 " << amount << " 元成功。当前余额: " << accountBalance << " 元。" << std::endl;
        return true;
    }
};

/**
 * @class UserCollection
 * @brief 管理系统中所有用户的集合。
 *
 * 提供添加、查找、列出用户的功能。
 * 可以按角色筛选用户。
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
    void addUser(User* user) {
        users.push_back(user);
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

    // 获取特定角色的用户
    std::vector<User*> getUsersByRole(UserRole role) const {
        std::vector<User*> result;
        for (auto user : users) {
            if (user->getRole() == role) {
                result.push_back(user);
            }
        }
        return result;
    }

    // 显示所有用户
    void displayAllUsers() const {
        std::cout << "===== 所有用户列表 =====\n";
        for (auto user : users) {
            std::cout << "ID: " << user->getUserId() << "\n";
            std::cout << "用户名: " << user->getUsername() << "\n";
            std::cout << "角色: " << UserRoleToString(user->getRole()) << "\n";
            std::cout << "状态: " << UserStatusToString(user->getStatus()) << "\n";
            std::cout << "余额: " << user->getAccountBalance() << "\n";
            std::cout << "------------------------\n";
        }
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
            // 写入用户角色标识
            UserRole role = user->getRole();
            file.write(reinterpret_cast<const char*>(&role), sizeof(UserRole));
            
            // 根据角色序列化用户
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
            // 读取用户角色标识
            UserRole role;
            file.read(reinterpret_cast<char*>(&role), sizeof(UserRole));
            
            // 根据角色创建相应的用户对象
            User* user = nullptr;
            switch (role) {
                case UserRole::STUDENT:
                    user = new Student();
                    break;
                case UserRole::TEACHER:
                    user = new Teacher();
                    break;
                case UserRole::ADMIN:
                    user = new Admin();
                    break;
                default:
                    throw std::runtime_error("未知的用户角色");
            }
            
            // 反序列化用户
            user->deserialize(file);
            
            // 添加到集合
            users.push_back(user);
        }
        
        file.close();
    }
};

// 创建预设用户集合的辅助函数
inline UserCollection createDefaultUserCollection() {
    UserCollection collection;
    
    // 添加管理员用户
    collection.addUser(new Admin("admin001", "系统管理员", "admin123"));
    
    // 添加教师用户
    collection.addUser(new Teacher("teacher001", "张教授", "teacher123", 1000.0));
    collection.addUser(new Teacher("teacher002", "李教授", "teacher123", 1000.0));
    collection.addUser(new Teacher("teacher003", "王教授", "teacher123", 1000.0));
    
    // 添加学生用户
    collection.addUser(new Student("student001", "张三", "student123"));
    collection.addUser(new Student("student002", "李四", "student123"));
    collection.addUser(new Student("student003", "王五", "student123"));
    collection.addUser(new Student("student004", "赵六", "student123"));
    collection.addUser(new Student("student005", "钱七", "student123"));
    
    return collection;
}

/**
 * @class UserManager
 * @brief 管理系统中的所有用户。
 *
 * 负责用户的注册、登录、查找、修改和数据持久化。
 */
class UserManager {
private:
    std::vector<User*> users;
    std::string dataFilePath; // 用户数据文件路径
    int nextUserId;

public:
    UserManager(const std::string& filePath) : dataFilePath(filePath), nextUserId(1) {}
    
    ~UserManager() { 
        saveUsers(); 
        for (auto user : users) {
            delete user;
        }
        users.clear();
    }

    // 加载用户数据
    void loadUsers() {
        std::ifstream file(dataFilePath, std::ios::binary);
        if (!file) {
            std::cerr << "警告: 无法打开用户数据文件，将创建新文件。\n";
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
            // 读取用户角色标识
            UserRole role;
            file.read(reinterpret_cast<char*>(&role), sizeof(UserRole));

            // 根据角色创建相应的用户对象
            User* user = nullptr;
            switch (role) {
                case UserRole::STUDENT:
                    user = new Student();
                    break;
                case UserRole::TEACHER:
                    user = new Teacher();
                    break;
                case UserRole::ADMIN:
                    user = new Admin();
                    break;
                default:
                    throw std::runtime_error("未知的用户角色");
            }

            // 反序列化用户
            user->deserialize(file);

            // 添加到集合
            users.push_back(user);
        }

        file.close();
    }

    // 保存用户数据
    void saveUsers() const {
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
            // 写入用户角色标识
            UserRole role = user->getRole();
            file.write(reinterpret_cast<const char*>(&role), sizeof(UserRole));

            // 序列化用户
            user->serialize(file);
        }

        file.close();
    }

    // 用户注册
    bool registerUser(const std::string& username, const std::string& password, UserRole role) {
        // 检查用户名是否已存在
        for (auto user : users) {
            if (user->getUsername() == username) {
                return false; // 用户名已存在
            }
        }

        // 生成用户ID
        std::string userId = generateUniqueUserId();

        // 创建新用户
        User* newUser = nullptr;
        switch (role) {
            case UserRole::STUDENT:
                newUser = new Student(userId, username, password);
                break;
            case UserRole::TEACHER:
                newUser = new Teacher(userId, username, password);
                break;
            case UserRole::ADMIN:
                newUser = new Admin(userId, username, password);
                break;
            default:
                return false; // 无效的角色
        }

        // 添加用户
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
    User* findUserById(const std::string& userId) {
        for (auto user : users) {
            if (user->getUserId() == userId) {
                return user;
            }
        }
        return nullptr; // 未找到用户
    }

    const User* findUserById(const std::string& userId) const {
        for (auto user : users) {
            if (user->getUserId() == userId) {
                return user;
            }
        }
        return nullptr; // 未找到用户
    }

    // 根据用户名查找用户
    User* findUserByUsername(const std::string& username) {
        for (auto user : users) {
            if (user->getUsername() == username) {
                return user;
            }
        }
        return nullptr; // 未找到用户
    }

    const User* findUserByUsername(const std::string& username) const {
        for (auto user : users) {
            if (user->getUsername() == username) {
                return user;
            }
        }
        return nullptr; // 未找到用户
    }

    // 管理员操作：修改用户状态
    bool setUserStatus(const std::string& userId, UserStatus status) {
        User* user = findUserById(userId);
        if (user) {
            user->setStatus(status);
            return true;
        }
        return false;
    }

    // 获取所有用户（用于管理员查看）
    const std::vector<User*>& getAllUsers() const { return users; }

private:
    // 生成唯一的用户ID
    std::string generateUniqueUserId() {
        std::string prefix;
        switch (nextUserId % 3) {
            case 0: prefix = "STU"; break;
            case 1: prefix = "TCH"; break;
            case 2: prefix = "ADM"; break;
        }
        return prefix + std::to_string(nextUserId++);
    }
};

#endif // USER_HPP
