#include "DataManager_fixed.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <direct.h> // 用于Windows下创建目录
#include <sys/stat.h> // 用于Linux下创建目录

// 创建数据目录
void DataManager::createDataDirectory() {
    #ifdef _WIN32
        _mkdir("data");
    #else
        mkdir("data", 0777);
    #endif
    
    std::cout << "数据目录已创建。\n";
}

// 加载所有数据
void DataManager::loadAllData() {
    // 确保数据目录存在
    createDataDirectory();
    
    loadUsers();
    loadResources();
    loadRentals();
    loadNotifications();
    loadBills();
    loadBillingRules();
    
    std::cout << "所有数据已加载。\n";
}

// 保存所有数据
void DataManager::saveAllData() {
    // 确保数据目录存在
    createDataDirectory();
    
    saveUsers();
    saveResources();
    saveRentals();
    saveNotifications();
    saveBills();
    saveBillingRules();
    
    std::cout << "所有数据已保存。\n";
}

// 加载用户数据
void DataManager::loadUsers() {
    try {
        std::ifstream file(usersFilePath, std::ios::binary);
        if (!file) {
            std::cerr << "警告: 无法打开用户数据文件，将创建新文件。\n";
            // 创建默认用户
            userCollection = createDefaultUserCollection();
            return;
        }
        
        // 使用UserCollection的loadFromFile方法加载用户数据
        userCollection.loadFromFile(usersFilePath);
        
        std::cout << "用户数据已加载。\n";
    } catch (const std::exception& e) {
        std::cerr << "加载用户数据时发生错误: " << e.what() << "\n";
        // 如果加载失败，创建默认用户
        userCollection = createDefaultUserCollection();
    }
}

// 保存用户数据
void DataManager::saveUsers() {
    try {
        // 使用UserCollection的saveToFile方法保存用户数据
        userCollection.saveToFile(usersFilePath);
        
        std::cout << "用户数据已保存。\n";
    } catch (const std::exception& e) {
        std::cerr << "保存用户数据时发生错误: " << e.what() << "\n";
    }
}

// 加载资源数据
void DataManager::loadResources() {
    try {
        std::ifstream file(resourcesFilePath, std::ios::binary);
        if (!file) {
            std::cerr << "警告: 无法打开资源数据文件，将创建新文件。\n";
            
            // 创建默认资源
            resourceCollection = createDefaultResourceCollection();
            saveResources(); // 立即保存默认资源到文件
            return;
        }
        
        // 清空当前资源集合
        resourceCollection = ResourceCollection();
        
        // 读取资源数量
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
        
        // 逐个读取资源
        for (size_t i = 0; i < count; ++i) {
            // 读取资源类型标识
            ResourceType type;
            file.read(reinterpret_cast<char*>(&type), sizeof(ResourceType));
            
            // 根据类型创建相应的资源对象
            Resource* resource = nullptr;
            switch (type) {
                case ResourceType::CPU:
                    resource = new CPUResource();
                    break;
                case ResourceType::GPU:
                    resource = new GPUResource();
                    break;
                default:
                    throw std::runtime_error("未知的资源类型");
            }
            
            // 反序列化资源
            resource->deserialize(file);
            
            // 添加到集合
            resourceCollection.addResource(resource);
        }
        
        file.close();
        std::cout << "资源数据已加载。\n";
    } catch (const std::exception& e) {
        std::cerr << "加载资源数据时发生错误: " << e.what() << "\n";
        
        // 创建默认资源
        resourceCollection = createDefaultResourceCollection();
        saveResources(); // 立即保存默认资源到文件
    }
}

// 保存资源数据
void DataManager::saveResources() {
    try {
        // 确保数据目录存在
        createDataDirectory();
        
        std::ofstream file(resourcesFilePath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行写入: " + resourcesFilePath);
        }
        
        // 获取所有资源
        const auto& resources = resourceCollection.getAllResources();
        
        // 写入资源数量
        size_t count = resources.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));
        
        // 逐个写入资源
        for (const auto& resource : resources) {
            // 写入资源类型标识
            ResourceType type = resource->getResourceType();
            file.write(reinterpret_cast<const char*>(&type), sizeof(ResourceType));
            
            // 序列化资源
            resource->serialize(file);
        }
        
        file.close();
        std::cout << "资源数据已保存。\n";
    } catch (const std::exception& e) {
        std::cerr << "保存资源数据时发生错误: " << e.what() << "\n";
    }
}

// 加载租赁数据
void DataManager::loadRentals() {
    try {
        std::ifstream file(rentalsFilePath, std::ios::binary);
        if (!file) {
            std::cerr << "警告: 无法打开租赁数据文件，将创建新文件。\n";
            return;
        }
        
        // 清空当前租赁记录
        rentalRecords.clear();
        
        // 读取租赁记录数量
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
        
        // 逐个读取租赁记录
        for (size_t i = 0; i < count; ++i) {
            RentalRecord rental;
            rental.deserialize(file);
            rentalRecords.push_back(rental);
        }
        
        file.close();
        std::cout << "租赁数据已加载。\n";
    } catch (const std::exception& e) {
        std::cerr << "加载租赁数据时发生错误: " << e.what() << "\n";
    }
}

