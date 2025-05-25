#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <ctime>
#include <filesystem>
#include "User_fixed.hpp"
#include "Resource_fixed.hpp"
#include "Rental_fixed.hpp"
#include "Notification_fixed.hpp"
#include "Billing_fixed.hpp"

/**
 * @class DataManager
 * @brief 管理系统中的所有数据。
 *
 * 负责数据的加载、保存和持久化。
 * 提供创建默认数据的功能。
 */
class DataManager {
private:
    // 数据文件路径
    std::string usersFilePath = "data/users.dat";
    std::string resourcesFilePath = "data/resources.dat";
    std::string rentalsFilePath = "data/rentals.dat";
    std::string notificationsFilePath = "data/notifications.dat";
    std::string billsFilePath = "data/bills.dat";
    std::string billingRulesFilePath = "data/billing_rules.dat";
    std::string logFilePath = "data/system.log";
    
    // 数据集合
    UserCollection userCollection;
    ResourceCollection resourceCollection;
    std::vector<RentalRecord> rentalRecords;
    std::vector<Notification> notifications;
    std::vector<Bill> bills;
    std::vector<BillingRule> billingRules;

public:
    // 创建数据目录
    void createDataDirectory();
    
    // 加载所有数据
    void loadAllData();
    
    // 保存所有数据
    void saveAllData();
    
    // 加载用户数据
    void loadUsers();
    
    // 保存用户数据
    void saveUsers();
    
    // 加载资源数据
    void loadResources();
    
    // 保存资源数据
    void saveResources();
    
    // 加载租赁数据
    void loadRentals();
    
    // 保存租赁数据
    void saveRentals();
    
    // 加载通知数据
    void loadNotifications();
    
    // 保存通知数据
    void saveNotifications();
    
    // 加载账单数据
    void loadBills();
    
    // 保存账单数据
    void saveBills();
    
    // 加载计费规则
    void loadBillingRules();
    
    // 保存计费规则
    void saveBillingRules();
    
    // 记录日志
    void logMessage(const std::string& message);
    
    // 创建默认计费规则
    void createDefaultBillingRules() {
        billingRules.clear();
        billingRules.push_back(BillingRule(ResourceType::CPU, 10.0, 0.8, 0.9));
        billingRules.push_back(BillingRule(ResourceType::GPU, 20.0, 0.8, 0.9));
    }
};

#endif // DATAMANAGER_HPP
