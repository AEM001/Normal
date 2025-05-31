#ifndef RENTAL_MANAGER_H
#define RENTAL_MANAGER_H

#include "rental.h"
#include "resource_manager.h"
#include "user_manager.h"
#include "exception.h"
#include "data_manager.h"
#include <vector>
#include <memory>
#include <algorithm>

// 租用管理器类
class RentalManager {
private:
    std::vector<std::shared_ptr<Rental>> rentals;
    std::shared_ptr<DataManager> dataManager;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<UserManager> userManager;
    
    // 查找租用记录
    std::shared_ptr<Rental> findRentalById(const std::string& id) {
        for (const auto& rental : rentals) {
            if (rental->getId() == id) {
                return rental;
            }
        }
        return nullptr;
    }
    
public:
    RentalManager(std::shared_ptr<DataManager> dataManager,
                 std::shared_ptr<ResourceManager> resourceManager,
                 std::shared_ptr<UserManager> userManager) 
        : dataManager(dataManager), resourceManager(resourceManager), userManager(userManager) {
        // 加载租用数据
        dataManager->loadRentals(rentals);
    }
    
    ~RentalManager() {
        saveRentals();
    }
    
    // 保存租用数据
    bool saveRentals() {
        return dataManager->saveRentals(rentals);
    }
    
    // 申请租用
    bool applyRental(const std::string& resourceId, const std::string& startTime, const std::string& endTime) {
        try {
            auto currentUser = userManager->getCurrentUser();
            if (currentUser == nullptr) {
                throw RentalException("未登录");
            }
            
            if (!currentUser->isActive()) {
                throw RentalException("账号已被暂停，无法申请租用");
            }
            
            auto resource = resourceManager->getResource(resourceId);
            if (resource == nullptr) {
                throw RentalException("资源不存在");
            }
            
            if (!resource->isIdle()) {
                throw RentalException("资源不可用");
            }
            
            // 创建租用记录
            auto rental = std::make_shared<Rental>(currentUser->getId(), resourceId, startTime, endTime);
            rentals.push_back(rental);
            saveRentals();
            return true;
        } catch (const Exception& e) {
            std::cerr << "申请租用失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 取消租用
    bool cancelRental(const std::string& rentalId) {
        try {
            auto currentUser = userManager->getCurrentUser();
            if (currentUser == nullptr) {
                throw RentalException("未登录");
            }
            
            auto rental = findRentalById(rentalId);
            if (rental == nullptr) {
                throw RentalException("租用记录不存在");
            }
            
            if (rental->getUserId() != currentUser->getId() && currentUser->getType() != ADMIN) {
                throw RentalException("没有权限取消此租用");
            }
            
            if (!rental->isPending() && !rental->isApproved()) {
                throw RentalException("只能取消申请中或已审核通过的租用");
            }
            
            rental->setStatus(CANCELLED);
            saveRentals();
            return true;
        } catch (const Exception& e) {
            std::cerr << "取消租用失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 审核租用申请
    bool reviewRental(const std::string& rentalId, bool approved, const std::string& comment) {
        try {
            auto currentUser = userManager->getCurrentUser();
            if (currentUser == nullptr || currentUser->getType() != ADMIN) {
                throw RentalException("没有权限审核租用申请");
            }
            
            auto rental = findRentalById(rentalId);
            if (rental == nullptr) {
                throw RentalException("租用记录不存在");
            }
            
            if (!rental->isPending()) {
                throw RentalException("只能审核申请中的租用");
            }
            
            if (approved) {
                // 检查资源是否可用
                auto resource = resourceManager->getResource(rental->getResourceId());
                if (resource == nullptr) {
                    throw RentalException("资源不存在");
                }
                
                if (!resource->isIdle()) {
                    throw RentalException("资源不可用");
                }
                
                // 检查用户是否有足够余额
                auto user = userManager->getCurrentUser();
                if (user == nullptr) {
                    throw RentalException("用户不存在");
                }
                
                // 更新租用状态
                rental->setStatus(APPROVED);
                rental->setReviewComment(comment);
                
                // 更新资源状态
                resourceManager->updateResourceStatus(rental->getResourceId(), IN_USE);
            } else {
                // 拒绝申请
                rental->setStatus(REJECTED);
                rental->setReviewComment(comment);
            }
            
            saveRentals();
            return true;
        } catch (const Exception& e) {
            std::cerr << "审核租用申请失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 开始使用资源
    bool startRental(const std::string& rentalId) {
        try {
            auto currentUser = userManager->getCurrentUser();
            if (currentUser == nullptr) {
                throw RentalException("未登录");
            }
            
            auto rental = findRentalById(rentalId);
            if (rental == nullptr) {
                throw RentalException("租用记录不存在");
            }
            
            if (rental->getUserId() != currentUser->getId() && currentUser->getType() != ADMIN) {
                throw RentalException("没有权限操作此租用");
            }
            
            if (!rental->isApproved()) {
                throw RentalException("只能开始已审核通过的租用");
            }
            
            rental->setStatus(USING);
            saveRentals();
            return true;
        } catch (const Exception& e) {
            std::cerr << "开始使用资源失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 完成租用
    bool completeRental(const std::string& rentalId) {
        try {
            auto currentUser = userManager->getCurrentUser();
            if (currentUser == nullptr) {
                throw RentalException("未登录");
            }
            
            auto rental = findRentalById(rentalId);
            if (rental == nullptr) {
                throw RentalException("租用记录不存在");
            }
            
            if (rental->getUserId() != currentUser->getId() && currentUser->getType() != ADMIN) {
                throw RentalException("没有权限操作此租用");
            }
            
            if (!rental->isUsing()) {
                throw RentalException("只能完成使用中的租用");
            }
            
            // 更新租用状态
            rental->setStatus(COMPLETED);
            
            // 更新资源状态
            resourceManager->updateResourceStatus(rental->getResourceId(), IDLE);
            
            saveRentals();
            return true;
        } catch (const Exception& e) {
            std::cerr << "完成租用失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 获取用户的租用记录
    std::vector<std::shared_ptr<Rental>> getUserRentals(const std::string& userId) const {
        std::vector<std::shared_ptr<Rental>> result;
        for (const auto& rental : rentals) {
            if (rental->getUserId() == userId) {
                result.push_back(rental);
            }
        }
        return result;
    }
    
    // 获取资源的租用记录
    std::vector<std::shared_ptr<Rental>> getResourceRentals(const std::string& resourceId) const {
        std::vector<std::shared_ptr<Rental>> result;
        for (const auto& rental : rentals) {
            if (rental->getResourceId() == resourceId) {
                result.push_back(rental);
            }
        }
        return result;
    }
    
    // 获取所有租用记录
    std::vector<std::shared_ptr<Rental>> getAllRentals() const {
        return rentals;
    }
    
    // 获取待审核的租用记录
    std::vector<std::shared_ptr<Rental>> getPendingRentals() const {
        std::vector<std::shared_ptr<Rental>> result;
        for (const auto& rental : rentals) {
            if (rental->isPending()) {
                result.push_back(rental);
            }
        }
        return result;
    }
    
    // 获取租用记录
    std::shared_ptr<Rental> getRental(const std::string& id) {
        return findRentalById(id);
    }
};

#endif // RENTAL_MANAGER_H
