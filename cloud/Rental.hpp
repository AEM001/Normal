#ifndef RENTAL_HPP
#define RENTAL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>

// 租赁状态枚举
enum class RentalStatus {
    PENDING,   // 申请中，等待审核
    APPROVED,  // 已批准，等待使用
    REJECTED,  // 已拒绝
    ACTIVE,    // 使用中
    COMPLETED, // 已完成
    CANCELLED  // 已取消
};

// 枚举转字符串函数
inline const char* RentalStatusToString(RentalStatus status) {
    switch(status) {
        case RentalStatus::PENDING: return "申请中";
        case RentalStatus::APPROVED: return "已批准";
        case RentalStatus::REJECTED: return "已拒绝";
        case RentalStatus::ACTIVE: return "使用中";
        case RentalStatus::COMPLETED: return "已完成";
        case RentalStatus::CANCELLED: return "已取消";
        default: return "未知状态";
    }
}

/**
 * @class RentalRecord
 * @brief 表示一条租赁记录。
 *
 * 包含租赁的所有信息，如用户ID、资源ID、开始和结束时间、状态等。
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
    std::string reviewComment;

public:
    // 构造函数
    RentalRecord(std::string id, std::string user, std::string resource)
        : rentalId(id), userId(user), resourceId(resource), 
          requestTime(std::chrono::system_clock::now()), 
          status(RentalStatus::PENDING), totalCost(0.0) {}
    
    // 为反序列化添加默认构造函数
    RentalRecord() : rentalId(""), userId(""), resourceId(""), 
                     status(RentalStatus::PENDING), totalCost(0.0) {}

    // 获取器
    std::string getRentalId() const { return rentalId; }
    std::string getUserId() const { return userId; }
    std::string getResourceId() const { return resourceId; }
    std::chrono::system_clock::time_point getRequestTime() const { return requestTime; }
    std::chrono::system_clock::time_point getStartTime() const { return startTime; }
    std::chrono::system_clock::time_point getEndTime() const { return endTime; }
    RentalStatus getStatus() const { return status; }
    double getTotalCost() const { return totalCost; }
    std::string getReviewComment() const { return reviewComment; }

    // 设置器
    void setStatus(RentalStatus stat) { status = stat; }
    void setReviewComment(const std::string& comment) { reviewComment = comment; }
    void setTotalCost(double cost) { totalCost = cost; }

    // 开始租赁
    void start() {
        if (status != RentalStatus::APPROVED) {
            throw std::runtime_error("只有已批准的租赁才能开始");
        }
        startTime = std::chrono::system_clock::now();
        status = RentalStatus::ACTIVE;
    }

    // 结束租赁
    void complete(double hourlyRate) {
        if (status != RentalStatus::ACTIVE) {
            throw std::runtime_error("只有使用中的租赁才能完成");
        }
        endTime = std::chrono::system_clock::now();
        status = RentalStatus::COMPLETED;
        
        // 计算总费用
        double hours = calculateDurationHours();
        totalCost = hours * hourlyRate;
    }

    // 取消租赁
    void cancel() {
        if (status != RentalStatus::PENDING) {
            throw std::runtime_error("只有申请中的租赁才能取消");
        }
        status = RentalStatus::CANCELLED;
    }

    // 计算租赁时长（小时）
    double calculateDurationHours() const {
        if (status != RentalStatus::COMPLETED) {
            return 0.0;
        }
        
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
        return duration.count() / 3600.0;
    }

    // 显示租赁详情
    void displayDetails() const {
        std::cout << "===== 租赁详情 =====\n";
        std::cout << "租赁ID: " << rentalId << "\n";
        std::cout << "用户ID: " << userId << "\n";
        std::cout << "资源ID: " << resourceId << "\n";
        std::cout << "状态: " << RentalStatusToString(status) << "\n";
        
        // 显示申请时间
        auto requestTimeT = std::chrono::system_clock::to_time_t(requestTime);
        std::cout << "申请时间: " << std::ctime(&requestTimeT);
        
        // 显示开始时间（如果已开始）
        if (status == RentalStatus::ACTIVE || status == RentalStatus::COMPLETED) {
            auto startTimeT = std::chrono::system_clock::to_time_t(startTime);
            std::cout << "开始时间: " << std::ctime(&startTimeT);
        }
        
        // 显示结束时间和总费用（如果已完成）
        if (status == RentalStatus::COMPLETED) {
            auto endTimeT = std::chrono::system_clock::to_time_t(endTime);
            std::cout << "结束时间: " << std::ctime(&endTimeT);
            std::cout << "使用时长: " << std::fixed << std::setprecision(2) << calculateDurationHours() << " 小时\n";
            std::cout << "总费用: " << std::fixed << std::setprecision(2) << totalCost << " 元\n";
        }
        
        // 显示审核备注（如果有）
        if (!reviewComment.empty()) {
            std::cout << "审核备注: " << reviewComment << "\n";
        }
    }

    // 序列化/反序列化方法（用于数据持久化）
    void serialize(std::ostream& os) const {
        // 写入基本属性
        os.write(rentalId.c_str(), rentalId.size() + 1);
        os.write(userId.c_str(), userId.size() + 1);
        os.write(resourceId.c_str(), resourceId.size() + 1);
        
        // 写入时间点
        auto requestTimeT = std::chrono::system_clock::to_time_t(requestTime);
        os.write(reinterpret_cast<const char*>(&requestTimeT), sizeof(time_t));
        
        auto startTimeT = std::chrono::system_clock::to_time_t(startTime);
        os.write(reinterpret_cast<const char*>(&startTimeT), sizeof(time_t));
        
        auto endTimeT = std::chrono::system_clock::to_time_t(endTime);
        os.write(reinterpret_cast<const char*>(&endTimeT), sizeof(time_t));
        
        // 写入状态和费用
        os.write(reinterpret_cast<const char*>(&status), sizeof(RentalStatus));
        os.write(reinterpret_cast<const char*>(&totalCost), sizeof(double));
        
        // 写入审核备注
        os.write(reviewComment.c_str(), reviewComment.size() + 1);
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
        
        // 读取时间点
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
        
        // 读取审核备注
        is.getline(buffer, 256, '\0');
        reviewComment = buffer;
    }
};

/**
 * @class RentalManager
 * @brief 管理系统中的所有租赁记录。
 *
 * 负责租赁的创建、查找、修改和数据持久化。
 */
