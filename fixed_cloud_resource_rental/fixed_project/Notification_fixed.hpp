#ifndef NOTIFICATION_HPP
#define NOTIFICATION_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <ctime>
#include <stdexcept>
#include <memory>

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
 * @brief 表示系统通知。
 *
 * 包含通知的基本信息和状态。
 */
class Notification {
private:
    std::string notificationId;
    std::string userId;
    std::string message;
    NotificationPriority priority;
    bool read;
    std::chrono::system_clock::time_point creationTime;

public:
    // 构造函数
    Notification(std::string id, std::string uid, std::string msg, NotificationPriority prio = NotificationPriority::MEDIUM)
        : notificationId(id), userId(uid), message(msg), priority(prio), read(false),
          creationTime(std::chrono::system_clock::now()) {}
    
    // 为反序列化添加默认构造函数
    Notification() : notificationId(""), userId(""), message(""), 
                     priority(NotificationPriority::MEDIUM), read(false) {}
    
    // 获取器
    std::string getNotificationId() const { return notificationId; }
    std::string getUserId() const { return userId; }
    std::string getMessage() const { return message; }
    NotificationPriority getPriority() const { return priority; }
    bool isRead() const { return read; }
    std::chrono::system_clock::time_point getCreationTime() const { return creationTime; }
    
    // 标记为已读
    void markAsRead() { read = true; }
    
    // 获取格式化的时间字符串
    std::string getFormattedTime() const {
        auto timeT = std::chrono::system_clock::to_time_t(creationTime);
        std::tm* timeInfo = std::localtime(&timeT);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M", timeInfo);
        return std::string(timeStr);
    }
    
    // 序列化/反序列化方法（用于数据持久化）
    void serialize(std::ostream& os) const {
        os.write(notificationId.c_str(), notificationId.size() + 1);
        os.write(userId.c_str(), userId.size() + 1);
        os.write(message.c_str(), message.size() + 1);
        os.write(reinterpret_cast<const char*>(&priority), sizeof(NotificationPriority));
        os.write(reinterpret_cast<const char*>(&read), sizeof(bool));
        
        // 序列化时间点
        auto timeT = std::chrono::system_clock::to_time_t(creationTime);
        os.write(reinterpret_cast<const char*>(&timeT), sizeof(std::time_t));
    }
    
    void deserialize(std::istream& is) {
        char buffer[1024];
        is.getline(buffer, 1024, '\0');
        notificationId = buffer;
        
        is.getline(buffer, 1024, '\0');
        userId = buffer;
        
        is.getline(buffer, 1024, '\0');
        message = buffer;
        
        is.read(reinterpret_cast<char*>(&priority), sizeof(NotificationPriority));
        is.read(reinterpret_cast<char*>(&read), sizeof(bool));
        
        // 反序列化时间点
        std::time_t timeT;
        is.read(reinterpret_cast<char*>(&timeT), sizeof(std::time_t));
        creationTime = std::chrono::system_clock::from_time_t(timeT);
    }
};

/**
 * @class NotificationManager
 * @brief 管理系统中的所有通知。
 *
 * 负责通知的创建、查找和数据持久化。
 */
class NotificationManager {
private:
    std::vector<Notification> notifications;
    int nextNotificationId;

public:
    NotificationManager() : nextNotificationId(1) {}
    
    // 创建通知
    std::string createNotification(const std::string& userId, const std::string& message, 
                                  NotificationPriority priority = NotificationPriority::MEDIUM) {
        // 生成通知ID
        char idStr[6];
        sprintf(idStr, "N%04d", nextNotificationId++);
        std::string notificationId = idStr;
        
        // 创建新通知
        notifications.push_back(Notification(notificationId, userId, message, priority));
        
        return notificationId;
    }
    
    // 创建余额不足通知
    std::string createLowBalanceNotification(const std::string& userId, double balance) {
        std::string message = "您的账户余额不足，当前余额: " + std::to_string(balance) + " 元。请及时充值。";
        return createNotification(userId, message, NotificationPriority::HIGH);
    }
    
    // 创建租赁状态变更通知
    std::string createRentalStatusNotification(const std::string& userId, const std::string& rentalId, 
                                              RentalStatus status) {
        std::string statusStr;
        switch (status) {
            case RentalStatus::PENDING:
                statusStr = "申请中";
                break;
            case RentalStatus::APPROVED:
                statusStr = "已批准";
                break;
            case RentalStatus::REJECTED:
                statusStr = "已拒绝";
                break;
            case RentalStatus::COMPLETED:
                statusStr = "已完成";
                break;
            case RentalStatus::CANCELLED:
                statusStr = "已取消";
                break;
            default:
                statusStr = "未知状态";
                break;
        }
        
        std::string message = "您的租赁申请 " + rentalId + " 状态已更新为: " + statusStr;
        return createNotification(userId, message, NotificationPriority::HIGH);
    }
    
    // 获取用户的所有通知
    std::vector<Notification> getUserNotifications(const std::string& userId) const {
        std::vector<Notification> result;
        for (const auto& notification : notifications) {
            if (notification.getUserId() == userId) {
                result.push_back(notification);
            }
        }
        return result;
    }
    
    // 获取用户的未读通知
    std::vector<Notification> getUnreadUserNotifications(const std::string& userId) const {
        std::vector<Notification> result;
        for (const auto& notification : notifications) {
            if (notification.getUserId() == userId && !notification.isRead()) {
                result.push_back(notification);
            }
        }
        return result;
    }
    
    // 标记通知为已读
    bool markNotificationAsRead(const std::string& notificationId) {
        for (auto& notification : notifications) {
            if (notification.getNotificationId() == notificationId) {
                notification.markAsRead();
                return true;
            }
        }
        return false; // 未找到通知
    }
    
    // 从文件加载通知
    void loadFromFile(const std::string& filename) {
        try {
            std::ifstream file(filename, std::ios::binary);
            if (!file) {
                std::cerr << "警告: 无法打开通知数据文件，将创建新文件。\n";
                return;
            }
            
            // 清空当前通知
            notifications.clear();
            
            // 读取下一个可用的通知ID
            file.read(reinterpret_cast<char*>(&nextNotificationId), sizeof(int));
            
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
        } catch (const std::exception& e) {
            std::cerr << "加载通知数据时发生错误: " << e.what() << "\n";
        }
    }
    
    // 保存通知到文件
    void saveToFile(const std::string& filename) const {
        try {
            // 确保data目录存在
            #ifdef _WIN32
                _mkdir("data");
            #else
                mkdir("data", 0777);
            #endif
            
            std::ofstream file(filename, std::ios::binary);
            if (!file) {
                throw std::runtime_error("无法打开文件进行写入: " + filename);
            }
            
            // 写入下一个可用的通知ID
            file.write(reinterpret_cast<const char*>(&nextNotificationId), sizeof(int));
            
            // 写入通知数量
            size_t count = notifications.size();
            file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));
            
            // 逐个写入通知
            for (const auto& notification : notifications) {
                notification.serialize(file);
            }
            
            file.close();
        } catch (const std::exception& e) {
            std::cerr << "保存通知数据时发生错误: " << e.what() << "\n";
        }
    }
};

#endif // NOTIFICATION_HPP
