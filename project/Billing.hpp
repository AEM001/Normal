#ifndef BILLING_HPP
#define BILLING_HPP

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "User.hpp"
#include "Rental.hpp"
#include "Resource.hpp"

/**
 * @class BillingRule
 * @brief 定义不同资源类型的计费规则。
 *
 * 存储计费规则的详细信息，包括资源类型、基础费率和折扣等。
 */
class BillingRule {
private:
    std::string ruleId;
    ResourceType resourceType;
    double baseHourlyRate;
    double studentDiscount; // 学生折扣率，例如0.8表示8折
    double teacherDiscount; // 教师折扣率
    double minChargeDuration; // 最小计费时长（小时），例如1表示不足1小时按1小时计
    double maxRentalDuration; // 最长租用时间（小时），例如360表示15天

public:
    // 构造函数
    BillingRule(const std::string& id, ResourceType type, double rate, 
               double sDiscount = 0.9, double tDiscount = 0.8,
               double minDuration = 1.0, double maxDuration = 360.0)
        : ruleId(id), resourceType(type), baseHourlyRate(rate),
          studentDiscount(sDiscount), teacherDiscount(tDiscount),
          minChargeDuration(minDuration), maxRentalDuration(maxDuration) {}
    
    // 默认构造函数用于反序列化
    BillingRule() : ruleId(""), resourceType(ResourceType::CPU), baseHourlyRate(0.0),
                   studentDiscount(1.0), teacherDiscount(1.0),
                   minChargeDuration(1.0), maxRentalDuration(360.0) {}

    // 获取器
    std::string getRuleId() const { return ruleId; }
    ResourceType getResourceType() const { return resourceType; }
    double getBaseHourlyRate() const { return baseHourlyRate; }
    double getStudentDiscount() const { return studentDiscount; }
    double getTeacherDiscount() const { return teacherDiscount; }
    double getMinChargeDuration() const { return minChargeDuration; }
    double getMaxRentalDuration() const { return maxRentalDuration; }

    // 设置器
    void setBaseHourlyRate(double rate) { baseHourlyRate = rate; }
    void setStudentDiscount(double discount) { studentDiscount = discount; }
    void setTeacherDiscount(double discount) { teacherDiscount = discount; }
    void setMinChargeDuration(double duration) { minChargeDuration = duration; }
    void setMaxRentalDuration(double duration) { maxRentalDuration = duration; }

    // 计算特定用户类型的实际费率
    double calculateActualRate(UserRole userRole) const {
        switch(userRole) {
            case UserRole::STUDENT:
                return baseHourlyRate * studentDiscount;
            case UserRole::TEACHER:
                return baseHourlyRate * teacherDiscount;
            default:
                return baseHourlyRate; // 管理员或其他角色使用基础费率
        }
    }

    // 序列化/反序列化方法
    void serialize(std::ostream& os) const {
        os.write(ruleId.c_str(), ruleId.size() + 1);
        os.write(reinterpret_cast<const char*>(&resourceType), sizeof(ResourceType));
        os.write(reinterpret_cast<const char*>(&baseHourlyRate), sizeof(double));
        os.write(reinterpret_cast<const char*>(&studentDiscount), sizeof(double));
        os.write(reinterpret_cast<const char*>(&teacherDiscount), sizeof(double));
        os.write(reinterpret_cast<const char*>(&minChargeDuration), sizeof(double));
        os.write(reinterpret_cast<const char*>(&maxRentalDuration), sizeof(double));
    }
    
    void deserialize(std::istream& is) {
        char buffer[256];
        is.getline(buffer, 256, '\0');
        ruleId = buffer;
        
        is.read(reinterpret_cast<char*>(&resourceType), sizeof(ResourceType));
        is.read(reinterpret_cast<char*>(&baseHourlyRate), sizeof(double));
        is.read(reinterpret_cast<char*>(&studentDiscount), sizeof(double));
        is.read(reinterpret_cast<char*>(&teacherDiscount), sizeof(double));
        is.read(reinterpret_cast<char*>(&minChargeDuration), sizeof(double));
        is.read(reinterpret_cast<char*>(&maxRentalDuration), sizeof(double));
    }
    
    // 显示计费规则详情
    void displayDetails() const {
        std::cout << "===== 计费规则详情 =====\n";
        std::cout << "规则ID: " << ruleId << "\n";
        std::cout << "资源类型: " << ResourceTypeToString(resourceType) << "\n";
        std::cout << "基础小时费率: " << baseHourlyRate << " 元/小时\n";
        std::cout << "学生折扣: " << studentDiscount * 100 << "%\n";
        std::cout << "教师折扣: " << teacherDiscount * 100 << "%\n";
        std::cout << "最小计费时长: " << minChargeDuration << " 小时\n";
        std::cout << "最长租用时间: " << maxRentalDuration << " 小时 (" << maxRentalDuration / 24 << " 天)\n";
    }
};


/**
 * @class Bill
 * @brief 表示一条账单记录。
 *
 * 存储账单的详细信息，包括关联的租赁、费用和支付状态。
 */
