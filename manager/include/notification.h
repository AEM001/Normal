#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "entity.h"

// 通知类
class Notification : public Entity {
private:
    std::string userId;
    std::string content;
    NotificationPriority priority;
    NotificationStatus status;

public:
    Notification() : Entity(), priority(MEDIUM), status(UNREAD) {}
    
    Notification(const std::string& userId, const std::string& content, 
                 NotificationPriority priority = MEDIUM)
        : Entity(), userId(userId), content(content), 
          priority(priority), status(UNREAD) {}
    
    Notification(const std::string& id, const std::string& createTime,
                 const std::string& userId, const std::string& content,
                 NotificationPriority priority, NotificationStatus status)
        : Entity(id, createTime), userId(userId), content(content),
          priority(priority), status(status) {}
    
    // Getters
    std::string getUserId() const { return userId; }
    std::string getContent() const { return content; }
    NotificationPriority getPriority() const { return priority; }
    NotificationStatus getStatus() const { return status; }
    
    // Setters
    void setContent(const std::string& content) { this->content = content; }
    void setPriority(NotificationPriority priority) { this->priority = priority; }
    void setStatus(NotificationStatus status) { this->status = status; }
    
    // 业务方法
    bool isUnread() const { return status == UNREAD; }
    void markAsRead() { status = READ; }
    bool isHighPriority() const { return priority == HIGH; }
    
    // 序列化和反序列化
    void serialize(std::ofstream& out) const override {
        Entity::serialize(out);
        out.write(userId.c_str(), userId.size() + 1);
        out.write(content.c_str(), content.size() + 1);
        out.write(reinterpret_cast<const char*>(&priority), sizeof(priority));
        out.write(reinterpret_cast<const char*>(&status), sizeof(status));
    }
    
    void deserialize(std::ifstream& in) override {
        Entity::deserialize(in);
        char buffer[1024];
        
        in.getline(buffer, 1024, '\0');
        userId = buffer;
        
        in.getline(buffer, 1024, '\0');
        content = buffer;
        
        in.read(reinterpret_cast<char*>(&priority), sizeof(priority));
        in.read(reinterpret_cast<char*>(&status), sizeof(status));
    }
    
    // 显示通知信息
    void displayInfo() const {
        std::cout << "通知ID: " << id << std::endl;
        
        std::cout << "优先级: ";
        switch (priority) {
            case LOW: std::cout << "低"; break;
            case MEDIUM: std::cout << "中"; break;
            case HIGH: std::cout << "高"; break;
        }
        std::cout << std::endl;
        
        std::cout << "状态: " << (status == UNREAD ? "未读" : "已读") << std::endl;
        std::cout << "时间: " << createTime << std::endl;
        std::cout << "内容: " << content << std::endl;
    }
};

#endif // NOTIFICATION_H
