#ifndef NOTIFICATION_HPP
#define NOTIFICATION_HPP

#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "User.hpp"
#include "Rental.hpp" 

// 通知优先级
enum class NotificationPriority {
    LOW,
    MEDIUM,
    HIGH // 用于重要提醒，如租赁超时、余额不足等
};

// 将枚举转换为字符串的辅助函数
inline const char* NotificationPriorityToString(NotificationPriority priority) {
    switch(priority) {
        case NotificationPriority::LOW: return "低";
        case NotificationPriority::MEDIUM: return "中";
        case NotificationPriority::HIGH: return "高";
        default: return "未知优先级";
    }
}

/**
 * @struct Notification
 * @brief 表示用户的系统通知。
 *
 * 包含消息详情、时间戳、优先级和阅读状态。
 */
struct Notification {
    std::string notificationId;
    std::string userId; // 通知接收用户的ID
    std::string message;
    std::chrono::system_clock::time_point timestamp;
    NotificationPriority priority;
    bool isRead;

    // 构造函数
    Notification(std::string nId, std::string uId, std::string msg, NotificationPriority prio)
        : notificationId(std::move(nId)), userId(std::move(uId)), message(std::move(msg)),
          timestamp(std::chrono::system_clock::now()), priority(prio), isRead(false) {}
    
    // 默认构造函数用于反序列化
    Notification() : notificationId(""), userId(""), message(""),
                     timestamp(std::chrono::system_clock::now()),
                     priority(NotificationPriority::LOW), isRead(false) {}
    
    // 显示通知详情
    void display() const {
        std::cout << "===== 通知 =====\n";
        std::cout << "ID: " << notificationId << "\n";
        std::cout << "优先级: " << NotificationPriorityToString(priority) << "\n";
        
        // 格式化时间输出
        auto timeToString = [](const std::chrono::system_clock::time_point& time) -> std::string {
            auto t = std::chrono::system_clock::to_time_t(time);
            std::tm* now = std::localtime(&t);
            char buffer[128];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now);
            return buffer;
        };
        
        std::cout << "时间: " << timeToString(timestamp) << "\n";
        std::cout << "状态: " << (isRead ? "已读" : "未读") << "\n";
        std::cout << "消息: " << message << "\n";
    }
    
    // 序列化方法
    void serialize(std::ostream& os) const {
        os.write(notificationId.c_str(), notificationId.size() + 1);
        os.write(userId.c_str(), userId.size() + 1);
        os.write(message.c_str(), message.size() + 1);
        
        os.write(reinterpret_cast<const char*>(&priority), sizeof(NotificationPriority));
        
        auto timeT = std::chrono::system_clock::to_time_t(timestamp);
        os.write(reinterpret_cast<const char*>(&timeT), sizeof(time_t));
        
        os.write(reinterpret_cast<const char*>(&isRead), sizeof(bool));
    }
    
    // 反序列化方法
    void deserialize(std::istream& is) {
        char buffer[1024];
        
        is.getline(buffer, 1024, '\0');
        notificationId = buffer;
        
        is.getline(buffer, 1024, '\0');
        userId = buffer;
        
        is.getline(buffer, 1024, '\0');
        message = buffer;
        
        is.read(reinterpret_cast<char*>(&priority), sizeof(NotificationPriority));
        
        time_t timeT;
        is.read(reinterpret_cast<char*>(&timeT), sizeof(time_t));
        timestamp = std::chrono::system_clock::from_time_t(timeT);
        
        is.read(reinterpret_cast<char*>(&isRead), sizeof(bool));
    }
};

/**
 * @class NotificationManager
 * @brief 管理通知的创建、存储和检索。
 *
 * 处理各种系统事件的通知发送（如租赁状态变更、余额不足）。
 */
class NotificationManager {
private:
    std::vector<Notification> notifications;
    
    // 生成唯一的通知ID
    std::string generateNotificationId() const {
        // 使用时间戳和随机数
        auto now = std::chrono::system_clock::now();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        return "NOTIF_" + std::to_string(millis) + "_" + std::to_string(rand() % 1000);
    }

public:
    NotificationManager() = default;

    // 通知创建
    void createNotification(const std::string& userId, const std::string& message, NotificationPriority priority) {
        std::string notificationId = generateNotificationId();
        notifications.emplace_back(notificationId, userId, message, priority);
    }

    // 创建租赁状态变更通知
    void createRentalStatusNotification(const std::string& userId, const std::string& rentalId, RentalStatus status) {
        std::string message;
        NotificationPriority priority;
        
        switch(status) {
            case RentalStatus::APPROVED:
                message = "您的租用申请 " + rentalId + " 已被批准。";
                priority = NotificationPriority::MEDIUM;
                break;
            case RentalStatus::REJECTED:
                message = "您的租用申请 " + rentalId + " 已被拒绝。";
                priority = NotificationPriority::HIGH;
                break;
            case RentalStatus::ACTIVE:
                message = "您的租用 " + rentalId + " 已开始。";
                priority = NotificationPriority::MEDIUM;
                break;
            case RentalStatus::COMPLETED:
                message = "您的租用 " + rentalId + " 已完成。";
                priority = NotificationPriority::MEDIUM;
                break;
            case RentalStatus::CANCELLED:
                message = "您的租用申请 " + rentalId + " 已取消。";
                priority = NotificationPriority::LOW;
                break;
            default:
                message = "您的租用 " + rentalId + " 状态已更新。";
                priority = NotificationPriority::LOW;
        }
        
        createNotification(userId, message, priority);
    }

