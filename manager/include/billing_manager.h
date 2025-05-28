#ifndef BILLING_MANAGER_H
#define BILLING_MANAGER_H

#include "bill.h"
#include "rental.h"
#include "resource_manager.h"
#include "user_manager.h"
#include "exception.h"
#include "data_manager.h"
#include <vector>
#include <memory>
#include <algorithm>

// 计费管理器类
class BillingManager {
private:
    std::vector<std::shared_ptr<Bill>> bills;
    std::vector<std::shared_ptr<PricingRule>> pricingRules;
    std::shared_ptr<DataManager> dataManager;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<UserManager> userManager;
    
    // 查找账单
    std::shared_ptr<Bill> findBillById(const std::string& id) {
        for (const auto& bill : bills) {
            if (bill->getId() == id) {
                return bill;
            }
        }
        return nullptr;
    }
    
    // 查找计费规则
    std::shared_ptr<PricingRule> findPricingRuleByType(ResourceType type) {
        for (const auto& rule : pricingRules) {
            if (rule->getResourceType() == type) {
                return rule;
            }
        }
        return nullptr;
    }
    
public:
    BillingManager(std::shared_ptr<DataManager> dataManager,
                  std::shared_ptr<ResourceManager> resourceManager,
                  std::shared_ptr<UserManager> userManager) 
        : dataManager(dataManager), resourceManager(resourceManager), userManager(userManager) {
        // 加载账单数据
        dataManager->loadBills(bills);
        
        // 加载计费规则
        dataManager->loadPricingRules(pricingRules);
        
        // 如果没有计费规则，创建默认规则
        if (pricingRules.empty()) {
            auto cpuRule = std::make_shared<PricingRule>(CPU, 10.0);
            auto gpuRule = std::make_shared<PricingRule>(GPU, 20.0);
            auto storageRule = std::make_shared<PricingRule>(STORAGE, 5.0);
            
            pricingRules.push_back(cpuRule);
            pricingRules.push_back(gpuRule);
            pricingRules.push_back(storageRule);
            
            savePricingRules();
        }
    }
    
    ~BillingManager() {
        saveBills();
        savePricingRules();
    }
    
    // 保存账单数据
    bool saveBills() {
        return dataManager->saveBills(bills);
    }
    
    // 保存计费规则
    bool savePricingRules() {
        return dataManager->savePricingRules(pricingRules);
    }
    
    // 生成账单
    bool generateBill(const std::shared_ptr<Rental>& rental) {
        try {
            if (rental == nullptr) {
                throw BillingException("租用记录不存在");
            }
            
            if (!rental->isCompleted()) {
                throw BillingException("只能为已完成的租用生成账单");
            }
            
            // 检查是否已经生成过账单
            for (const auto& bill : bills) {
                if (bill->getRentalId() == rental->getId()) {
                    throw BillingException("该租用已经生成过账单");
                }
            }
            
            // 获取资源
            auto resource = resourceManager->getResource(rental->getResourceId());
            if (resource == nullptr) {
                throw BillingException("资源不存在");
            }
            
            // 计算费用
            int hours = rental->calculateDuration();
            double amount = resource->getPricePerHour() * hours;
            
            // 创建账单
            auto bill = std::make_shared<Bill>(rental->getId(), rental->getUserId(), 
                                              rental->getResourceId(), amount);
            bills.push_back(bill);
            
            // 更新租用记录的总费用
            rental->setTotalCost(amount);
            
            saveBills();
            return true;
        } catch (const Exception& e) {
            std::cerr << "生成账单失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 支付账单
    bool payBill(const std::string& billId) {
        try {
            auto currentUser = userManager->getCurrentUser();
            if (currentUser == nullptr) {
                throw BillingException("未登录");
            }
            
            auto bill = findBillById(billId);
            if (bill == nullptr) {
                throw BillingException("账单不存在");
            }
            
            if (bill->getUserId() != currentUser->getId() && currentUser->getType() != ADMIN) {
                throw BillingException("没有权限支付此账单");
            }
            
            if (bill->isPaid()) {
                throw BillingException("账单已支付");
            }
            
            // 扣除用户余额
            if (!userManager->deductBalance(bill->getAmount())) {
                throw BillingException("余额不足");
            }
            
            // 标记账单为已支付
            bill->markAsPaid();
            
            saveBills();
            return true;
        } catch (const Exception& e) {
            std::cerr << "支付账单失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 设置计费规则
    bool setPricingRule(ResourceType type, double pricePerHour, 
                        int minBillingHour = MIN_BILLING_HOUR, 
                        int maxRentalDays = MAX_RENTAL_DAYS) {
        try {
            auto currentUser = userManager->getCurrentUser();
            if (currentUser == nullptr || currentUser->getType() != ADMIN) {
                throw BillingException("没有权限设置计费规则");
            }
            
            auto rule = findPricingRuleByType(type);
            if (rule != nullptr) {
                // 更新现有规则
                rule->setPricePerHour(pricePerHour);
                rule->setMinBillingHour(minBillingHour);
                rule->setMaxRentalDays(maxRentalDays);
            } else {
                // 创建新规则
                auto newRule = std::make_shared<PricingRule>(type, pricePerHour, minBillingHour, maxRentalDays);
                pricingRules.push_back(newRule);
            }
            
            savePricingRules();
            return true;
        } catch (const Exception& e) {
            std::cerr << "设置计费规则失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 获取用户的账单
    std::vector<std::shared_ptr<Bill>> getUserBills(const std::string& userId) const {
        std::vector<std::shared_ptr<Bill>> result;
        for (const auto& bill : bills) {
            if (bill->getUserId() == userId) {
                result.push_back(bill);
            }
        }
        return result;
    }
    
    // 获取所有账单
    std::vector<std::shared_ptr<Bill>> getAllBills() const {
        return bills;
    }
    
    // 获取未支付的账单
    std::vector<std::shared_ptr<Bill>> getUnpaidBills() const {
        std::vector<std::shared_ptr<Bill>> result;
        for (const auto& bill : bills) {
            if (!bill->isPaid()) {
                result.push_back(bill);
            }
        }
        return result;
    }
    
    // 获取计费规则
    std::vector<std::shared_ptr<PricingRule>> getAllPricingRules() const {
        return pricingRules;
    }
    
    // 获取特定类型的计费规则
    std::shared_ptr<PricingRule> getPricingRule(ResourceType type) {
        return findPricingRuleByType(type);
    }
    
    // 计算租用费用
    double calculateRentalCost(const std::shared_ptr<Rental>& rental) {
        try {
            if (rental == nullptr) {
                throw BillingException("租用记录不存在");
            }
            
            // 获取资源
            auto resource = resourceManager->getResource(rental->getResourceId());
            if (resource == nullptr) {
                throw BillingException("资源不存在");
            }
            
            // 计算费用
            int hours = rental->calculateDuration();
            return resource->getPricePerHour() * hours;
        } catch (const Exception& e) {
            std::cerr << "计算租用费用失败: " << e.what() << std::endl;
            return 0.0;
        }
    }
};

#endif // BILLING_MANAGER_H
