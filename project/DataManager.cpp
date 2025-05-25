#include "DataManager.hpp"
#include <iostream>
#include <filesystem>

// // 实现DataManager类的方法
// void DataManager::createDataDirectory() {
//     std::filesystem::path dataDir = "data";
//     if (!std::filesystem::exists(dataDir)) {
//         std::filesystem::create_directory(dataDir);
//     }
// }

void DataManager::loadAllData() {
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

void DataManager::saveAllData() {
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

void DataManager::loadUsers() {
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

void DataManager::saveUsers() {
    try {
        userCollection.saveToFile(usersFilePath);
        logMessage("用户数据保存成功");
    } catch (const std::exception& e) {
        std::cerr << "保存用户数据时发生错误: " << e.what() << std::endl;
        logMessage("保存用户数据失败: " + std::string(e.what()));
    }
}

void DataManager::loadResources() {
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

void DataManager::saveResources() {
    try {
        resourceCollection.saveToFile(resourcesFilePath);
        logMessage("资源数据保存成功");
    } catch (const std::exception& e) {
        std::cerr << "保存资源数据时发生错误: " << e.what() << std::endl;
        logMessage("保存资源数据失败: " + std::string(e.what()));
    }
}

void DataManager::loadRentals() {
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

void DataManager::saveRentals() {
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

void DataManager::loadNotifications() {
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

void DataManager::saveNotifications() {
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

void DataManager::loadBills() {
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

void DataManager::saveBills() {
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

void DataManager::loadBillingRules() {
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

void DataManager::saveBillingRules() {
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

void DataManager::logMessage(const std::string& message) {
    std::ofstream logFile(logFilePath, std::ios::app);
    if (logFile) {
        auto now = std::chrono::system_clock::now();
        auto nowTime = std::chrono::system_clock::to_time_t(now);
        logFile << std::ctime(&nowTime) << " - " << message << std::endl;
        logFile.close();
    }
}
