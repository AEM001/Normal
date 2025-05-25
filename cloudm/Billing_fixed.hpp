#ifndef BILLING_HPP
#define BILLING_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <ctime>
#include <stdexcept>
#include <memory>
#include "User_fixed.hpp"
#include "Resource_fixed.hpp"
#include "Rental_fixed.hpp"

/**
 * @class BillingRule
 * @brief 表示资源计费规则。
 *
 * 包含不同资源类型的基础费率和用户角色折扣。
 */
class BillingRule {
private:
    ResourceType resourceType;
    double baseHourlyRate;
    double studentDiscount; // 0.0-1.0，表示折扣比例
    double teacherDiscount; // 0.0-1.0，表示折扣比例

public:
    // 构造函数
    BillingRule(ResourceType type, double rate, double studentDisc, double teacherDisc)
        : resourceType(type), baseHourlyRate(rate), 
          studentDiscount(studentDisc), teacherDiscount(teacherDisc) {}
    
    // 为反序列化添加默认构造函数
    BillingRule() : resourceType(ResourceType::CPU), baseHourlyRate(0.0), 
                    studentDiscount(1.0), teacherDiscount(1.0) {}
    
    // 获取器
    ResourceType getResourceType() const { return resourceType; }
    double getBaseHourlyRate() const { return baseHourlyRate; }
    double getStudentDiscount() const { return studentDiscount; }
    double getTeacherDiscount() const { return teacherDiscount; }
    
    // 设置器
    void setBaseHourlyRate(double rate) { baseHourlyRate = rate; }
    void setStudentDiscount(double discount) { studentDiscount = discount; }
    void setTeacherDiscount(double discount) { teacherDiscount = discount; }
    
