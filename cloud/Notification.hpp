#ifndef NOTIFICATION_HPP
#define NOTIFICATION_HPP

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <fstream>
#include <algorithm>
#include "Rental.hpp"

// 通知优先级枚举
enum class NotificationPriority {
    LOW,
    MEDIUM,
    HIGH
};

// 枚举转字符串函数
inline const char* NotificationPriorityToString(NotificationPriority priority) {
    switch(priority) {
        case NotificationPriority::LOW: return "低";
        case NotificationPriority::MEDIUM: return "中";
        case NotificationPriority::HIGH: return "高";
        default: return "未知优先级";
    }
}

/**
 * @class Notification
 * @brief 表示系统中的一条通知。
 *
 * 包含通知的所有信息，如ID、用户ID、消息内容、优先级、时间戳等。
 */
class Notification {
public:
    std::string notificationId;
    std::string userId;
    std::string message;
    NotificationPriority priority;
    std::chrono::system_clock::time_point timestamp;
    bool isRead;

    // 构造函数
    Notification(std::string id, std::string user, std::string msg, NotificationPriority prio = NotificationPriority::MEDIUM)
        : notificationId(id), userId(user), message(msg), priority(prio), 
          timestamp(std::chrono::system_clock::now()), isRead(false) {}
    
    // 为反序列化添加默认构造函数
    Notification() : notificationId(""), userId(""), message(""), 
                     priority(NotificationPriority::MEDIUM), isRead(false) {}

    // 显示通知详情
    void display() const {
        std::cout << "通知ID: " << notificationId << "\n";
        std::cout << "优先级: " << NotificationPriorityToString(priority) << "\n";
        std::cout << "状态: " << (isRead ? "已读" : "未读") << "\n";
        
        // 显示时间
        auto timeT = std::chrono::system_clock::to_time_t(timestamp);
        std::cout << "时间: " << std::ctime(&timeT);
        
        std::cout << "内容: " << message << "\n";
    }

    // 序列化/反序列化方法（用于数据持久化）
    void serialize(std::ostream& os) const {
        // 写入基本属性
        os.write(notificationId.c_str(), notificationId.size() + 1);
        os.write(userId.c_str(), userId.size() + 1);
        os.write(message.c_str(), message.size() + 1);
        
        // 写入优先级和已读状态
        os.write(reinterpret_cast<const char*>(&priority), sizeof(NotificationPriority));
        os.write(reinterpret_cast<const char*>(&isRead), sizeof(bool));
        
        // 写入时间戳
        auto timeT = std::chrono::system_clock::to_time_t(timestamp);
        os.write(reinterpret_cast<const char*>(&timeT), sizeof(time_t));
    }
    
    void deserialize(std::istream& is) {
        // 读取基本属性
        char buffer[1024];
        is.getline(buffer, 1024, '\0');
        notificationId = buffer;
        
        is.getline(buffer, 1024, '\0');
        userId = buffer;
        
        is.getline(buffer, 1024, '\0');
        message = buffer;
        
        // 读取优先级和已读状态
        is.read(reinterpret_cast<char*>(&priority), sizeof(NotificationPriority));
        is.read(reinterpret_cast<char*>(&isRead), sizeof(bool));
        
        // 读取时间戳
        time_t timeT;
        is.read(reinterpret_cast<char*>(&timeT), sizeof(time_t));
        timestamp = std::chrono::system_clock::from_time_t(timeT);
    }
};

/**
 * @class NotificationManager
 * @brief 管理系统中的所有通知。
 *
 * 负责通知的创建、查找、标记已读和数据持久化。
 */
class NotificationManager {
private:
    std::vector<Notification> notifications;

public:
    // 创建通知
    void createNotification(const std::string& userId, const std::string& message, NotificationPriority priority = NotificationPriority::MEDIUM);
    
    // 创建租赁状态变更通知
    void createRentalStatusNotification(const std::string& userId, const std::string& rentalId, RentalStatus status);
    
    // 创建余额不足通知
    void createLowBalanceNotification(const std::string& userId, double balance);
    
    // 创建租赁即将到期通知
    void createRentalExpiryNotification(const std::string& userId, const std::string& rentalId, int hoursLeft);
    
    // 获取用户的所有通知
    std::vector<Notification> getUserNotifications(const std::string& userId, bool includeRead = true) const;
    
    // 获取用户的未读通知
    std::vector<Notification> getUnreadUserNotifications(const std::string& userId) const;
    
    // 根据ID查找通知
    Notification* getNotificationById(const std::string& notificationId);
    
    // 将通知标记为已读
    bool markNotificationAsRead(const std::string& notificationId);
    
    // 将用户的所有通知标记为已读
    void markAllUserNotificationsAsRead(const std::string& userId);
    
    // 清除用户通知
    void clearUserNotifications(const std::string& userId, bool clearOnlyRead = true);
    
    // 自动清理旧通知
    void autoClearOldNotifications(int daysOld = 30);
    
    // 显示用户通知
    void displayUserNotifications(const std::string& userId, bool includeRead = true) const;
    
    // 保存通知到文件
    bool saveToFile(const std::string& filename) const;
    
    // 从文件加载通知
    bool loadFromFile(const std::string& filename);
    
private:
    // 生成唯一的通知ID
    std::string generateNotificationId() const;
};

#endif // NOTIFICATION_HPP
