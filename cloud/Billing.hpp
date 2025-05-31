#ifndef BILLING_HPP
#define BILLING_HPP

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include "User.hpp"
#include "Resource.hpp"
#include "Rental.hpp"

// 账单状态枚举
enum class BillStatus {
    UNPAID,
    PAID,
    OVERDUE
};

// 枚举转字符串函数
inline const char* BillStatusToString(BillStatus status) {
    switch(status) {
        case BillStatus::UNPAID: return "未支付";
        case BillStatus::PAID: return "已支付";
        case BillStatus::OVERDUE: return "已逾期";
        default: return "未知状态";
    }
}

/**
 * @class BillingRule
 * @brief 表示计费规则。
 *
 * 包含资源类型、基础费率和不同用户角色的折扣。
 */
class BillingRule {
public:
    ResourceType resourceType;
    double baseHourlyRate;
    double studentDiscount; // 0.0-1.0
    double teacherDiscount; // 0.0-1.0

    // 构造函数
    BillingRule(ResourceType type, double rate, double stuDiscount = 0.8, double tchDiscount = 0.9)
        : resourceType(type), baseHourlyRate(rate), 
          studentDiscount(stuDiscount), teacherDiscount(tchDiscount) {}
    
    // 为反序列化添加默认构造函数
    BillingRule() : resourceType(ResourceType::CPU), baseHourlyRate(0.0), 
                    studentDiscount(1.0), teacherDiscount(1.0) {}

    // 根据用户角色计算实际费率
    double calculateActualRate(UserRole role) const {
        switch(role) {
            case UserRole::STUDENT:
                return baseHourlyRate * studentDiscount;
            case UserRole::TEACHER:
                return baseHourlyRate * teacherDiscount;
            default:
                return baseHourlyRate;
        }
    }

    // 显示计费规则详情
    void displayDetails() const {
        std::cout << "资源类型: " << ResourceTypeToString(resourceType) << "\n";
        std::cout << "基础小时费率: " << baseHourlyRate << " 元/小时\n";
        std::cout << "学生折扣: " << studentDiscount * 100 << "%\n";
        std::cout << "教师折扣: " << teacherDiscount * 100 << "%\n";
    }

    // 序列化/反序列化方法（用于数据持久化）
    void serialize(std::ostream& os) const {
        os.write(reinterpret_cast<const char*>(&resourceType), sizeof(ResourceType));
        os.write(reinterpret_cast<const char*>(&baseHourlyRate), sizeof(double));
        os.write(reinterpret_cast<const char*>(&studentDiscount), sizeof(double));
        os.write(reinterpret_cast<const char*>(&teacherDiscount), sizeof(double));
    }
    
    void deserialize(std::istream& is) {
        is.read(reinterpret_cast<char*>(&resourceType), sizeof(ResourceType));
        is.read(reinterpret_cast<char*>(&baseHourlyRate), sizeof(double));
        is.read(reinterpret_cast<char*>(&studentDiscount), sizeof(double));
        is.read(reinterpret_cast<char*>(&teacherDiscount), sizeof(double));
    }
};

/**
 * @class Bill
 * @brief 表示一条账单记录。
 *
 * 包含账单的所有信息，如ID、用户ID、租赁ID、金额、状态等。
 */
class Bill {
private:
    std::string billId;
    std::string userId;
    std::string rentalId;
    double amount;
    BillStatus status;
    std::chrono::system_clock::time_point creationTime;
    std::chrono::system_clock::time_point paymentTime;

public:
    // 构造函数
    Bill(std::string id, std::string user, std::string rental, double amt)
        : billId(id), userId(user), rentalId(rental), amount(amt), 
          status(BillStatus::UNPAID), creationTime(std::chrono::system_clock::now()) {}
    
    // 为反序列化添加默认构造函数
    Bill() : billId(""), userId(""), rentalId(""), amount(0.0), 
             status(BillStatus::UNPAID) {}

    // 获取器
    std::string getBillId() const { return billId; }
    std::string getUserId() const { return userId; }
    std::string getRentalId() const { return rentalId; }
    double getAmount() const { return amount; }
    BillStatus getStatus() const { return status; }
    std::chrono::system_clock::time_point getCreationTime() const { return creationTime; }
    std::chrono::system_clock::time_point getPaymentTime() const { return paymentTime; }

    // 检查账单是否已支付
    bool isPaid() const { return status == BillStatus::PAID; }

    // 支付账单
    void pay() {
        if (status == BillStatus::PAID) {
            throw std::runtime_error("账单已支付");
        }
        status = BillStatus::PAID;
        paymentTime = std::chrono::system_clock::now();
    }

    // 标记为逾期
    void markAsOverdue() {
        if (status == BillStatus::PAID) {
            throw std::runtime_error("已支付的账单不能标记为逾期");
        }
        status = BillStatus::OVERDUE;
    }

