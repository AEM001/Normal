#ifndef RENTAL_HPP
#define RENTAL_HPP

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "User.hpp"
#include "Resource.hpp"

// 租赁状态枚举
enum class RentalStatus {
    PENDING,       // 申请中，等待管理员审核
    APPROVED,      // 已批准，等待开始使用
    ACTIVE,        // 使用中
    COMPLETED,     // 已完成
    REJECTED,      // 被管理员拒绝
    CANCELLED      // 被用户取消
};

// 将枚举转换为字符串的辅助函数
inline const char* RentalStatusToString(RentalStatus status) {
    switch(status) {
        case RentalStatus::PENDING: return "申请中";
        case RentalStatus::APPROVED: return "已批准";
        case RentalStatus::ACTIVE: return "使用中";
        case RentalStatus::COMPLETED: return "已完成";
        case RentalStatus::REJECTED: return "已拒绝";
        case RentalStatus::CANCELLED: return "已取消";
        default: return "未知状态";
    }
}

/**
 * @class RentalRecord
 * @brief 表示一条租赁记录。
 *
 * 存储租赁的详细信息，包括用户、资源、时间和状态。
 * 提供计算费用和管理租赁状态的功能。
 */
class RentalRecord {
private:
    std::string rentalId;
    std::string userId;
    std::string resourceId;
    std::chrono::system_clock::time_point requestTime;
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point endTime;
    RentalStatus status;
    double totalCost;
    std::string adminComment;  // 管理员审核备注

public:
    // 构造函数
    RentalRecord(const std::string& rId, const std::string& uId, const std::string& resId)
        : rentalId(rId), userId(uId), resourceId(resId),
          requestTime(std::chrono::system_clock::now()),
          status(RentalStatus::PENDING), totalCost(0.0) {}
    
    // 默认构造函数用于反序列化
    RentalRecord() : rentalId(""), userId(""), resourceId(""),
                     requestTime(std::chrono::system_clock::now()),
                     status(RentalStatus::PENDING), totalCost(0.0) {}

    // 获取器
    std::string getRentalId() const { return rentalId; }
    std::string getUserId() const { return userId; }
    std::string getResourceId() const { return resourceId; }
    RentalStatus getStatus() const { return status; }
    std::chrono::system_clock::time_point getRequestTime() const { return requestTime; }
    std::chrono::system_clock::time_point getStartTime() const { return startTime; }
    std::chrono::system_clock::time_point getEndTime() const { return endTime; }
    double getTotalCost() const { return totalCost; }
    std::string getAdminComment() const { return adminComment; }

    // 设置器
    void setStatus(RentalStatus newStatus) { status = newStatus; }
    void setStartTime(const std::chrono::system_clock::time_point& time) { startTime = time; }
    void setEndTime(const std::chrono::system_clock::time_point& time) { endTime = time; }
    void setTotalCost(double cost) { totalCost = cost; }
    void setAdminComment(const std::string& comment) { adminComment = comment; }

    // 计算租赁时长（小时）
    double calculateDurationHours() const {
        if (status != RentalStatus::COMPLETED) {
            return 0.0;
        }
        
        auto duration = std::chrono::duration_cast<std::chrono::hours>(endTime - startTime);
        double hours = duration.count();
        
        // 不足1小时按1小时计算
        return hours < 1.0 ? 1.0 : hours;
    }

    // 计算租赁费用
    double calculateCost(double hourlyRate) {
        double hours = calculateDurationHours();
        totalCost = hours * hourlyRate;
        return totalCost;
    }

    // 验证租赁时长是否符合规定（最长15天）
    bool validateRentalDuration(const std::chrono::system_clock::time_point& proposedEndTime) const {
        auto maxDuration = std::chrono::hours(24 * 15); // 15天
        auto proposedDuration = std::chrono::duration_cast<std::chrono::hours>(proposedEndTime - startTime);
        return proposedDuration <= maxDuration;
    }

    // 显示租赁详情
    void displayDetails() const {
        std::cout << "===== 租赁记录详情 =====\n";
        std::cout << "租赁ID: " << rentalId << "\n";
        std::cout << "用户ID: " << userId << "\n";
        std::cout << "资源ID: " << resourceId << "\n";
        std::cout << "状态: " << RentalStatusToString(status) << "\n";
        
        // 格式化时间输出
        auto timeToString = [](const std::chrono::system_clock::time_point& time) -> std::string {
            auto t = std::chrono::system_clock::to_time_t(time);
            std::tm* now = std::localtime(&t);
            char buffer[128];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now);
            return buffer;
        };
        
        std::cout << "申请时间: " << timeToString(requestTime) << "\n";
        
        if (status == RentalStatus::APPROVED || status == RentalStatus::ACTIVE || 
            status == RentalStatus::COMPLETED) {
            std::cout << "开始时间: " << timeToString(startTime) << "\n";
        }
        
