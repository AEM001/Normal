#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "user.h"
#include "resource.h"
#include "rental.h"
#include "bill.h"
#include "notification.h"
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cstdio>
// 数据管理器类
class DataManager {
private:
    std::string dataPath;
    std::string backupPath;
    
    // 数据文件名
    const std::string USER_FILE = "users.dat";
    const std::string RESOURCE_FILE = "resources.dat";
    const std::string RENTAL_FILE = "rentals.dat";
    const std::string BILL_FILE = "bills.dat";
    const std::string NOTIFICATION_FILE = "notifications.dat";
    const std::string CONFIG_FILE = "config.dat";
    const std::string LOG_FILE = "logs.dat";
    
    // 备份文件后缀
    const std::string BACKUP_SUFFIX = ".bak";
    
    // 创建目录
    void createDirectories() {
        std::string cmd = "mkdir -p " + dataPath;
        system(cmd.c_str());
        
        cmd = "mkdir -p " + backupPath;
        system(cmd.c_str());
    }
    
    // 记录日志
    void logOperation(const std::string& operation) {
        std::ofstream logFile(dataPath + LOG_FILE, std::ios::app);
        if (logFile.is_open()) {
            logFile << getCurrentTime() << " - " << operation << std::endl;
            logFile.close();
        }
    }
    
public:
    DataManager(const std::string& dataPath = "./data/", const std::string& backupPath = "./backup/")
        : dataPath(dataPath), backupPath(backupPath) {
        createDirectories();
    }
    