    // 显示账单详情
    void displayDetails() const {
        std::cout << "===== 账单详情 =====\n";
        std::cout << "账单ID: " << billId << "\n";
        std::cout << "用户ID: " << userId << "\n";
        std::cout << "租赁ID: " << rentalId << "\n";
        std::cout << "金额: " << std::fixed << std::setprecision(2) << amount << " 元\n";
        std::cout << "状态: " << BillStatusToString(status) << "\n";
        
        // 显示创建时间
        auto creationTimeT = std::chrono::system_clock::to_time_t(creationTime);
        std::cout << "创建时间: " << std::ctime(&creationTimeT);
        
        // 显示支付时间（如果已支付）
        if (status == BillStatus::PAID) {
            auto paymentTimeT = std::chrono::system_clock::to_time_t(paymentTime);
            std::cout << "支付时间: " << std::ctime(&paymentTimeT);
        }
    }

    // 序列化/反序列化方法（用于数据持久化）
    void serialize(std::ostream& os) const {
        // 写入基本属性
        os.write(billId.c_str(), billId.size() + 1);
        os.write(userId.c_str(), userId.size() + 1);
        os.write(rentalId.c_str(), rentalId.size() + 1);
        os.write(reinterpret_cast<const char*>(&amount), sizeof(double));
        os.write(reinterpret_cast<const char*>(&status), sizeof(BillStatus));
        
        // 写入时间点
        auto creationTimeT = std::chrono::system_clock::to_time_t(creationTime);
        os.write(reinterpret_cast<const char*>(&creationTimeT), sizeof(time_t));
        
        auto paymentTimeT = std::chrono::system_clock::to_time_t(paymentTime);
        os.write(reinterpret_cast<const char*>(&paymentTimeT), sizeof(time_t));
    }
    
    void deserialize(std::istream& is) {
        // 读取基本属性
        char buffer[256];
        is.getline(buffer, 256, '\0');
        billId = buffer;
        
        is.getline(buffer, 256, '\0');
        userId = buffer;
        
        is.getline(buffer, 256, '\0');
        rentalId = buffer;
        
        is.read(reinterpret_cast<char*>(&amount), sizeof(double));
        is.read(reinterpret_cast<char*>(&status), sizeof(BillStatus));
        
        // 读取时间点
        time_t creationTimeT;
        is.read(reinterpret_cast<char*>(&creationTimeT), sizeof(time_t));
        creationTime = std::chrono::system_clock::from_time_t(creationTimeT);
        
        time_t paymentTimeT;
        is.read(reinterpret_cast<char*>(&paymentTimeT), sizeof(time_t));
        paymentTime = std::chrono::system_clock::from_time_t(paymentTimeT);
    }
};

/**
 * @class BillingManager
 * @brief 管理系统中的所有账单和计费规则。
 *
 * 负责账单的创建、查找、支付和数据持久化。
 * 管理不同资源类型的计费规则。
 */
class BillingManager {
private:
    std::vector<Bill*> bills;
    std::vector<BillingRule> billingRules;
    std::string billsFilePath; // 账单数据文件路径
    std::string rulesFilePath; // 计费规则数据文件路径
    int nextBillId;

public:
    BillingManager(const std::string& billsPath, const std::string& rulesPath) 
        : billsFilePath(billsPath), rulesFilePath(rulesPath), nextBillId(1) {
        // 初始化默认计费规则
        if (billingRules.empty()) {
            billingRules.push_back(BillingRule(ResourceType::CPU, 10.0, 0.8, 0.9));
            billingRules.push_back(BillingRule(ResourceType::GPU, 20.0, 0.8, 0.9));
        }
    }
    
    ~BillingManager() { 
        saveBills(); 
        saveBillingRules();
        for (auto bill : bills) {
            delete bill;
        }
        bills.clear();
    }

