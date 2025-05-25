#include "Notification.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <random>

// 创建通知
void NotificationManager::createNotification(const std::string& userId, const std::string& message, NotificationPriority priority) {
    std::string notificationId = generateNotificationId();
    Notification notification(notificationId, userId, message, priority);
    notifications.push_back(notification);
}

// 创建租赁状态变更通知
void NotificationManager::createRentalStatusNotification(const std::string& userId, const std::string& rentalId, RentalStatus status) {
    std::string message;
    NotificationPriority priority;
    
    switch(status) {
        case RentalStatus::APPROVED:
            message = "您的租赁申请 " + rentalId + " 已被批准。";
            priority = NotificationPriority::HIGH;
            break;
        case RentalStatus::REJECTED:
            message = "您的租赁申请 " + rentalId + " 已被拒绝。";
            priority = NotificationPriority::HIGH;
            break;
        case RentalStatus::ACTIVE:
            message = "您的租赁 " + rentalId + " 已开始使用。";
            priority = NotificationPriority::MEDIUM;
            break;
        case RentalStatus::COMPLETED:
            message = "您的租赁 " + rentalId + " 已完成。";
            priority = NotificationPriority::MEDIUM;
            break;
        case RentalStatus::CANCELLED:
            message = "您的租赁申请 " + rentalId + " 已取消。";
            priority = NotificationPriority::MEDIUM;
            break;
        default:
            message = "您的租赁 " + rentalId + " 状态已更新。";
            priority = NotificationPriority::LOW;
    }
    
    createNotification(userId, message, priority);
}

// 创建余额不足通知
void NotificationManager::createLowBalanceNotification(const std::string& userId, double balance) {
    std::stringstream ss;
    ss << "您的账户余额不足，当前余额: " << balance << " 元。请及时充值以确保服务正常使用。";
    createNotification(userId, ss.str(), NotificationPriority::HIGH);
}

// 创建租赁即将到期通知
void NotificationManager::createRentalExpiryNotification(const std::string& userId, const std::string& rentalId, int hoursLeft) {
    std::stringstream ss;
    ss << "您的租赁 " << rentalId << " 将在 " << hoursLeft << " 小时后到期。";
    createNotification(userId, ss.str(), NotificationPriority::MEDIUM);
}

// 获取用户的所有通知
std::vector<Notification> NotificationManager::getUserNotifications(const std::string& userId, bool includeRead) const {
    std::vector<Notification> result;
    for (const auto& notification : notifications) {
        if (notification.userId == userId && (includeRead || !notification.isRead)) {
            result.push_back(notification);
        }
    }
    return result;
}

// 获取用户的未读通知
std::vector<Notification> NotificationManager::getUnreadUserNotifications(const std::string& userId) const {
    return getUserNotifications(userId, false);
}

// 根据ID查找通知
Notification* NotificationManager::getNotificationById(const std::string& notificationId) {
    for (auto& notification : notifications) {
        if (notification.notificationId == notificationId) {
            return &notification;
        }
    }
    return nullptr;
}

// 将通知标记为已读
bool NotificationManager::markNotificationAsRead(const std::string& notificationId) {
    Notification* notification = getNotificationById(notificationId);
    if (notification) {
        notification->isRead = true;
        return true;
    }
    return false;
}

// 将用户的所有通知标记为已读
void NotificationManager::markAllUserNotificationsAsRead(const std::string& userId) {
    for (auto& notification : notifications) {
        if (notification.userId == userId) {
            notification.isRead = true;
        }
    }
}

// 清除用户通知
void NotificationManager::clearUserNotifications(const std::string& userId, bool clearOnlyRead) {
    notifications.erase(
        std::remove_if(notifications.begin(), notifications.end(),
            [userId, clearOnlyRead](const Notification& notification) {
                return notification.userId == userId && (!clearOnlyRead || notification.isRead);
            }
        ),
        notifications.end()
    );
}

// 自动清理旧通知
void NotificationManager::autoClearOldNotifications(int daysOld) {
    auto now = std::chrono::system_clock::now();
    auto cutoff = now - std::chrono::hours(24 * daysOld);
    
    notifications.erase(
        std::remove_if(notifications.begin(), notifications.end(),
            [cutoff](const Notification& notification) {
                return notification.timestamp < cutoff;
            }
        ),
        notifications.end()
    );
}

// 显示用户通知
void NotificationManager::displayUserNotifications(const std::string& userId, bool includeRead) const {
    auto userNotifications = getUserNotifications(userId, includeRead);
    
    if (userNotifications.empty()) {
        std::cout << "您没有" << (includeRead ? "" : "未读") << "通知。\n";
        return;
    }
    
    // 按优先级和时间排序
    std::sort(userNotifications.begin(), userNotifications.end(),
        [](const Notification& a, const Notification& b) {
            if (a.priority != b.priority) {
                return a.priority > b.priority; // 高优先级在前
            }
            return a.timestamp > b.timestamp; // 新通知在前
        }
    );
    
    for (const auto& notification : userNotifications) {
        std::cout << "===== " << (notification.isRead ? "已读" : "未读") << " 通知 =====\n";
        notification.display();
        std::cout << "------------------------\n";
    }
}

// 保存通知到文件
bool NotificationManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
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
bool NotificationManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
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

// 生成唯一的通知ID
std::string NotificationManager::generateNotificationId() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 999999);
    
    // 使用时间戳和随机数生成唯一ID
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch().count();
    
    std::stringstream ss;
    ss << "NOTIF" << value << "-" << dis(gen);
    return ss.str();
}