    // 计算特定用户角色的实际费率
    double calculateRateForRole(UserRole role) const {
        switch (role) {
            case UserRole::STUDENT:
                return baseHourlyRate * studentDiscount;
            case UserRole::TEACHER:
                return baseHourlyRate * teacherDiscount;
            default:
                return baseHourlyRate;
        }
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
 * @brief 表示账单。
 *
 * 包含账单的基本信息和支付状态。
 */
class Bill {
private:
    std::string billId;
    std::string userId;
    std::string rentalId;
    double amount;
    bool paid;
    std::chrono::system_clock::time_point generationTime;
    std::chrono::system_clock::time_point paymentTime;

public:
    // 构造函数
    Bill(std::string id, std::string uid, std::string rid, double amt)
        : billId(id), userId(uid), rentalId(rid), amount(amt), paid(false),
          generationTime(std::chrono::system_clock::now()) {}
    
    // 为反序列化添加默认构造函数
    Bill() : billId(""), userId(""), rentalId(""), amount(0.0), paid(false) {}
    
    // 获取器
    std::string getBillId() const { return billId; }
    std::string getUserId() const { return userId; }
    std::string getRentalId() const { return rentalId; }
    double getAmount() const { return amount; }
    bool isPaid() const { return paid; }
    std::chrono::system_clock::time_point getGenerationTime() const { return generationTime; }
    std::chrono::system_clock::time_point getPaymentTime() const { return paymentTime; }
    
    // 支付账单
    void pay() {
        if (paid) {
            throw std::logic_error("账单已支付");
        }
        paid = true;
        paymentTime = std::chrono::system_clock::now();
    }
    
    // 显示账单详情
    void displayDetails() const {
        std::cout << "===== 账单详情 =====\n";
        std::cout << "账单ID: " << billId << "\n";
        std::cout << "用户ID: " << userId << "\n";
        std::cout << "租赁ID: " << rentalId << "\n";
        std::cout << "金额: " << amount << " 元\n";
        std::cout << "状态: " << (paid ? "已支付" : "未支付") << "\n";
        
        // 格式化时间
        auto genTimeT = std::chrono::system_clock::to_time_t(generationTime);
        std::tm* genTimeInfo = std::localtime(&genTimeT);
        char genTimeStr[20];
        strftime(genTimeStr, sizeof(genTimeStr), "%Y-%m-%d %H:%M", genTimeInfo);
        std::cout << "生成时间: " << genTimeStr << "\n";
        
        if (paid) {
            auto payTimeT = std::chrono::system_clock::to_time_t(paymentTime);
            std::tm* payTimeInfo = std::localtime(&payTimeT);
            char payTimeStr[20];
            strftime(payTimeStr, sizeof(payTimeStr), "%Y-%m-%d %H:%M", payTimeInfo);
            std::cout << "支付时间: " << payTimeStr << "\n";
        }
    }
    
    // 序列化/反序列化方法（用于数据持久化）
    void serialize(std::ostream& os) const {
        os.write(billId.c_str(), billId.size() + 1);
        os.write(userId.c_str(), userId.size() + 1);
        os.write(rentalId.c_str(), rentalId.size() + 1);
        os.write(reinterpret_cast<const char*>(&amount), sizeof(double));
        os.write(reinterpret_cast<const char*>(&paid), sizeof(bool));
        
        // 序列化时间点
        auto genTimeT = std::chrono::system_clock::to_time_t(generationTime);
        os.write(reinterpret_cast<const char*>(&genTimeT), sizeof(std::time_t));
        
        auto payTimeT = std::chrono::system_clock::to_time_t(paymentTime);
        os.write(reinterpret_cast<const char*>(&payTimeT), sizeof(std::time_t));
    }
    
    void deserialize(std::istream& is) {
        char buffer[256];
        is.getline(buffer, 256, '\0');
        billId = buffer;
        
        is.getline(buffer, 256, '\0');
        userId = buffer;
        
        is.getline(buffer, 256, '\0');
        rentalId = buffer;
        
        is.read(reinterpret_cast<char*>(&amount), sizeof(double));
        is.read(reinterpret_cast<char*>(&paid), sizeof(bool));
        
        // 反序列化时间点
        std::time_t genTimeT;
        is.read(reinterpret_cast<char*>(&genTimeT), sizeof(std::time_t));
        generationTime = std::chrono::system_clock::from_time_t(genTimeT);
        
        std::time_t payTimeT;
        is.read(reinterpret_cast<char*>(&payTimeT), sizeof(std::time_t));
        paymentTime = std::chrono::system_clock::from_time_t(payTimeT);
    }
};

/**
 * @class BillingManager
 * @brief 管理系统中的所有账单和计费规则。
 *
 * 负责账单的生成、支付和数据持久化。
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
        billingRules.push_back(BillingRule(ResourceType::CPU, 10.0, 0.8, 0.9));
        billingRules.push_back(BillingRule(ResourceType::GPU, 20.0, 0.8, 0.9));
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
        try {
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
        } catch (const std::exception& e) {
            std::cerr << "加载账单数据时发生错误: " << e.what() << "\n";
        }
    }

    // 保存账单数据
    void saveBills() const {
        try {
            // 确保data目录存在
            #ifdef _WIN32
                _mkdir("data");
            #else
                mkdir("data", 0777);
            #endif
            
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
        } catch (const std::exception& e) {
            std::cerr << "保存账单数据时发生错误: " << e.what() << "\n";
        }
    }

    // 加载计费规则
    void loadBillingRules() {
        try {
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
        } catch (const std::exception& e) {
            std::cerr << "加载计费规则时发生错误: " << e.what() << "\n";
            // 使用默认规则
            billingRules.clear();
            billingRules.push_back(BillingRule(ResourceType::CPU, 10.0, 0.8, 0.9));
            billingRules.push_back(BillingRule(ResourceType::GPU, 20.0, 0.8, 0.9));
        }
    }

    // 保存计费规则
    void saveBillingRules() const {
        try {
            // 确保data目录存在
            #ifdef _WIN32
                _mkdir("data");
            #else
                mkdir("data", 0777);
            #endif
            
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
        } catch (const std::exception& e) {
            std::cerr << "保存计费规则时发生错误: " << e.what() << "\n";
        }
    }

    // 生成账单
    std::string generateBill(const std::string& userId, const std::string& rentalId, 
                             const Resource* resource, double hours, UserRole userRole) {
        // 查找适用的计费规则
        const BillingRule* rule = nullptr;
        for (const auto& r : billingRules) {
            if (r.getResourceType() == resource->getResourceType()) {
                rule = &r;
                break;
            }
        }

        if (!rule) {
            throw std::runtime_error("未找到适用的计费规则");
        }

        // 计算费用
        double rate = rule->calculateRateForRole(userRole);
        double amount = rate * hours;

        // 生成账单ID
        char idStr[6];
        sprintf(idStr, "B%04d", nextBillId++);
        std::string billId = idStr;

        // 创建新账单
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

        // 检查用户余额
        if (user->getAccountBalance() < bill->getAmount()) {
            return false; // 余额不足
        }

        // 扣除用户余额
        if (!user->withdraw(bill->getAmount())) {
            return false;
        }

        // 标记账单为已支付
        bill->pay();
        return true;
    }

    // 根据ID查找账单
    Bill* findBillById(const std::string& id) const {
        for (auto bill : bills) {
            if (bill->getBillId() == id) {
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
    Bill* getRentalBill(const std::string& rentalId) const {
        for (auto bill : bills) {
            if (bill->getRentalId() == rentalId) {
                return bill;
            }
        }
        return nullptr; // 未找到账单
    }

    // 设置资源类型的计费规则
    void setResourceTypeBillingRule(ResourceType type, double baseRate, 
                                    double studentDiscount, double teacherDiscount) {
        // 查找现有规则
        for (auto& rule : billingRules) {
            if (rule.getResourceType() == type) {
                rule.setBaseHourlyRate(baseRate);
                rule.setStudentDiscount(studentDiscount);
                rule.setTeacherDiscount(teacherDiscount);
                return;
            }
        }

        // 如果没有找到现有规则，添加新规则
        billingRules.push_back(BillingRule(type, baseRate, studentDiscount, teacherDiscount));
    }

    // 显示所有计费规则
    void displayAllBillingRules() const {
        std::cout << "===== 计费规则 =====\n";
        for (const auto& rule : billingRules) {
            std::cout << "资源类型: " << ResourceTypeToString(rule.getResourceType()) << "\n";
            std::cout << "基础小时费率: " << rule.getBaseHourlyRate() << " 元/小时\n";
            std::cout << "学生折扣: " << rule.getStudentDiscount() * 100 << "%\n";
            std::cout << "教师折扣: " << rule.getTeacherDiscount() * 100 << "%\n";
            std::cout << "------------------------\n";
        }
    }
};

#endif // BILLING_HPP