// 保存租赁数据
void DataManager::saveRentals() {
    try {
        // 确保数据目录存在
        createDataDirectory();
        
        std::ofstream file(rentalsFilePath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行写入: " + rentalsFilePath);
        }
        
        // 写入租赁记录数量
        size_t count = rentalRecords.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));
        
        // 逐个写入租赁记录
        for (const auto& rental : rentalRecords) {
            rental.serialize(file);
        }
        
        file.close();
        std::cout << "租赁数据已保存。\n";
    } catch (const std::exception& e) {
        std::cerr << "保存租赁数据时发生错误: " << e.what() << "\n";
    }
}

// 加载通知数据
void DataManager::loadNotifications() {
    try {
        std::ifstream file(notificationsFilePath, std::ios::binary);
        if (!file) {
            std::cerr << "警告: 无法打开通知数据文件，将创建新文件。\n";
            return;
        }
        
        // 清空当前通知
        notifications.clear();
        
        // 读取通知数量
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
        
        // 逐个读取通知
        for (size_t i = 0; i < count; ++i) {
            Notification notification;
            notification.deserialize(file);
            notifications.push_back(notification);
        }
        
        file.close();
        std::cout << "通知数据已加载。\n";
    } catch (const std::exception& e) {
        std::cerr << "加载通知数据时发生错误: " << e.what() << "\n";
    }
}

// 保存通知数据
void DataManager::saveNotifications() {
    try {
        // 确保数据目录存在
        createDataDirectory();
        
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
        
        file.close();
        std::cout << "通知数据已保存。\n";
    } catch (const std::exception& e) {
        std::cerr << "保存通知数据时发生错误: " << e.what() << "\n";
    }
}

// 加载账单数据
void DataManager::loadBills() {
    try {
        std::ifstream file(billsFilePath, std::ios::binary);
        if (!file) {
            std::cerr << "警告: 无法打开账单数据文件，将创建新文件。\n";
            return;
        }
        
        // 清空当前账单
        bills.clear();
        
        // 读取账单数量
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
        
        // 逐个读取账单
        for (size_t i = 0; i < count; ++i) {
            Bill bill;
            bill.deserialize(file);
            bills.push_back(bill);
        }
        
        file.close();
        std::cout << "账单数据已加载。\n";
    } catch (const std::exception& e) {
        std::cerr << "加载账单数据时发生错误: " << e.what() << "\n";
    }
}

// 保存账单数据
void DataManager::saveBills() {
    try {
        // 确保数据目录存在
        createDataDirectory();
        
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
        
        file.close();
        std::cout << "账单数据已保存。\n";
    } catch (const std::exception& e) {
        std::cerr << "保存账单数据时发生错误: " << e.what() << "\n";
    }
}

// 加载计费规则
void DataManager::loadBillingRules() {
    try {
        std::ifstream file(billingRulesFilePath, std::ios::binary);
        if (!file) {
            std::cerr << "警告: 无法打开计费规则数据文件，将使用默认规则。\n";
            createDefaultBillingRules();
            saveBillingRules(); // 立即保存默认规则到文件
            return;
        }
        
        // 清空当前规则
        billingRules.clear();
        
        // 读取规则数量
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
        
        // 逐个读取规则
        for (size_t i = 0; i < count; ++i) {
            BillingRule rule;
            rule.deserialize(file);
            billingRules.push_back(rule);
        }
        
        file.close();
        std::cout << "计费规则已加载。\n";
    } catch (const std::exception& e) {
        std::cerr << "加载计费规则时发生错误: " << e.what() << "\n";
        createDefaultBillingRules();
        saveBillingRules(); // 立即保存默认规则到文件
    }
}

// 保存计费规则
void DataManager::saveBillingRules() {
    try {
        // 确保数据目录存在
        createDataDirectory();
        
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
        
        file.close();
        std::cout << "计费规则已保存。\n";
    } catch (const std::exception& e) {
        std::cerr << "保存计费规则时发生错误: " << e.what() << "\n";
    }
}

// 记录日志
void DataManager::logMessage(const std::string& message) {
    try {
        // 确保数据目录存在
        createDataDirectory();
        
        std::ofstream file(logFilePath, std::ios::app);
        if (!file) {
            std::cerr << "无法打开日志文件进行写入: " << logFilePath << "\n";
            return;
        }
        
        // 获取当前时间
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&now_time_t);
        
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", now_tm);
        
        // 写入日志
        file << "[" << time_str << "] " << message << std::endl;
        
        file.close();
    } catch (const std::exception& e) {
        std::cerr << "记录日志时发生错误: " << e.what() << "\n";
    }
}