    // 保存用户数据
    template<typename T>
    bool saveUsers(const std::vector<std::shared_ptr<T>>& users) {
        try {
            std::ofstream outFile(dataPath + USER_FILE, std::ios::binary);
            if (!outFile.is_open()) {
                throw FileException("无法打开用户数据文件进行写入");
            }
            
            // 写入用户数量
            size_t count = users.size();
            outFile.write(reinterpret_cast<const char*>(&count), sizeof(count));
            
            // 写入每个用户数据
            for (const auto& user : users) {
                // 写入用户类型
                UserType type = user->getType();
                outFile.write(reinterpret_cast<const char*>(&type), sizeof(type));
                
                // 写入用户数据
                user->serialize(outFile);
            }
            
            outFile.close();
            logOperation("保存用户数据");
            return true;
        } catch (const Exception& e) {
            std::cerr << "保存用户数据失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 加载用户数据
    template<typename T>
    bool loadUsers(std::vector<std::shared_ptr<T>>& users) {
        try {
            std::ifstream inFile(dataPath + USER_FILE, std::ios::binary);
            if (!inFile.is_open()) {
                // 文件不存在，不是错误
                return true;
            }
            
            // 读取用户数量
            size_t count;
            inFile.read(reinterpret_cast<char*>(&count), sizeof(count));
            
            // 读取每个用户数据
            for (size_t i = 0; i < count; ++i) {
                // 读取用户类型
                UserType type;
                inFile.read(reinterpret_cast<char*>(&type), sizeof(type));
                
                // 根据类型创建相应的用户对象
                std::shared_ptr<User> user;
                switch (type) {
                    case STUDENT:
                        user = std::make_shared<Student>();
                        break;
                    case TEACHER:
                        user = std::make_shared<Teacher>();
                        break;
                    case ADMIN:
                        user = std::make_shared<Admin>();
                        break;
                    default:
                        throw DataException("未知的用户类型");
                }
                
                // 读取用户数据
                user->deserialize(inFile);
                users.push_back(std::static_pointer_cast<T>(user));
            }
            
            inFile.close();
            logOperation("加载用户数据");
            return true;
        } catch (const Exception& e) {
            std::cerr << "加载用户数据失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 保存资源数据
    template<typename T>
    bool saveResources(const std::vector<std::shared_ptr<T>>& resources) {
        try {
            std::ofstream outFile(dataPath + RESOURCE_FILE, std::ios::binary);
            if (!outFile.is_open()) {
                throw FileException("无法打开资源数据文件进行写入");
            }
            
            // 写入资源数量
            size_t count = resources.size();
            outFile.write(reinterpret_cast<const char*>(&count), sizeof(count));
            
            // 写入每个资源数据
            for (const auto& resource : resources) {
                // 写入资源类型
                ResourceType type = resource->getType();
                outFile.write(reinterpret_cast<const char*>(&type), sizeof(type));
                
                // 写入资源数据
                resource->serialize(outFile);
            }
            
            outFile.close();
            logOperation("保存资源数据");
            return true;
        } catch (const Exception& e) {
            std::cerr << "保存资源数据失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 加载资源数据
    template<typename T>
    bool loadResources(std::vector<std::shared_ptr<T>>& resources) {
        try {
            std::ifstream inFile(dataPath + RESOURCE_FILE, std::ios::binary);
            if (!inFile.is_open()) {
                // 文件不存在，不是错误
                return true;
            }
            
            // 读取资源数量
            size_t count;
            inFile.read(reinterpret_cast<char*>(&count), sizeof(count));
            
            // 读取每个资源数据
            for (size_t i = 0; i < count; ++i) {
                // 读取资源类型
                ResourceType type;
                inFile.read(reinterpret_cast<char*>(&type), sizeof(type));
                
                // 根据类型创建相应的资源对象
                std::shared_ptr<Resource> resource;
                switch (type) {
                    case CPU:
                        resource = std::make_shared<CPUResource>();
                        break;
                    case GPU:
                        resource = std::make_shared<GPUResource>();
                        break;
                    case STORAGE:
                        resource = std::make_shared<StorageResource>();
                        break;
                    default:
                        throw DataException("未知的资源类型");
                }
                
                // 读取资源数据
                resource->deserialize(inFile);
                resources.push_back(std::static_pointer_cast<T>(resource));
            }
            
            inFile.close();
            logOperation("加载资源数据");
            return true;
        } catch (const Exception& e) {
            std::cerr << "加载资源数据失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 保存租用记录
    bool saveRentals(const std::vector<std::shared_ptr<Rental>>& rentals) {
        try {
            std::ofstream outFile(dataPath + RENTAL_FILE, std::ios::binary);
            if (!outFile.is_open()) {
                throw FileException("无法打开租用记录文件进行写入");
            }
            
            // 写入租用记录数量
            size_t count = rentals.size();
            outFile.write(reinterpret_cast<const char*>(&count), sizeof(count));
            
            // 写入每个租用记录
            for (const auto& rental : rentals) {
                rental->serialize(outFile);
            }
            
            outFile.close();
            logOperation("保存租用记录");
            return true;
        } catch (const Exception& e) {
            std::cerr << "保存租用记录失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 加载租用记录
    bool loadRentals(std::vector<std::shared_ptr<Rental>>& rentals) {
        try {
            std::ifstream inFile(dataPath + RENTAL_FILE, std::ios::binary);
            if (!inFile.is_open()) {
                // 文件不存在，不是错误
                return true;
            }
            
            // 读取租用记录数量
            size_t count;
            inFile.read(reinterpret_cast<char*>(&count), sizeof(count));
            
            // 读取每个租用记录
            for (size_t i = 0; i < count; ++i) {
                auto rental = std::make_shared<Rental>();
                rental->deserialize(inFile);
                rentals.push_back(rental);
            }
            
            inFile.close();
            logOperation("加载租用记录");
            return true;
        } catch (const Exception& e) {
            std::cerr << "加载租用记录失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 保存账单
    bool saveBills(const std::vector<std::shared_ptr<Bill>>& bills) {
        try {
            std::ofstream outFile(dataPath + BILL_FILE, std::ios::binary);
            if (!outFile.is_open()) {
                throw FileException("无法打开账单文件进行写入");
            }
            
            // 写入账单数量
            size_t count = bills.size();
            outFile.write(reinterpret_cast<const char*>(&count), sizeof(count));
            
            // 写入每个账单
            for (const auto& bill : bills) {
                bill->serialize(outFile);
            }
            
            outFile.close();
            logOperation("保存账单");
            return true;
        } catch (const Exception& e) {
            std::cerr << "保存账单失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 加载账单
    bool loadBills(std::vector<std::shared_ptr<Bill>>& bills) {
        try {
            std::ifstream inFile(dataPath + BILL_FILE, std::ios::binary);
            if (!inFile.is_open()) {
                // 文件不存在，不是错误
                return true;
            }
            
            // 读取账单数量
            size_t count;
            inFile.read(reinterpret_cast<char*>(&count), sizeof(count));
            
            // 读取每个账单
            for (size_t i = 0; i < count; ++i) {
                auto bill = std::make_shared<Bill>();
                bill->deserialize(inFile);
                bills.push_back(bill);
            }
            
            inFile.close();
            logOperation("加载账单");
            return true;
        } catch (const Exception& e) {
            std::cerr << "加载账单失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 保存通知
    bool saveNotifications(const std::vector<std::shared_ptr<Notification>>& notifications) {
        try {
            std::ofstream outFile(dataPath + NOTIFICATION_FILE, std::ios::binary);
            if (!outFile.is_open()) {
                throw FileException("无法打开通知文件进行写入");
            }
            
            // 写入通知数量
            size_t count = notifications.size();
            outFile.write(reinterpret_cast<const char*>(&count), sizeof(count));
            
            // 写入每个通知
            for (const auto& notification : notifications) {
                notification->serialize(outFile);
            }
            
            outFile.close();
            logOperation("保存通知");
            return true;
        } catch (const Exception& e) {
            std::cerr << "保存通知失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 加载通知
    bool loadNotifications(std::vector<std::shared_ptr<Notification>>& notifications) {
        try {
            std::ifstream inFile(dataPath + NOTIFICATION_FILE, std::ios::binary);
            if (!inFile.is_open()) {
                // 文件不存在，不是错误
                return true;
            }
            
            // 读取通知数量
            size_t count;
            inFile.read(reinterpret_cast<char*>(&count), sizeof(count));
            
            // 读取每个通知
            for (size_t i = 0; i < count; ++i) {
                auto notification = std::make_shared<Notification>();
                notification->deserialize(inFile);
                notifications.push_back(notification);
            }
            
            inFile.close();
            logOperation("加载通知");
            return true;
        } catch (const Exception& e) {
            std::cerr << "加载通知失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 保存计费规则
    bool savePricingRules(const std::vector<std::shared_ptr<PricingRule>>& rules) {
        try {
            std::ofstream outFile(dataPath + CONFIG_FILE, std::ios::binary);
            if (!outFile.is_open()) {
                throw FileException("无法打开配置文件进行写入");
            }
            
            // 写入规则数量
            size_t count = rules.size();
            outFile.write(reinterpret_cast<const char*>(&count), sizeof(count));
            
            // 写入每个规则
            for (const auto& rule : rules) {
                rule->serialize(outFile);
            }
            
            outFile.close();
            logOperation("保存计费规则");
            return true;
        } catch (const Exception& e) {
            std::cerr << "保存计费规则失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 加载计费规则
    bool loadPricingRules(std::vector<std::shared_ptr<PricingRule>>& rules) {
        try {
            std::ifstream inFile(dataPath + CONFIG_FILE, std::ios::binary);
            if (!inFile.is_open()) {
                // 文件不存在，不是错误
                return true;
            }
            
            // 读取规则数量
            size_t count;
            inFile.read(reinterpret_cast<char*>(&count), sizeof(count));
            
            // 读取每个规则
            for (size_t i = 0; i < count; ++i) {
                auto rule = std::make_shared<PricingRule>();
                rule->deserialize(inFile);
                rules.push_back(rule);
            }
            
            inFile.close();
            logOperation("加载计费规则");
            return true;
        } catch (const Exception& e) {
            std::cerr << "加载计费规则失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 备份数据
    bool backupData() {
        try {
            // 获取当前时间作为备份文件名后缀
            time_t now = time(nullptr);
            struct tm* timeinfo = localtime(&now);
            char timeStr[20];
            strftime(timeStr, sizeof(timeStr), "%Y%m%d%H%M%S", timeinfo);
            
            // 备份每个数据文件
            std::string cmd;
            
            cmd = "cp " + dataPath + USER_FILE + " " + backupPath + USER_FILE + "_" + timeStr + BACKUP_SUFFIX;
            system(cmd.c_str());
            
            cmd = "cp " + dataPath + RESOURCE_FILE + " " + backupPath + RESOURCE_FILE + "_" + timeStr + BACKUP_SUFFIX;
            system(cmd.c_str());
            
            cmd = "cp " + dataPath + RENTAL_FILE + " " + backupPath + RENTAL_FILE + "_" + timeStr + BACKUP_SUFFIX;
            system(cmd.c_str());
            
            cmd = "cp " + dataPath + BILL_FILE + " " + backupPath + BILL_FILE + "_" + timeStr + BACKUP_SUFFIX;
            system(cmd.c_str());
            
            cmd = "cp " + dataPath + NOTIFICATION_FILE + " " + backupPath + NOTIFICATION_FILE + "_" + timeStr + BACKUP_SUFFIX;
            system(cmd.c_str());
            
            cmd = "cp " + dataPath + CONFIG_FILE + " " + backupPath + CONFIG_FILE + "_" + timeStr + BACKUP_SUFFIX;
            system(cmd.c_str());
            
            cmd = "cp " + dataPath + LOG_FILE + " " + backupPath + LOG_FILE + "_" + timeStr + BACKUP_SUFFIX;
            system(cmd.c_str());
            
            logOperation("备份数据");
            return true;
        } catch (const Exception& e) {
            std::cerr << "备份数据失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 恢复数据
    bool restoreData(const std::string& backupTime) {
        try {
            // 恢复每个数据文件
            std::string cmd;
            
            cmd = "cp " + backupPath + USER_FILE + "_" + backupTime + BACKUP_SUFFIX + " " + dataPath + USER_FILE;
            system(cmd.c_str());
            
            cmd = "cp " + backupPath + RESOURCE_FILE + "_" + backupTime + BACKUP_SUFFIX + " " + dataPath + RESOURCE_FILE;
            system(cmd.c_str());
            
            cmd = "cp " + backupPath + RENTAL_FILE + "_" + backupTime + BACKUP_SUFFIX + " " + dataPath + RENTAL_FILE;
            system(cmd.c_str());
            
            cmd = "cp " + backupPath + BILL_FILE + "_" + backupTime + BACKUP_SUFFIX + " " + dataPath + BILL_FILE;
            system(cmd.c_str());
            
            cmd = "cp " + backupPath + NOTIFICATION_FILE + "_" + backupTime + BACKUP_SUFFIX + " " + dataPath + NOTIFICATION_FILE;
            system(cmd.c_str());
            
            cmd = "cp " + backupPath + CONFIG_FILE + "_" + backupTime + BACKUP_SUFFIX + " " + dataPath + CONFIG_FILE;
            system(cmd.c_str());
            
            cmd = "cp " + backupPath + LOG_FILE + "_" + backupTime + BACKUP_SUFFIX + " " + dataPath + LOG_FILE;
            system(cmd.c_str());
            
            logOperation("恢复数据: " + backupTime);
            return true;
        } catch (const Exception& e) {
            std::cerr << "恢复数据失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 获取备份列表
    std::vector<std::string> getBackupList() {
        std::vector<std::string> backupList;
        
        // 使用系统命令获取备份目录中的文件列表
        std::string cmd = "ls " + backupPath + " | grep " + USER_FILE + " | cut -d'_' -f2 | cut -d'.' -f1";
        FILE* pipe = fopen(cmd.c_str(), "r");
        if (!pipe) {
            return backupList;
        }
        
        char buffer[128];
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr) {
                std::string backupTime = buffer;
                // 移除换行符
                backupTime.erase(std::remove(backupTime.begin(), backupTime.end(), '\n'), backupTime.end());
                if (!backupTime.empty()) {
                    backupList.push_back(backupTime);
                }
            }
        }
        
        fclose(pipe);
        return backupList;
    }
};

#endif // DATA_MANAGER_H