        if (status == RentalStatus::COMPLETED) {
            std::cout << "结束时间: " << timeToString(endTime) << "\n";
            std::cout << "使用时长: " << calculateDurationHours() << " 小时\n";
            std::cout << "总费用: " << totalCost << " 元\n";
        }
        
        if (!adminComment.empty()) {
            std::cout << "管理员备注: " << adminComment << "\n";
        }
    }

    // 序列化/反序列化方法
    void serialize(std::ostream& os) const {
        // 写入基本属性
        os.write(rentalId.c_str(), rentalId.size() + 1);
        os.write(userId.c_str(), userId.size() + 1);
        os.write(resourceId.c_str(), resourceId.size() + 1);
        
        // 写入时间点（转换为时间戳）
        auto requestTimeT = std::chrono::system_clock::to_time_t(requestTime);
        os.write(reinterpret_cast<const char*>(&requestTimeT), sizeof(time_t));
        
        auto startTimeT = std::chrono::system_clock::to_time_t(startTime);
        os.write(reinterpret_cast<const char*>(&startTimeT), sizeof(time_t));
        
        auto endTimeT = std::chrono::system_clock::to_time_t(endTime);
        os.write(reinterpret_cast<const char*>(&endTimeT), sizeof(time_t));
        
        // 写入状态和费用
        os.write(reinterpret_cast<const char*>(&status), sizeof(RentalStatus));
        os.write(reinterpret_cast<const char*>(&totalCost), sizeof(double));
        
        // 写入管理员备注
        os.write(adminComment.c_str(), adminComment.size() + 1);
    }
    
    void deserialize(std::istream& is) {
        // 读取基本属性
        char buffer[256];
        is.getline(buffer, 256, '\0');
        rentalId = buffer;
        
        is.getline(buffer, 256, '\0');
        userId = buffer;
        
        is.getline(buffer, 256, '\0');
        resourceId = buffer;
        
        // 读取时间点（从时间戳转换）
        time_t requestTimeT;
        is.read(reinterpret_cast<char*>(&requestTimeT), sizeof(time_t));
        requestTime = std::chrono::system_clock::from_time_t(requestTimeT);
        
        time_t startTimeT;
        is.read(reinterpret_cast<char*>(&startTimeT), sizeof(time_t));
        startTime = std::chrono::system_clock::from_time_t(startTimeT);
        
        time_t endTimeT;
        is.read(reinterpret_cast<char*>(&endTimeT), sizeof(time_t));
        endTime = std::chrono::system_clock::from_time_t(endTimeT);
        
        // 读取状态和费用
        is.read(reinterpret_cast<char*>(&status), sizeof(RentalStatus));
        is.read(reinterpret_cast<char*>(&totalCost), sizeof(double));
        
        // 读取管理员备注
        is.getline(buffer, 256, '\0');
        adminComment = buffer;
    }
};

/**
 * @class RentalManager
 * @brief 管理系统中所有租赁记录。
 *
 * 提供创建、查询、更新租赁记录的功能。
 * 处理租赁状态的转换和相关业务逻辑。
 */
class RentalManager {
private:
    std::vector<std::unique_ptr<RentalRecord>> rentals;
    std::string dataFilePath;
    int nextRentalId;

public:
    RentalManager(const std::string& filePath) : dataFilePath(filePath), nextRentalId(1) {}
    ~RentalManager() { saveRentals(); }