class Bill {
private:
    std::string billId;
    std::string rentalId;
    std::string userId;
    std::chrono::system_clock::time_point generationTime;
    double amount;
    bool paid;

public:
    // 构造函数
    Bill(const std::string& bId, const std::string& rId, const std::string& uId, double amt)
        : billId(bId), rentalId(rId), userId(uId), 
          generationTime(std::chrono::system_clock::now()),
          amount(amt), paid(false) {}
    
    // 默认构造函数用于反序列化
    Bill() : billId(""), rentalId(""), userId(""), 
             generationTime(std::chrono::system_clock::now()),
             amount(0.0), paid(false) {}

    // 获取器
    std::string getBillId() const { return billId; }
    std::string getRentalId() const { return rentalId; }
    std::string getUserId() const { return userId; }
    std::chrono::system_clock::time_point getGenerationTime() const { return generationTime; }
    double getAmount() const { return amount; }
    bool isPaid() const { return paid; }

    // 设置器
    void setPaid(bool status) { paid = status; }

    // 显示账单详情
    void displayDetails() const {
        std::cout << "===== 账单详情 =====\n";
        std::cout << "账单ID: " << billId << "\n";
        std::cout << "租赁ID: " << rentalId << "\n";
        std::cout << "用户ID: " << userId << "\n";
        
        // 格式化时间输出
        auto timeToString = [](const std::chrono::system_clock::time_point& time) -> std::string {
            auto t = std::chrono::system_clock::to_time_t(time);
            std::tm* now = std::localtime(&t);
            char buffer[128];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now);
            return buffer;
        };
        
        std::cout << "生成时间: " << timeToString(generationTime) << "\n";
        std::cout << "金额: " << amount << " 元\n";
        std::cout << "状态: " << (paid ? "已支付" : "未支付") << "\n";
    }

    // 序列化/反序列化方法
    void serialize(std::ostream& os) const {
        // 写入基本属性
        os.write(billId.c_str(), billId.size() + 1);
        os.write(rentalId.c_str(), rentalId.size() + 1);
        os.write(userId.c_str(), userId.size() + 1);
        
        // 写入时间点（转换为时间戳）
        auto timeT = std::chrono::system_clock::to_time_t(generationTime);
        os.write(reinterpret_cast<const char*>(&timeT), sizeof(time_t));
        
        // 写入金额和支付状态
        os.write(reinterpret_cast<const char*>(&amount), sizeof(double));
        os.write(reinterpret_cast<const char*>(&paid), sizeof(bool));
    }
    
    void deserialize(std::istream& is) {
        // 读取基本属性
        char buffer[256];
        is.getline(buffer, 256, '\0');
        billId = buffer;
        
        is.getline(buffer, 256, '\0');
        rentalId = buffer;
        
        is.getline(buffer, 256, '\0');
        userId = buffer;
        
        // 读取时间点（从时间戳转换）
        time_t timeT;
        is.read(reinterpret_cast<char*>(&timeT), sizeof(time_t));
        generationTime = std::chrono::system_clock::from_time_t(timeT);
        
        // 读取金额和支付状态
        is.read(reinterpret_cast<char*>(&amount), sizeof(double));
        is.read(reinterpret_cast<char*>(&paid), sizeof(bool));
    }
};

/**
 * @class BillingManager
 * @brief 管理系统中所有账单记录。
 *
 * 提供创建、查询、更新账单的功能。
 * 处理账单支付和相关业务逻辑。
 */
class BillingManager {
private:
    std::vector<std::unique_ptr<Bill>> bills;
    std::vector<BillingRule> billingRules;
    std::string dataFilePath;
    std::string rulesFilePath;
    int nextBillId;

public:
    BillingManager(const std::string& billsPath, const std::string& rulesPath) 
        : dataFilePath(billsPath), rulesFilePath(rulesPath), nextBillId(1) {}
    
    ~BillingManager() { 
        saveBills(); 
        saveBillingRules();
    }

