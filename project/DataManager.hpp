#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include "User.hpp"
#include "Resource.hpp"
#include "Rental.hpp"
#include "Notification.hpp"
#include "Billing.hpp"

/**
 * @class DataManager
 * @brief 管理系统中所有数据的持久化。
 *
 * 负责用户、资源、租赁记录、通知和账单等数据的加载和保存。
 * 提供数据备份和恢复功能。
 */
class DataManager {
private:
    // 数据文件路径
    std::string usersFilePath;
    std::string resourcesFilePath;
    std::string rentalsFilePath;
    std::string notificationsFilePath;
    std::string billsFilePath;
    std::string billingRulesFilePath;
    std::string logFilePath;
    
    // 数据集合
    UserCollection userCollection;
    ResourceCollection resourceCollection;
    std::vector<RentalRecord> rentalRecords;
    std::vector<Notification> notifications;
    std::vector<Bill> bills;
    std::vector<BillingRule> billingRules;
    
    // // 创建数据目录
    // void createDataDirectory() {
    //     std::filesystem::path dataDir = "data";
    //     if (!std::filesystem::exists(dataDir)) {
    //         std::filesystem::create_directory(dataDir);
    //     }
    // }
    
    // 日志记录
    void logMessage(const std::string& message) {
        std::ofstream logFile(logFilePath, std::ios::app);
        if (logFile) {
            auto now = std::chrono::system_clock::now();
            auto nowTime = std::chrono::system_clock::to_time_t(now);
            logFile << std::ctime(&nowTime) << " - " << message << std::endl;
            logFile.close();
        }
    }

public:
    // 构造函数
    DataManager() 
        : usersFilePath("data/users.dat"),
          resourcesFilePath("data/resources.dat"),
          rentalsFilePath("data/rentals.dat"),
          notificationsFilePath("data/notifications.dat"),
          billsFilePath("data/bills.dat"),
          billingRulesFilePath("data/billing_rules.dat"),
          logFilePath("data/system.log") {
        // createDataDirectory();
    }
    
    // 加载所有数据
    void loadAllData() {
        try {
            loadUsers();
            loadResources();
            loadRentals();
            loadNotifications();
            loadBills();
            loadBillingRules();
            logMessage("所有数据加载成功");
        } catch (const std::exception& e) {
            std::cerr << "加载数据时发生错误: " << e.what() << std::endl;
            logMessage("加载数据失败: " + std::string(e.what()));
        }
    }
    
    // 保存所有数据
    void saveAllData() {
        try {
            saveUsers();
            saveResources();
            saveRentals();
            saveNotifications();
            saveBills();
            saveBillingRules();
            logMessage("所有数据保存成功");
        } catch (const std::exception& e) {
            std::cerr << "保存数据时发生错误: " << e.what() << std::endl;
            logMessage("保存数据失败: " + std::string(e.what()));
        }
    }
    
    // 用户数据操作
    void loadUsers() {
        try {
            std::ifstream file(usersFilePath, std::ios::binary);
            if (!file) {
                // 如果文件不存在，创建默认用户
                userCollection = createDefaultUserCollection();
                saveUsers();
                logMessage("创建并保存默认用户数据");
                return;
            }
            
            userCollection.loadFromFile(usersFilePath);
            logMessage("用户数据加载成功");
        } catch (const std::exception& e) {
            std::cerr << "加载用户数据时发生错误: " << e.what() << std::endl;
            logMessage("加载用户数据失败: " + std::string(e.what()));
            // 如果加载失败，创建默认用户
            userCollection = createDefaultUserCollection();
        }
    }
    
    void saveUsers() {
        try {
            userCollection.saveToFile(usersFilePath);
            logMessage("用户数据保存成功");
        } catch (const std::exception& e) {
            std::cerr << "保存用户数据时发生错误: " << e.what() << std::endl;
            logMessage("保存用户数据失败: " + std::string(e.what()));
        }
    }
    
    // 资源数据操作
    void loadResources() {
        try {
            std::ifstream file(resourcesFilePath, std::ios::binary);
            if (!file) {
                // 如果文件不存在，创建默认资源
                resourceCollection = createDefaultResourceCollection();
                saveResources();
                logMessage("创建并保存默认资源数据");
                return;
            }
            
            resourceCollection.loadFromFile(resourcesFilePath);
            logMessage("资源数据加载成功");
        } catch (const std::exception& e) {
            std::cerr << "加载资源数据时发生错误: " << e.what() << std::endl;
            logMessage("加载资源数据失败: " + std::string(e.what()));
            // 如果加载失败，创建默认资源
            resourceCollection = createDefaultResourceCollection();
        }
    }
    