    // 创建余额不足通知
    void createLowBalanceNotification(const std::string& userId, double balance) {
        std::string message = "您的账户余额不足，当前余额: " + std::to_string(balance) + " 元。请及时充值以确保服务不中断。";
        createNotification(userId, message, NotificationPriority::HIGH);
    }

    // 创建租赁即将到期通知
    void createRentalExpiryNotification(const std::string& userId, const std::string& rentalId, int hoursLeft) {
        std::string message = "您的租用 " + rentalId + " 将在 " + std::to_string(hoursLeft) + " 小时后到期。";
        createNotification(userId, message, NotificationPriority::MEDIUM);
    }

    // 获取用户的所有通知
    std::vector<Notification> getUserNotifications(const std::string& userId, bool includeRead = false) const {
        std::vector<Notification> userNotifications;
        
        for (const auto& notification : notifications) {
            if (notification.userId == userId && (includeRead || !notification.isRead)) {
                userNotifications.push_back(notification);
            }
        }
        
        // 按时间倒序排序，最新的通知在前
        std::sort(userNotifications.begin(), userNotifications.end(), 
                 [](const Notification& a, const Notification& b) {
                     return a.timestamp > b.timestamp;
                 });
        
        return userNotifications;
    }

    // 获取用户的未读通知
    std::vector<Notification> getUnreadUserNotifications(const std::string& userId) const {
        return getUserNotifications(userId, false);
    }

    // 根据ID查找通知
    Notification* getNotificationById(const std::string& notificationId) {
        for (auto& notification : notifications) {
            if (notification.notificationId == notificationId) {
                return &notification;
            }
        }
        return nullptr;
    }

    // 将通知标记为已读
    bool markNotificationAsRead(const std::string& notificationId) {
        Notification* notification = getNotificationById(notificationId);
        if (notification) {
            notification->isRead = true;
            return true;
        }
        return false;
    }

    // 将用户的所有通知标记为已读
    void markAllUserNotificationsAsRead(const std::string& userId) {
        for (auto& notification : notifications) {
            if (notification.userId == userId && !notification.isRead) {
                notification.isRead = true;
            }
        }
    }

    // 清除用户通知
    void clearUserNotifications(const std::string& userId, bool clearOnlyRead = true) {
        auto it = notifications.begin();
        while (it != notifications.end()) {
            if (it->userId == userId && (!clearOnlyRead || it->isRead)) {
                it = notifications.erase(it);
            } else {
                ++it;
            }
        }
    }

    // 自动清理旧通知
    void autoClearOldNotifications(int daysOld = 30) {
        auto now = std::chrono::system_clock::now();
        auto cutoff = now - std::chrono::hours(24 * daysOld);
        
        auto it = notifications.begin();
        while (it != notifications.end()) {
            if (it->timestamp < cutoff && it->isRead) {
                it = notifications.erase(it);
            } else {
                ++it;
            }
        }
    }

    // 加载通知数据
    void loadNotifications(const std::vector<Notification>& loadedNotifications) {
        notifications = loadedNotifications;
    }

    // 获取所有通知
    const std::vector<Notification>& getAllNotifications() const {
        return notifications;
    }
    
    // 显示用户通知
    void displayUserNotifications(const std::string& userId, bool includeRead = false) const {
        auto userNotifications = getUserNotifications(userId, includeRead);
        
        if (userNotifications.empty()) {
            std::cout << "没有" << (includeRead ? "" : "未读") << "通知。\n";
            return;
        }
        
        std::cout << "===== 用户通知 =====\n";
        for (const auto& notification : userNotifications) {
            notification.display();
            std::cout << "-------------------\n";
        }
    }
    
    // 获取用户的高优先级未读通知
    std::vector<Notification> getHighPriorityUnreadNotifications(const std::string& userId) const {
        std::vector<Notification> result;
        for (const auto& notification : notifications) {
            if (notification.userId == userId && 
                !notification.isRead && 
                notification.priority == NotificationPriority::HIGH) {
                result.push_back(notification);
            }
        }
        return result;
    }
    
    // 保存通知到文件
    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "无法打开文件进行写入: " << filename << std::endl;
            return false;
        }
        
        // 写入通知数量
        size_t count = notifications.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));
        
        // 逐个写入通知
        for (const auto& notification : notifications) {
            notification.serialize(file);
        }
        
        file.close();
        return true;
    }
    
    // 从文件加载通知
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "无法打开文件进行读取: " << filename << std::endl;
            return false;
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
        return true;
    }
};

#endif // NOTIFICATION_HPP
