#ifndef NOTIFICATION_MANAGER_H
#define NOTIFICATION_MANAGER_H

#include "notification.h"
#include "user_manager.h"
#include "exception.h"
#include "data_manager.h"
#include <vector>
#include <memory>
#include <algorithm>

// 通知管理器类
class NotificationManager {
private:
    std::vector<std::shared_ptr<Notification>> notifications;
    std::shared_ptr<DataManager> dataManager;
    std::shared_ptr<UserManager> userManager;
    
    // 查找通知
    std::shared_ptr<Notification> findNotificationById(const std::string& id) {
        for (const auto& notification : notifications) {
            if (notification->getId() == id) {
                return notification;
            }
        }
        return nullptr;
    }
    
public:
    NotificationManager(std::shared_ptr<DataManager> dataManager,
                       std::shared_ptr<UserManager> userManager) 
        : dataManager(dataManager), userManager(userManager) {
        // 加载通知数据
        dataManager->loadNotifications(notifications);
    }
    
    ~NotificationManager() {
        saveNotifications();
    }
    
    // 保存通知数据
    bool saveNotifications() {
        return dataManager->saveNotifications(notifications);
    }
    
    // 创建通知
    bool createNotification(const std::string& userId, const std::string& content, 
                           NotificationPriority priority = MEDIUM) {
        try {
            auto notification = std::make_shared<Notification>(userId, content, priority);
            notifications.push_back(notification);
            saveNotifications();
            return true;
        } catch (const Exception& e) {
            std::cerr << "创建通知失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 标记通知为已读
    bool markAsRead(const std::string& notificationId) {
        try {
            auto currentUser = userManager->getCurrentUser();
            if (currentUser == nullptr) {
                throw UserException("未登录");
            }
            
            auto notification = findNotificationById(notificationId);
            if (notification == nullptr) {
                throw DataException("通知不存在"); // 或者定义 NotificationException
            }
            
            if (notification->getUserId() != currentUser->getId() && currentUser->getType() != ADMIN) {
                throw UserException("没有权限操作此通知");
            }
            
            notification->markAsRead();
            saveNotifications();
            return true;
        } catch (const Exception& e) {
            std::cerr << "标记通知为已读失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 删除通知
    bool deleteNotification(const std::string& notificationId) {
        try {
            auto currentUser = userManager->getCurrentUser();
            if (currentUser == nullptr) {
                throw UserException("未登录");
            }
            
            auto it = std::find_if(notifications.begin(), notifications.end(), 
                                  [&notificationId](const std::shared_ptr<Notification>& notification) {
                                      return notification->getId() == notificationId;
                                  });
            
            if (it == notifications.end()) {
                throw DataException("通知不存在"); // 或者定义 NotificationException
            }
            
            if ((*it)->getUserId() != currentUser->getId() && currentUser->getType() != ADMIN) {
                throw UserException("没有权限删除此通知");
            }
            
            notifications.erase(it);
            saveNotifications();
            return true;
        } catch (const Exception& e) {
            std::cerr << "删除通知失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 获取用户的通知
    std::vector<std::shared_ptr<Notification>> getUserNotifications(const std::string& userId) const {
        std::vector<std::shared_ptr<Notification>> result;
        for (const auto& notification : notifications) {
            if (notification->getUserId() == userId) {
                result.push_back(notification);
            }
        }
        return result;
    }
    
    // 获取用户的未读通知
    std::vector<std::shared_ptr<Notification>> getUserUnreadNotifications(const std::string& userId) const {
        std::vector<std::shared_ptr<Notification>> result;
        for (const auto& notification : notifications) {
            if (notification->getUserId() == userId && notification->isUnread()) {
                result.push_back(notification);
            }
        }
        return result;
    }
    
    // 获取用户的高优先级通知
    std::vector<std::shared_ptr<Notification>> getUserHighPriorityNotifications(const std::string& userId) const {
        std::vector<std::shared_ptr<Notification>> result;
        for (const auto& notification : notifications) {
            if (notification->getUserId() == userId && notification->isHighPriority()) {
                result.push_back(notification);
            }
        }
        return result;
    }
    
    // 获取所有通知
    std::vector<std::shared_ptr<Notification>> getAllNotifications() const {
        return notifications;
    }
    
    // 清理已读通知
    bool cleanReadNotifications() {
        try {
            auto currentUser = userManager->getCurrentUser();
            if (currentUser == nullptr) {
                throw UserException("未登录");
            }
            
            auto it = notifications.begin();
            while (it != notifications.end()) {
                if ((*it)->getUserId() == currentUser->getId() && !(*it)->isUnread()) {
                    it = notifications.erase(it);
                } else {
                    ++it;
                }
            }
            
            saveNotifications();
            return true;
        } catch (const Exception& e) {
            std::cerr << "清理已读通知失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 创建余额不足通知
    bool createLowBalanceNotification(const std::string& userId) {
        std::string content = "您的账户余额不足，请及时充值，以免影响正常使用。";
        return createNotification(userId, content, HIGH);
    }
    
    // 创建租用审核通知
    bool createRentalReviewNotification(const std::string& userId, const std::string& rentalId, bool approved) {
        std::string content = "您的租用申请 " + rentalId + " 已被" + (approved ? "通过" : "拒绝") + "。";
        return createNotification(userId, content, HIGH);
    }
    
    // 创建租用即将到期通知
    bool createRentalExpiringNotification(const std::string& userId, const std::string& rentalId, const std::string& endTime) {
        std::string content = "您的租用 " + rentalId + " 将于 " + endTime + " 到期，请及时处理。";
        return createNotification(userId, content, MEDIUM);
    }
    
    // 创建账号状态变更通知
    bool createAccountStatusNotification(const std::string& userId, UserStatus status) {
        std::string content = "您的账号状态已变更为" + std::string(status == ACTIVE ? "激活" : "暂停") + "。";
        return createNotification(userId, content, HIGH);
    }
};

#endif // NOTIFICATION_MANAGER_H