    void saveResources() {
        try {
            resourceCollection.saveToFile(resourcesFilePath);
            logMessage("资源数据保存成功");
        } catch (const std::exception& e) {
            std::cerr << "保存资源数据时发生错误: " << e.what() << std::endl;
            logMessage("保存资源数据失败: " + std::string(e.what()));
        }
    }
    
    // 租赁记录操作
    void loadRentals() {
        try {
            std::ifstream file(rentalsFilePath, std::ios::binary);
            if (!file) {
                logMessage("租赁数据文件不存在，将创建新文件");
                return;
            }
            
            // 读取租赁记录数量
            size_t count;
            file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
            
            // 清空当前租赁记录
            rentalRecords.clear();
            
            // 逐个读取租赁记录
            for (size_t i = 0; i < count; ++i) {
                RentalRecord record;
                record.deserialize(file);
                rentalRecords.push_back(record);
            }
            
            logMessage("租赁数据加载成功");
        } catch (const std::exception& e) {
            std::cerr << "加载租赁数据时发生错误: " << e.what() << std::endl;
            logMessage("加载租赁数据失败: " + std::string(e.what()));
        }
    }
    
    void saveRentals() {
        try {
            std::ofstream file(rentalsFilePath, std::ios::binary);
            if (!file) {
                throw std::runtime_error("无法打开文件进行写入: " + rentalsFilePath);
            }
            
            // 写入租赁记录数量
            size_t count = rentalRecords.size();
            file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));
            
            // 逐个写入租赁记录
            for (const auto& record : rentalRecords) {
                record.serialize(file);
            }
            