class RentalManager {
private:
    std::vector<RentalRecord*> rentals;
    std::string dataFilePath; // 租赁数据文件路径
    int nextRentalId;

public:
    RentalManager(const std::string& filePath) : dataFilePath(filePath), nextRentalId(1) {}
    
    ~RentalManager() { 
        saveRentals(); 
        for (auto rental : rentals) {
            delete rental;
        }
        rentals.clear();
    }

    // 加载租赁数据
    void loadRentals() {
        std::ifstream file(dataFilePath, std::ios::binary);
        if (!file) {
            std::cerr << "警告: 无法打开租赁数据文件，将创建新文件。\n";
            return;
        }

        // 清空当前租赁记录
        for (auto rental : rentals) {
            delete rental;
        }
        rentals.clear();

        // 读取下一个可用的租赁ID
        file.read(reinterpret_cast<char*>(&nextRentalId), sizeof(int));

        // 读取租赁记录数量
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));

        // 逐个读取租赁记录
        for (size_t i = 0; i < count; ++i) {
            RentalRecord* rental = new RentalRecord();
            rental->deserialize(file);
            rentals.push_back(rental);
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
        for (auto rental : rentals) {
            rental->serialize(file);
        }

        file.close();
    }

    // 创建新的租赁记录
    std::string createRental(const std::string& userId, const std::string& resourceId) {
        std::string rentalId = generateUniqueRentalId();
        RentalRecord* rental = new RentalRecord(rentalId, userId, resourceId);
        rentals.push_back(rental);
        return rentalId;
    }

    // 审核租赁申请
    bool reviewRental(const std::string& rentalId, bool approved, const std::string& comment) {
        RentalRecord* rental = findRentalById(rentalId);
        if (!rental || rental->getStatus() != RentalStatus::PENDING) {
            return false;
        }
        
        rental->setStatus(approved ? RentalStatus::APPROVED : RentalStatus::REJECTED);
        rental->setReviewComment(comment);
        return true;
    }

    // 开始租赁
    bool startRental(const std::string& rentalId) {
        RentalRecord* rental = findRentalById(rentalId);
        if (!rental || rental->getStatus() != RentalStatus::APPROVED) {
            return false;
        }
        
        try {
            rental->start();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "开始租赁失败: " << e.what() << std::endl;
            return false;
        }
    }

    // 完成租赁
    bool completeRental(const std::string& rentalId, double hourlyRate) {
        RentalRecord* rental = findRentalById(rentalId);
        if (!rental || rental->getStatus() != RentalStatus::ACTIVE) {
            return false;
        }
        
        try {
            rental->complete(hourlyRate);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "完成租赁失败: " << e.what() << std::endl;
            return false;
        }
    }

    // 取消租赁申请
    bool cancelRental(const std::string& rentalId) {
        RentalRecord* rental = findRentalById(rentalId);
        if (!rental || rental->getStatus() != RentalStatus::PENDING) {
            return false;
        }
        
        try {
            rental->cancel();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "取消租赁失败: " << e.what() << std::endl;
            return false;
        }
    }

    // 根据ID查找租赁记录
    RentalRecord* findRentalById(const std::string& rentalId) {
        for (auto rental : rentals) {
            if (rental->getRentalId() == rentalId) {
                return rental;
            }
        }
        return nullptr; // 未找到租赁记录
    }

    const RentalRecord* findRentalById(const std::string& rentalId) const {
        for (auto rental : rentals) {
            if (rental->getRentalId() == rentalId) {
                return rental;
            }
        }
        return nullptr; // 未找到租赁记录
    }

    // 获取用户的所有租赁记录
    std::vector<RentalRecord*> getUserRentals(const std::string& userId) const {
        std::vector<RentalRecord*> result;
        for (auto rental : rentals) {
            if (rental->getUserId() == userId) {
                result.push_back(rental);
            }
        }
        return result;
    }

    // 获取资源的所有租赁记录
    std::vector<RentalRecord*> getResourceRentals(const std::string& resourceId) const {
        std::vector<RentalRecord*> result;
        for (auto rental : rentals) {
            if (rental->getResourceId() == resourceId) {
                result.push_back(rental);
            }
        }
        return result;
    }

    // 获取所有待审核的租赁申请
    std::vector<RentalRecord*> getPendingRentals() const {
        std::vector<RentalRecord*> result;
        for (auto rental : rentals) {
            if (rental->getStatus() == RentalStatus::PENDING) {
                result.push_back(rental);
            }
        }
        return result;
    }

    // 获取所有活跃的租赁
    std::vector<RentalRecord*> getActiveRentals() const {
        std::vector<RentalRecord*> result;
        for (auto rental : rentals) {
            if (rental->getStatus() == RentalStatus::ACTIVE) {
                result.push_back(rental);
            }
        }
        return result;
    }

    // 检查资源是否被租用
    bool isResourceRented(const std::string& resourceId) const {
        for (auto rental : rentals) {
            if (rental->getResourceId() == resourceId && 
                (rental->getStatus() == RentalStatus::ACTIVE || 
                 rental->getStatus() == RentalStatus::PENDING || 
                 rental->getStatus() == RentalStatus::APPROVED)) {
                return true;
            }
        }
        return false;
    }

    // 显示所有租赁记录
    void displayAllRentals() const {
        if (rentals.empty()) {
            std::cout << "当前没有租赁记录。\n";
            return;
        }
        
        for (auto rental : rentals) {
            rental->displayDetails();
            std::cout << "------------------------\n";
        }
    }

private:
    // 生成唯一的租赁ID
    std::string generateUniqueRentalId() {
        std::string id = "RENT" + std::to_string(nextRentalId++);
        return id;
    }
};

#endif // RENTAL_HPP
