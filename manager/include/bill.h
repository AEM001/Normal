#ifndef BILL_H
#define BILL_H

#include "entity.h"

// 账单类
class Bill : public Entity {
private:
    std::string rentalId;
    std::string userId;
    std::string resourceId;
    double amount;
    BillStatus status;

public:
    Bill() : Entity(), amount(0.0), status(UNPAID) {}
    
    Bill(const std::string& rentalId, const std::string& userId, const std::string& resourceId, double amount)
        : Entity(), rentalId(rentalId), userId(userId), resourceId(resourceId), 
          amount(amount), status(UNPAID) {}
    
    Bill(const std::string& id, const std::string& createTime,
         const std::string& rentalId, const std::string& userId, const std::string& resourceId,
         double amount, BillStatus status)
        : Entity(id, createTime), rentalId(rentalId), userId(userId), resourceId(resourceId),
          amount(amount), status(status) {}
    
    // Getters
    std::string getRentalId() const { return rentalId; }
    std::string getUserId() const { return userId; }
    std::string getResourceId() const { return resourceId; }
    double getAmount() const { return amount; }
    BillStatus getStatus() const { return status; }
    
    // Setters
    void setAmount(double amount) { this->amount = amount; }
    void setStatus(BillStatus status) { this->status = status; }
    
    // 业务方法
    bool isPaid() const { return status == PAID; }
    void markAsPaid() { status = PAID; }
    
    // 序列化和反序列化
    void serialize(std::ofstream& out) const override {
        Entity::serialize(out);
        out.write(rentalId.c_str(), rentalId.size() + 1);
        out.write(userId.c_str(), userId.size() + 1);
        out.write(resourceId.c_str(), resourceId.size() + 1);
        out.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
        out.write(reinterpret_cast<const char*>(&status), sizeof(status));
    }
    
    void deserialize(std::ifstream& in) override {
        Entity::deserialize(in);
        char buffer[256];
        
        in.getline(buffer, 256, '\0');
        rentalId = buffer;
        
        in.getline(buffer, 256, '\0');
        userId = buffer;
        
        in.getline(buffer, 256, '\0');
        resourceId = buffer;
        
        in.read(reinterpret_cast<char*>(&amount), sizeof(amount));
        in.read(reinterpret_cast<char*>(&status), sizeof(status));
    }
    
    // 显示账单信息
    void displayInfo() const {
        std::cout << "账单ID: " << id << std::endl;
        std::cout << "租用ID: " << rentalId << std::endl;
        std::cout << "用户ID: " << userId << std::endl;
        std::cout << "资源ID: " << resourceId << std::endl;
        std::cout << "金额: " << std::fixed << std::setprecision(2) << amount << std::endl;
        std::cout << "状态: " << (status == PAID ? "已支付" : "未支付") << std::endl;
        std::cout << "创建时间: " << createTime << std::endl;
    }
};

// 计费规则类
class PricingRule : public Entity {
private:
    ResourceType resourceType;
    double pricePerHour;
    int minBillingHour;
    int maxRentalDays;

public:
    PricingRule() : Entity(), pricePerHour(0.0), minBillingHour(MIN_BILLING_HOUR), maxRentalDays(MAX_RENTAL_DAYS) {}
    
    PricingRule(ResourceType resourceType, double pricePerHour, 
                int minBillingHour = MIN_BILLING_HOUR, int maxRentalDays = MAX_RENTAL_DAYS)
        : Entity(), resourceType(resourceType), pricePerHour(pricePerHour),
          minBillingHour(minBillingHour), maxRentalDays(maxRentalDays) {}
    
    PricingRule(const std::string& id, const std::string& createTime,
                ResourceType resourceType, double pricePerHour,
                int minBillingHour, int maxRentalDays)
        : Entity(id, createTime), resourceType(resourceType), pricePerHour(pricePerHour),
          minBillingHour(minBillingHour), maxRentalDays(maxRentalDays) {}
    
    // Getters
    ResourceType getResourceType() const { return resourceType; }
    double getPricePerHour() const { return pricePerHour; }
    int getMinBillingHour() const { return minBillingHour; }
    int getMaxRentalDays() const { return maxRentalDays; }
    
    // Setters
    void setPricePerHour(double price) { pricePerHour = price; }
    void setMinBillingHour(int hours) { minBillingHour = hours; }
    void setMaxRentalDays(int days) { maxRentalDays = days; }
    
    // 业务方法
    double calculateCost(int hours) const {
        if (hours < minBillingHour) {
            hours = minBillingHour;
        }
        return hours * pricePerHour;
    }
    
    // 序列化和反序列化
    void serialize(std::ofstream& out) const override {
        Entity::serialize(out);
        out.write(reinterpret_cast<const char*>(&resourceType), sizeof(resourceType));
        out.write(reinterpret_cast<const char*>(&pricePerHour), sizeof(pricePerHour));
        out.write(reinterpret_cast<const char*>(&minBillingHour), sizeof(minBillingHour));
        out.write(reinterpret_cast<const char*>(&maxRentalDays), sizeof(maxRentalDays));
    }
    
    void deserialize(std::ifstream& in) override {
        Entity::deserialize(in);
        in.read(reinterpret_cast<char*>(&resourceType), sizeof(resourceType));
        in.read(reinterpret_cast<char*>(&pricePerHour), sizeof(pricePerHour));
        in.read(reinterpret_cast<char*>(&minBillingHour), sizeof(minBillingHour));
        in.read(reinterpret_cast<char*>(&maxRentalDays), sizeof(maxRentalDays));
    }
    
    // 显示计费规则信息
    void displayInfo() const {
        std::cout << "规则ID: " << id << std::endl;
        std::cout << "资源类型: ";
        switch (resourceType) {
            case CPU: std::cout << "CPU"; break;
            case GPU: std::cout << "GPU"; break;
            case STORAGE: std::cout << "存储"; break;
        }
        std::cout << std::endl;
        std::cout << "每小时价格: " << std::fixed << std::setprecision(2) << pricePerHour << std::endl;
        std::cout << "最小计费小时: " << minBillingHour << std::endl;
        std::cout << "最长租用天数: " << maxRentalDays << std::endl;
        std::cout << "创建时间: " << createTime << std::endl;
    }
};

#endif // BILL_H