    // 加载账单数据
    void loadBills() {
        std::ifstream file(dataFilePath, std::ios::binary);
        if (!file) {
            std::cerr << "警告: 无法打开账单数据文件，将创建新文件。\n";
            return;
        }

        // 清空当前账单记录
        bills.clear();

        // 读取下一个可用的账单ID
        file.read(reinterpret_cast<char*>(&nextBillId), sizeof(int));

        // 读取账单记录数量
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));

        // 逐个读取账单记录
        for (size_t i = 0; i < count; ++i) {
            auto bill = std::make_unique<Bill>();
            bill->deserialize(file);
            bills.push_back(std::move(bill));
        }

        file.close();
    }

    // 保存账单数据
    void saveBills() const {
        std::ofstream file(dataFilePath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行写入: " + dataFilePath);
        }

        // 写入下一个可用的账单ID
        file.write(reinterpret_cast<const char*>(&nextBillId), sizeof(int));

        // 写入账单记录数量
        size_t count = bills.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));

        // 逐个写入账单记录
        for (const auto& bill : bills) {
            bill->serialize(file);
        }

        file.close();
    }
    
    // 加载计费规则
    void loadBillingRules() {
        std::ifstream file(rulesFilePath, std::ios::binary);
        if (!file) {
            std::cerr << "警告: 无法打开计费规则数据文件，将创建默认规则。\n";
            // 创建默认规则
            createDefaultBillingRules();
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
    
    // 创建默认计费规则
    void createDefaultBillingRules() {
        billingRules.clear();
        billingRules.emplace_back("RULE_CPU", ResourceType::CPU, 10.0, 0.9, 0.8, 1.0, 360.0);
        billingRules.emplace_back("RULE_GPU", ResourceType::GPU, 20.0, 0.9, 0.8, 1.0, 360.0);
        saveBillingRules();
    }

    // 创建新的账单
    std::string createBill(const std::string& rentalId, const std::string& userId, double amount) {
        std::string billId = "B" + std::to_string(nextBillId++);
        auto bill = std::make_unique<Bill>(billId, rentalId, userId, amount);
        bills.push_back(std::move(bill));
        return billId;
    }

    // 根据ID查找账单
    Bill* findBillById(const std::string& billId) {
        for (auto& bill : bills) {
            if (bill->getBillId() == billId) {
                return bill.get();
            }
        }
        return nullptr;
    }

    // 根据租赁ID查找账单
    Bill* findBillByRentalId(const std::string& rentalId) {
        for (auto& bill : bills) {
            if (bill->getRentalId() == rentalId) {
                return bill.get();
            }
        }
        return nullptr;
    }

    // 获取用户的所有账单
    std::vector<Bill*> getUserBills(const std::string& userId) {
        std::vector<Bill*> result;
        for (auto& bill : bills) {
            if (bill->getUserId() == userId) {
                result.push_back(bill.get());
            }
        }
        return result;
    }

    // 获取用户的未支付账单
    std::vector<Bill*> getUserUnpaidBills(const std::string& userId) {
        std::vector<Bill*> result;
        for (auto& bill : bills) {
            if (bill->getUserId() == userId && !bill->isPaid()) {
                result.push_back(bill.get());
            }
        }
        return result;
    }

    // 支付账单
    bool payBill(const std::string& billId, User* user) {
        auto bill = findBillById(billId);
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
        bill->setPaid(true);
        return true;
    }

    // 计算用户未支付账单总额
    double calculateUserUnpaidTotal(const std::string& userId) {
        double total = 0.0;
        for (const auto& bill : bills) {
            if (bill->getUserId() == userId && !bill->isPaid()) {
                total += bill->getAmount();
            }
        }
        return total;
    }

    // 显示所有账单
    void displayAllBills() const {
        std::cout << "===== 所有账单记录 =====\n";
        if (bills.empty()) {
            std::cout << "暂无账单记录\n";
            return;
        }

        for (const auto& bill : bills) {
            bill->displayDetails();
            std::cout << "------------------------\n";
        }
    }

    // 显示用户的账单
    void displayUserBills(const std::string& userId) const {
        std::cout << "===== 用户 " << userId << " 的账单记录 =====\n";
        bool found = false;

        for (const auto& bill : bills) {
            if (bill->getUserId() == userId) {
                bill->displayDetails();
                std::cout << "------------------------\n";
                found = true;
            }
        }

        if (!found) {
            std::cout << "该用户暂无账单记录\n";
        }
    }
    
    // 获取特定资源类型的计费规则
    BillingRule* getBillingRuleForResourceType(ResourceType type) {
        for (auto& rule : billingRules) {
            if (rule.getResourceType() == type) {
                return &rule;
            }
        }
        return nullptr;
    }
    
    // 添加或更新计费规则
    void setResourceTypeBillingRule(ResourceType type, double baseRate, 
                                   double studentDiscount = 0.9, double teacherDiscount = 0.8) {
        // 查找是否已存在该类型的规则
        for (auto& rule : billingRules) {
            if (rule.getResourceType() == type) {
                rule.setBaseHourlyRate(baseRate);
                rule.setStudentDiscount(studentDiscount);
                rule.setTeacherDiscount(teacherDiscount);
                return;
            }
        }
        
        // 不存在则创建新规则
        std::string ruleId = "RULE_" + std::string(ResourceTypeToString(type));
        billingRules.emplace_back(ruleId, type, baseRate, studentDiscount, teacherDiscount);
    }
    
    // 显示所有计费规则
    void displayAllBillingRules() const {
        std::cout << "===== 所有计费规则 =====\n";
        if (billingRules.empty()) {
            std::cout << "暂无计费规则\n";
            return;
        }

        for (const auto& rule : billingRules) {
            rule.displayDetails();
            std::cout << "------------------------\n";
        }
    }
    
    // 获取所有计费规则
    const std::vector<BillingRule>& getAllBillingRules() const {
        return billingRules;
    }
};

#endif // BILLING_HPP