    // 加载租赁数据
    void loadRentals() {
        std::ifstream file(dataFilePath, std::ios::binary);
        if (!file) {
            std::cerr << "警告: 无法打开租赁数据文件，将创建新文件。\n";
            return;
        }

        // 清空当前租赁记录
        rentals.clear();

        // 读取下一个可用的租赁ID
        file.read(reinterpret_cast<char*>(&nextRentalId), sizeof(int));

        // 读取租赁记录数量
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));

        // 逐个读取租赁记录
        for (size_t i = 0; i < count; ++i) {
            auto rental = std::make_unique<RentalRecord>();
            rental->deserialize(file);
            rentals.push_back(std::move(rental));
        }

        file.close();
    }

    // 保存租赁数据
    void saveRentals() const {
        std::ofstream file(dataFilePath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行写入: " + dataFilePath);
        }

        // 写入下一个可用的租赁ID
        file.write(reinterpret_cast<const char*>(&nextRentalId), sizeof(int));

        // 写入租赁记录数量
        size_t count = rentals.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));

        // 逐个写入租赁记录
        for (const auto& rental : rentals) {
            rental->serialize(file);
        }

        file.close();
    }

    // 创建新的租赁申请
    std::string createRental(const std::string& userId, const std::string& resourceId) {
        std::string rentalId = "R" + std::to_string(nextRentalId++);
        auto rental = std::make_unique<RentalRecord>(rentalId, userId, resourceId);
        rentals.push_back(std::move(rental));
        return rentalId;
    }

    // 根据ID查找租赁记录
    RentalRecord* findRentalById(const std::string& rentalId) {
        for (auto& rental : rentals) {
            if (rental->getRentalId() == rentalId) {
                return rental.get();
            }
        }
        return nullptr;
    }

    // 获取用户的所有租赁记录
    std::vector<RentalRecord*> getUserRentals(const std::string& userId) {
        std::vector<RentalRecord*> result;
        for (auto& rental : rentals) {
            if (rental->getUserId() == userId) {
                result.push_back(rental.get());
            }
        }
        return result;
    }

    // 获取资源的所有租赁记录
    std::vector<RentalRecord*> getResourceRentals(const std::string& resourceId) {
        std::vector<RentalRecord*> result;
        for (auto& rental : rentals) {
            if (rental->getResourceId() == resourceId) {
                result.push_back(rental.get());
            }
        }
        return result;
    }

    // 获取所有待审核的租赁申请
    std::vector<RentalRecord*> getPendingRentals() {
        std::vector<RentalRecord*> result;
        for (auto& rental : rentals) {
            if (rental->getStatus() == RentalStatus::PENDING) {
                result.push_back(rental.get());
            }
        }
        return result;
    }

    // 获取所有活跃的租赁
    std::vector<RentalRecord*> getActiveRentals() {
        std::vector<RentalRecord*> result;
        for (auto& rental : rentals) {
            if (rental->getStatus() == RentalStatus::ACTIVE) {
                result.push_back(rental.get());
            }
        }
        return result;
    }

    // 审核租赁申请
    bool reviewRental(const std::string& rentalId, bool approved, const std::string& comment) {
        auto rental = findRentalById(rentalId);
        if (!rental || rental->getStatus() != RentalStatus::PENDING) {
            return false;
        }

        rental->setAdminComment(comment);
        if (approved) {
            rental->setStatus(RentalStatus::APPROVED);
            rental->setStartTime(std::chrono::system_clock::now());
        } else {
            rental->setStatus(RentalStatus::REJECTED);
        }
        return true;
    }

    // 开始使用资源
    bool startRental(const std::string& rentalId) {
        auto rental = findRentalById(rentalId);
        if (!rental || rental->getStatus() != RentalStatus::APPROVED) {
            return false;
        }

        rental->setStatus(RentalStatus::ACTIVE);
        return true;
    }

    // 完成租赁
    bool completeRental(const std::string& rentalId, double hourlyRate) {
        auto rental = findRentalById(rentalId);
        if (!rental || rental->getStatus() != RentalStatus::ACTIVE) {
            return false;
        }

        rental->setEndTime(std::chrono::system_clock::now());
        rental->calculateCost(hourlyRate);
        rental->setStatus(RentalStatus::COMPLETED);
        return true;
    }

    // 取消租赁申请
    bool cancelRental(const std::string& rentalId) {
        auto rental = findRentalById(rentalId);
        if (!rental || (rental->getStatus() != RentalStatus::PENDING && 
                        rental->getStatus() != RentalStatus::APPROVED)) {
            return false;
        }

        rental->setStatus(RentalStatus::CANCELLED);
        return true;
    }

    // 显示所有租赁记录
    void displayAllRentals() const {
        std::cout << "===== 所有租赁记录 =====\n";
        if (rentals.empty()) {
            std::cout << "暂无租赁记录\n";
            return;
        }

        for (const auto& rental : rentals) {
            rental->displayDetails();
            std::cout << "------------------------\n";
        }
    }

    // 显示用户的租赁记录
    void displayUserRentals(const std::string& userId) const {
        std::cout << "===== 用户 " << userId << " 的租赁记录 =====\n";
        bool found = false;

        for (const auto& rental : rentals) {
            if (rental->getUserId() == userId) {
                rental->displayDetails();
                std::cout << "------------------------\n";
                found = true;
            }
        }

        if (!found) {
            std::cout << "该用户暂无租赁记录\n";
        }
    }
    
    // 检查用户是否有未完成的租赁
    bool hasActiveRentals(const std::string& userId) const {
        for (const auto& rental : rentals) {
            if (rental->getUserId() == userId && 
                (rental->getStatus() == RentalStatus::PENDING || 
                 rental->getStatus() == RentalStatus::APPROVED || 
                 rental->getStatus() == RentalStatus::ACTIVE)) {
                return true;
            }
        }
        return false;
    }
    
    // 检查资源是否已被租用
    bool isResourceRented(const std::string& resourceId) const {
        for (const auto& rental : rentals) {
            if (rental->getResourceId() == resourceId && 
                (rental->getStatus() == RentalStatus::APPROVED || 
                 rental->getStatus() == RentalStatus::ACTIVE)) {
                return true;
            }
        }
        return false;
    }
};

#endif // RENTAL_HPP