    // 加载账单数据
    void loadBills() {
        std::ifstream file(billsFilePath, std::ios::binary);
        if (!file) {
            std::cerr << "警告: 无法打开账单数据文件，将创建新文件。\n";
            return;
        }

        // 清空当前账单
        for (auto bill : bills) {
            delete bill;
        }
        bills.clear();

        // 读取下一个可用的账单ID
        file.read(reinterpret_cast<char*>(&nextBillId), sizeof(int));

        // 读取账单数量
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));

        // 逐个读取账单
        for (size_t i = 0; i < count; ++i) {
            Bill* bill = new Bill();
            bill->deserialize(file);
            bills.push_back(bill);
        }

        file.close();
    }

    // 保存账单数据
    void saveBills() const {
        std::ofstream file(billsFilePath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行写入: " + billsFilePath);
        }

        // 写入下一个可用的账单ID
        file.write(reinterpret_cast<const char*>(&nextBillId), sizeof(int));

        // 写入账单数量
        size_t count = bills.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));

        // 逐个写入账单
        for (auto bill : bills) {
            bill->serialize(file);
        }

        file.close();
    }

    // 加载计费规则
    void loadBillingRules() {
        std::ifstream file(rulesFilePath, std::ios::binary);
        if (!file) {
            std::cerr << "警告: 无法打开计费规则数据文件，将使用默认规则。\n";
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
    }

    // 保存计费规则
    void saveBillingRules() const {
        std::ofstream file(rulesFilePath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行写入: " + rulesFilePath);
        }

        // 写入规则数量
        size_t count = billingRules.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));

        // 逐个写入规则
        for (const auto& rule : billingRules) {
            rule.serialize(file);
        }

        file.close();
    }

    // 创建新的账单
    std::string createBill(const std::string& rentalId, const std::string& userId, double amount) {
        std::string billId = generateUniqueBillId();
        Bill* bill = new Bill(billId, userId, rentalId, amount);
        bills.push_back(bill);
        return billId;
    }

    // 支付账单
    bool payBill(const std::string& billId, User* user) {
        Bill* bill = findBillById(billId);
        if (!bill || bill->isPaid() || bill->getUserId() != user->getUserId()) {
            return false;
        }
        
        // 检查用户余额是否足够
        if (user->getAccountBalance() < bill->getAmount()) {
            return false;
        }
        
        // 扣除用户余额
        if (!user->withdraw(bill->getAmount())) {
            return false;
        }
        
        // 标记账单为已支付
        try {
            bill->pay();
            return true;
        } catch (const std::exception& e) {
            // 支付失败，退还用户余额
            user->deposit(bill->getAmount());
            std::cerr << "支付账单失败: " << e.what() << std::endl;
            return false;
        }
    }

    // 根据ID查找账单
    Bill* findBillById(const std::string& billId) {
        for (auto bill : bills) {
            if (bill->getBillId() == billId) {
                return bill;
            }
        }
        return nullptr; // 未找到账单
    }

    const Bill* findBillById(const std::string& billId) const {
        for (auto bill : bills) {
            if (bill->getBillId() == billId) {
                return bill;
            }
        }
        return nullptr; // 未找到账单
    }

    // 获取用户的所有账单
    std::vector<Bill*> getUserBills(const std::string& userId) const {
        std::vector<Bill*> result;
        for (auto bill : bills) {
            if (bill->getUserId() == userId) {
                result.push_back(bill);
            }
        }
        return result;
    }

    // 获取租赁的账单
    Bill* getRentalBill(const std::string& rentalId) {
        for (auto bill : bills) {
            if (bill->getRentalId() == rentalId) {
                return bill;
            }
        }
        return nullptr; // 未找到账单
    }

    const Bill* getRentalBill(const std::string& rentalId) const {
        for (auto bill : bills) {
            if (bill->getRentalId() == rentalId) {
                return bill;
            }
        }
        return nullptr; // 未找到账单
    }

    // 获取所有未支付的账单
    std::vector<Bill*> getUnpaidBills() const {
        std::vector<Bill*> result;
        for (auto bill : bills) {
            if (!bill->isPaid()) {
                result.push_back(bill);
            }
        }
        return result;
    }

    // 获取资源类型的计费规则
    const BillingRule* getResourceTypeBillingRule(ResourceType type) const {
        for (const auto& rule : billingRules) {
            if (rule.resourceType == type) {
                return &rule;
            }
        }
        return nullptr; // 未找到规则
    }

    // 设置资源类型的计费规则
    void setResourceTypeBillingRule(ResourceType type, double baseRate, double studentDiscount, double teacherDiscount) {
        // 检查是否已存在该类型的规则
        for (auto& rule : billingRules) {
            if (rule.resourceType == type) {
                rule.baseHourlyRate = baseRate;
                rule.studentDiscount = studentDiscount;
                rule.teacherDiscount = teacherDiscount;
                return;
            }
        }
        
        // 不存在则添加新规则
        billingRules.push_back(BillingRule(type, baseRate, studentDiscount, teacherDiscount));
    }

    // 计算租赁费用
    double calculateRentalCost(const RentalRecord* rental, const Resource* resource, const User* user) const {
        // 获取资源类型的计费规则
        const BillingRule* rule = getResourceTypeBillingRule(resource->getResourceType());
        if (!rule) {
            // 如果没有找到规则，使用资源的小时费率
            return rental->calculateDurationHours() * resource->getHourlyRate();
        }
        
        // 根据用户角色计算实际费率
        double actualRate = rule->calculateActualRate(user->getRole());
        
        // 计算总费用
        return rental->calculateDurationHours() * actualRate;
    }

    // 显示所有账单
    void displayAllBills() const {
        if (bills.empty()) {
            std::cout << "当前没有账单记录。\n";
            return;
        }
        
        for (auto bill : bills) {
            bill->displayDetails();
            std::cout << "------------------------\n";
        }
    }

    // 显示所有计费规则
    void displayAllBillingRules() const {
        std::cout << "===== 计费规则 =====\n";
        for (const auto& rule : billingRules) {
            rule.displayDetails();
            std::cout << "------------------------\n";
        }
    }

private:
    // 生成唯一的账单ID
    std::string generateUniqueBillId() {
        std::string id = "BILL" + std::to_string(nextBillId++);
        return id;
    }
};

#endif // BILLING_HPP
