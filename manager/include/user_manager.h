#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "user.h"
#include "exception.h"
#include "data_manager.h"
#include <vector>
#include <memory>
#include <algorithm>

// 用户管理器类
class UserManager {
private:
    std::vector<std::shared_ptr<User>> users;
    std::shared_ptr<DataManager> dataManager;
    std::shared_ptr<User> currentUser;
    
    // 查找用户
    std::shared_ptr<User> findUserByUsername(const std::string& username) {
        for (const auto& user : users) {
            if (user->getUsername() == username) {
                return user;
            }
        }
        return nullptr;
    }
    
    std::shared_ptr<User> findUserById(const std::string& id) {
        for (const auto& user : users) {
            if (user->getId() == id) {
                return user;
            }
        }
        return nullptr;
    }
    
public:
    UserManager(std::shared_ptr<DataManager> dataManager) 
        : dataManager(dataManager), currentUser(nullptr) {
        // 加载用户数据
        dataManager->loadUsers<User>(users);
        
        // 如果没有管理员，创建默认管理员
        bool hasAdmin = false;
        for (const auto& user : users) {
            if (user->getType() == ADMIN) {
                hasAdmin = true;
                break;
            }
        }
        
        if (!hasAdmin) {
            auto admin = std::make_shared<Admin>("admin", "admin123", "系统管理员", 1);
            users.push_back(admin);
            saveUsers();
        }
    }
    
    ~UserManager() {
        saveUsers();
    }
    
    // 保存用户数据
    bool saveUsers() {
        return dataManager->saveUsers<User>(users);
    }
    
    // 注册用户
    bool registerUser(const std::string& username, const std::string& password, 
                      const std::string& name, UserType type,
                      const std::string& idNumber, const std::string& specialInfo) {
        try {
            // 检查用户名是否已存在
            if (findUserByUsername(username) != nullptr) {
                throw UserException("用户名已存在");
            }
            
            // 创建用户
            std::shared_ptr<User> user;
            switch (type) {
                case STUDENT: {
                    auto student = std::make_shared<Student>(username, password, name, idNumber, specialInfo);
                    user = student;
                    break;
                }
                case TEACHER: {
                    auto teacher = std::make_shared<Teacher>(username, password, name, idNumber, specialInfo);
                    user = teacher;
                    break;
                }
                case ADMIN: {
                    // 只有管理员可以创建管理员
                    if (currentUser == nullptr || currentUser->getType() != ADMIN) {
                        throw UserException("没有权限创建管理员账号");
                    }
                    auto admin = std::make_shared<Admin>(username, password, name, std::stoi(specialInfo));
                    user = admin;
                    break;
                }
                default:
                    throw UserException("无效的用户类型");
            }
            
            users.push_back(user);
            saveUsers();
            return true;
        } catch (const Exception& e) {
            std::cerr << "注册用户失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 用户登录
    bool login(const std::string& username, const std::string& password) {
        try {
            auto user = findUserByUsername(username);
            if (user == nullptr) {
                throw UserException("用户名不存在");
            }
            
            if (!user->checkPassword(password)) {
                throw UserException("密码错误");
            }
            
            if (!user->isActive()) {
                throw UserException("账号已被暂停");
            }
            
            currentUser = user;
            return true;
        } catch (const Exception& e) {
            std::cerr << "登录失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 用户登出
    void logout() {
        currentUser = nullptr;
    }
    
    // 修改密码
    bool changePassword(const std::string& oldPassword, const std::string& newPassword) {
        try {
            if (currentUser == nullptr) {
                throw UserException("未登录");
            }
            
            if (!currentUser->checkPassword(oldPassword)) {
                throw UserException("原密码错误");
            }
            
            currentUser->setPassword(newPassword);
            saveUsers();
            return true;
        } catch (const Exception& e) {
            std::cerr << "修改密码失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 修改用户信息
    bool updateUserInfo(const std::string& name) {
        try {
            if (currentUser == nullptr) {
                throw UserException("未登录");
            }
            
            currentUser->setName(name);
            saveUsers();
            return true;
        } catch (const Exception& e) {
            std::cerr << "修改用户信息失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 充值余额
    bool addBalance(double amount) {
        try {
            if (currentUser == nullptr) {
                throw UserException("未登录");
            }
            
            if (amount <= 0) {
                throw UserException("充值金额必须大于0");
            }
            
            currentUser->addBalance(amount);
            saveUsers();
            return true;
        } catch (const Exception& e) {
            std::cerr << "充值失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 扣除余额
    bool deductBalance(double amount) {
        try {
            if (currentUser == nullptr) {
                throw UserException("未登录");
            }
            
            if (amount <= 0) {
                throw UserException("扣除金额必须大于0");
            }
            
            if (!currentUser->deductBalance(amount)) {
                throw UserException("余额不足");
            }
            
            saveUsers();
            return true;
        } catch (const Exception& e) {
            std::cerr << "扣费失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 获取当前用户
    std::shared_ptr<User> getCurrentUser() const {
        return currentUser;
    }
    
    // 获取所有用户
    std::vector<std::shared_ptr<User>> getAllUsers() const {
        return users;
    }
    
    // 获取用户数量
    size_t getUserCount() const {
        return users.size();
    }
    
    // 管理员功能：修改用户状态
    bool updateUserStatus(const std::string& userId, UserStatus status) {
        try {
            if (currentUser == nullptr || currentUser->getType() != ADMIN) {
                throw UserException("没有权限修改用户状态");
            }
            
            auto user = findUserById(userId);
            if (user == nullptr) {
                throw UserException("用户不存在");
            }
            
            user->setStatus(status);
            saveUsers();
            return true;
        } catch (const Exception& e) {
            std::cerr << "修改用户状态失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 管理员功能：删除用户
    bool deleteUser(const std::string& userId) {
        try {
            if (currentUser == nullptr || currentUser->getType() != ADMIN) {
                throw UserException("没有权限删除用户");
            }
            
            auto it = std::find_if(users.begin(), users.end(), 
                                  [&userId](const std::shared_ptr<User>& user) {
                                      return user->getId() == userId;
                                  });
            
            if (it == users.end()) {
                throw UserException("用户不存在");
            }
            
            users.erase(it);
            saveUsers();
            return true;
        } catch (const Exception& e) {
            std::cerr << "删除用户失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 管理员功能：查找用户
    std::shared_ptr<User> findUser(const std::string& username) {
        try {
            if (currentUser == nullptr || currentUser->getType() != ADMIN) {
                throw UserException("没有权限查找用户");
            }
            
            return findUserByUsername(username);
        } catch (const Exception& e) {
            std::cerr << "查找用户失败: " << e.what() << std::endl;
            return nullptr;
        }
    }
};

#endif // USER_MANAGER_H
