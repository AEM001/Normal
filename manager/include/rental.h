#ifndef RENTAL_H
#define RENTAL_H

#include "entity.h"
#include <ctime>
#include <cmath>

// 租用记录类
class Rental : public Entity {
private:
    std::string userId;
    std::string resourceId;
    std::string startTime;
    std::string endTime;
    RentalStatus status;
    std::string reviewComment;
    double totalCost;

public:
    Rental() : Entity(), status(PENDING), totalCost(0.0) {}
    
    Rental(const std::string& userId, const std::string& resourceId, 
           const std::string& startTime, const std::string& endTime)
        : Entity(), userId(userId), resourceId(resourceId), 
          startTime(startTime), endTime(endTime), 
          status(PENDING), reviewComment(""), totalCost(0.0) {}
    
    Rental(const std::string& id, const std::string& createTime,
           const std::string& userId, const std::string& resourceId,
           const std::string& startTime, const std::string& endTime,
           RentalStatus status, const std::string& reviewComment, double totalCost)
        : Entity(id, createTime), userId(userId), resourceId(resourceId),
          startTime(startTime), endTime(endTime), 
          status(status), reviewComment(reviewComment), totalCost(totalCost) {}
    
    // Getters
    std::string getUserId() const { return userId; }
    std::string getResourceId() const { return resourceId; }
    std::string getStartTime() const { return startTime; }
    std::string getEndTime() const { return endTime; }
    RentalStatus getStatus() const { return status; }
    std::string getReviewComment() const { return reviewComment; }
    double getTotalCost() const { return totalCost; }
    
    // Setters
    void setStartTime(const std::string& startTime) { this->startTime = startTime; }
    void setEndTime(const std::string& endTime) { this->endTime = endTime; }
    void setStatus(RentalStatus status) { this->status = status; }
    void setReviewComment(const std::string& comment) { this->reviewComment = comment; }
    void setTotalCost(double cost) { this->totalCost = cost; }
    
    // 业务方法
    bool isPending() const { return status == PENDING; }
    bool isApproved() const { return status == APPROVED; }
    bool isRejected() const { return status == REJECTED; }
    bool isUsing() const { return status == USING; }
    bool isCompleted() const { return status == COMPLETED; }
    bool isCancelled() const { return status == CANCELLED; }
    
    // 计算租用时长（小时）
    int calculateDuration() const {
        struct tm start_tm = {0}, end_tm = {0};
        
        // 解析时间字符串
        sscanf(startTime.c_str(), "%d-%d-%d %d:%d:%d", 
               &start_tm.tm_year, &start_tm.tm_mon, &start_tm.tm_mday, 
               &start_tm.tm_hour, &start_tm.tm_min, &start_tm.tm_sec);
        sscanf(endTime.c_str(), "%d-%d-%d %d:%d:%d", 
               &end_tm.tm_year, &end_tm.tm_mon, &end_tm.tm_mday, 
               &end_tm.tm_hour, &end_tm.tm_min, &end_tm.tm_sec);
        
        // 调整年月
        start_tm.tm_year -= 1900;
        start_tm.tm_mon -= 1;
        end_tm.tm_year -= 1900;
        end_tm.tm_mon -= 1;
        
        // 转换为时间戳
        time_t start_time = mktime(&start_tm);
        time_t end_time = mktime(&end_tm);
        
        // 计算差值（秒）
        double diff_seconds = difftime(end_time, start_time);
        
        // 转换为小时，向上取整
        int hours = static_cast<int>(ceil(diff_seconds / 3600.0));
        
        // 最小计费单位为1小时
        return (hours < MIN_BILLING_HOUR) ? MIN_BILLING_HOUR : hours;
    }
    
    // 序列化和反序列化
    void serialize(std::ofstream& out) const override {
        Entity::serialize(out);
        out.write(userId.c_str(), userId.size() + 1);
        out.write(resourceId.c_str(), resourceId.size() + 1);
        out.write(startTime.c_str(), startTime.size() + 1);
        out.write(endTime.c_str(), endTime.size() + 1);
        out.write(reinterpret_cast<const char*>(&status), sizeof(status));
        out.write(reviewComment.c_str(), reviewComment.size() + 1);
        out.write(reinterpret_cast<const char*>(&totalCost), sizeof(totalCost));
    }
    
    void deserialize(std::ifstream& in) override {
        Entity::deserialize(in);
        char buffer[256];
        
        in.getline(buffer, 256, '\0');
        userId = buffer;
        
        in.getline(buffer, 256, '\0');
        resourceId = buffer;
        
        in.getline(buffer, 256, '\0');
        startTime = buffer;
        
        in.getline(buffer, 256, '\0');
        endTime = buffer;
        
        in.read(reinterpret_cast<char*>(&status), sizeof(status));
        
        in.getline(buffer, 256, '\0');
        reviewComment = buffer;
        
        in.read(reinterpret_cast<char*>(&totalCost), sizeof(totalCost));
    }
    
    // 显示租用信息
    void displayInfo() const {
        std::cout << "租用ID: " << id << std::endl;
        std::cout << "用户ID: " << userId << std::endl;
        std::cout << "资源ID: " << resourceId << std::endl;
        std::cout << "开始时间: " << startTime << std::endl;
        std::cout << "结束时间: " << endTime << std::endl;
        std::cout << "状态: ";
        switch (status) {
            case PENDING: std::cout << "申请中"; break;
            case APPROVED: std::cout << "已审核通过"; break;
            case REJECTED: std::cout << "已拒绝"; break;
            case USING: std::cout << "使用中"; break;
            case COMPLETED: std::cout << "已完成"; break;
            case CANCELLED: std::cout << "已取消"; break;
        }
        std::cout << std::endl;
        
        if (!reviewComment.empty()) {
            std::cout << "审核备注: " << reviewComment << std::endl;
        }
        
        if (totalCost > 0) {
            std::cout << "总费用: " << std::fixed << std::setprecision(2) << totalCost << std::endl;
        }
        
        std::cout << "创建时间: " << createTime << std::endl;
    }
};

#endif // RENTAL_H