            logMessage("租赁数据保存成功");
        } catch (const std::exception& e) {
            std::cerr << "保存租赁数据时发生错误: " << e.what() << std::endl;
            logMessage("保存租赁数据失败: " + std::string(e.what()));
        }
    }
    
    // 通知操作
    void loadNotifications() {
        try {
            std::ifstream file(notificationsFilePath, std::ios::binary);
            if (!file) {
                logMessage("通知数据文件不存在，将创建新文件");
                return;
            }
            
            // 读取通知数量
            size_t count;
            file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
            
            // 清空当前通知
            notifications.clear();
            
            // 逐个读取通知
            for (size_t i = 0; i < count; ++i) {
                Notification notification;
                notification.deserialize(file);
                notifications.push_back(notification);
            }
            
            logMessage("通知数据加载成功");
        } catch (const std::exception& e) {
            std::cerr << "加载通知数据时发生错误: " << e.what() << std::endl;
            logMessage("加载通知数据失败: " + std::string(e.what()));
        }
    }
    
    void saveNotifications() {
        try {
            std::ofstream file(notificationsFilePath, std::ios::binary);
            if (!file) {
                throw std::runtime_error("无法打开文件进行写入: " + notificationsFilePath);
            }
            
            // 写入通知数量
            size_t count = notifications.size();
            file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));
            
            // 逐个写入通知
            for (const auto& notification : notifications) {
                notification.serialize(file);
            }
            
            logMessage("通知数据保存成功");
        } catch (const std::exception& e) {
            std::cerr << "保存通知数据时发生错误: " << e.what() << std::endl;
            logMessage("保存通知数据失败: " + std::string(e.what()));
        }
    }
    
    // 账单操作
    void loadBills() {
        try {
            std::ifstream file(billsFilePath, std::ios::binary);
            if (!file) {
                logMessage("账单数据文件不存在，将创建新文件");
                return;
            }
            
            // 读取账单数量
            size_t count;
            file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
            
            // 清空当前账单
            bills.clear();
            
            // 逐个读取账单
            for (size_t i = 0; i < count; ++i) {
                Bill bill;
                bill.deserialize(file);
                bills.push_back(bill);
            }
            
            logMessage("账单数据加载成功");
        } catch (const std::exception& e) {
            std::cerr << "加载账单数据时发生错误: " << e.what() << std::endl;
            logMessage("加载账单数据失败: " + std::string(e.what()));
        }
    }
    
    void saveBills() {
        try {
            std::ofstream file(billsFilePath, std::ios::binary);
            if (!file) {
                throw std::runtime_error("无法打开文件进行写入: " + billsFilePath);
            }
            
            // 写入账单数量
            size_t count = bills.size();
            file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));
            
            // 逐个写入账单
            for (const auto& bill : bills) {
                bill.serialize(file);
            }
            
            logMessage("账单数据保存成功");
        } catch (const std::exception& e) {
            std::cerr << "保存账单数据时发生错误: " << e.what() << std::endl;
            logMessage("保存账单数据失败: " + std::string(e.what()));
        }
    }
    
    // 计费规则操作
    void loadBillingRules() {
        try {
            std::ifstream file(billingRulesFilePath, std::ios::binary);
            if (!file) {
                // 如果文件不存在，创建默认计费规则
                createDefaultBillingRules();
                saveBillingRules();
                logMessage("创建并保存默认计费规则");
                return;
            }
            
            // 读取规则数量
            size_t count;
            file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
            
            // 清空当前规则
            billingRules.clear();
            
            // 逐个读取规则
            for (size_t i = 0; i < count; ++i) {
                BillingRule rule;
                rule.deserialize(file);
                billingRules.push_back(rule);
            }
            
            logMessage("计费规则加载成功");
        } catch (const std::exception& e) {
            std::cerr << "加载计费规则时发生错误: " << e.what() << std::endl;
            logMessage("加载计费规则失败: " + std::string(e.what()));
            // 如果加载失败，创建默认规则
            createDefaultBillingRules();
        }
    }
    
    void saveBillingRules() {
        try {
            std::ofstream file(billingRulesFilePath, std::ios::binary);
            if (!file) {
                throw std::runtime_error("无法打开文件进行写入: " + billingRulesFilePath);
            }
            
            // 写入规则数量
            size_t count = billingRules.size();
            file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));
            
            // 逐个写入规则
            for (const auto& rule : billingRules) {
                rule.serialize(file);
            }
            
            logMessage("计费规则保存成功");
        } catch (const std::exception& e) {
            std::cerr << "保存计费规则时发生错误: " << e.what() << std::endl;
            logMessage("保存计费规则失败: " + std::string(e.what()));
        }
    }
    
    // 创建默认计费规则
    void createDefaultBillingRules() {
        billingRules.clear();
        billingRules.emplace_back("RULE_CPU", ResourceType::CPU, 10.0, 0.9, 0.8, 1.0, 360.0);
        billingRules.emplace_back("RULE_GPU", ResourceType::GPU, 20.0, 0.9, 0.8, 1.0, 360.0);
    }
    
    // 获取数据集合
    UserCollection& getUserCollection() { return userCollection; }
    ResourceCollection& getResourceCollection() { return resourceCollection; }
    std::vector<RentalRecord>& getRentalRecords() { return rentalRecords; }
    std::vector<Notification>& getNotifications() { return notifications; }
    std::vector<Bill>& getBills() { return bills; }
    std::vector<BillingRule>& getBillingRules() { return billingRules; }
    
    // 添加租赁记录
    void addRentalRecord(const RentalRecord& record) {
        rentalRecords.push_back(record);
        logMessage("添加租赁记录: " + record.getRentalId());
    }
    
    // 添加通知
    void addNotification(const Notification& notification) {
        notifications.push_back(notification);
        logMessage("添加通知: " + notification.notificationId + " 给用户 " + notification.userId);
    }
    
    // 添加账单
    void addBill(const Bill& bill) {
        bills.push_back(bill);
        logMessage("添加账单: " + bill.getBillId() + " 给用户 " + bill.getUserId());
    }
    
    // 更新租赁记录
    bool updateRentalRecord(const RentalRecord& record) {
        for (auto& r : rentalRecords) {
            if (r.getRentalId() == record.getRentalId()) {
                r = record;
                logMessage("更新租赁记录: " + record.getRentalId());
                return true;
            }
        }
        return false;
    }
    
    // 更新账单
    bool updateBill(const Bill& bill) {
        for (auto& b : bills) {
            if (b.getBillId() == bill.getBillId()) {
                b = bill;
                logMessage("更新账单: " + bill.getBillId());
                return true;
            }
        }
        return false;
    }
    
    // 查找租赁记录
    RentalRecord* findRentalById(const std::string& rentalId) {
        for (auto& record : rentalRecords) {
            if (record.getRentalId() == rentalId) {
                return &record;
            }
        }
        return nullptr;
    }
    
    // 查找账单
    Bill* findBillById(const std::string& billId) {
        for (auto& bill : bills) {
            if (bill.getBillId() == billId) {
                return &bill;
            }
        }
        return nullptr;
    }
    
    // 获取用户的租赁记录
    std::vector<RentalRecord*> getUserRentals(const std::string& userId) {
        std::vector<RentalRecord*> result;
        for (auto& record : rentalRecords) {
            if (record.getUserId() == userId) {
                result.push_back(&record);
            }
        }
        return result;
    }
    
    // 获取用户的账单
    std::vector<Bill*> getUserBills(const std::string& userId) {
        std::vector<Bill*> result;
        for (auto& bill : bills) {
            if (bill.getUserId() == userId) {
                result.push_back(&bill);
            }
        }
        return result;
    }
    
    // 获取用户的通知
    std::vector<Notification*> getUserNotifications(const std::string& userId) {
        std::vector<Notification*> result;
        for (auto& notification : notifications) {
            if (notification.userId == userId) {
                result.push_back(&notification);
            }
        }
        return result;
    }
    
    // 记录日志
    void log(const std::string& message) {
        logMessage(message);
    }
};

#endif // DATAMANAGER_HPP
