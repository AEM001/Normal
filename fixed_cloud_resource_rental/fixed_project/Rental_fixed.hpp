#ifndef RENTAL_HPP
#define RENTAL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <ctime>
#include <stdexcept>
#include <memory>

// 租赁状态枚举
enum class RentalStatus {
    PENDING,    // 申请中，等待审核
    APPROVED,   // 已批准，正在使用
    REJECTED,   // 已拒绝
    COMPLETED,  // 已完成
    CANCELLED   // 已取消
};

// 枚举转字符串函数
inline const char* RentalStatusToString(RentalStatus status) {
    switch(status) {
        case RentalStatus::PENDING: return "申请中";
        case RentalStatus::APPROVED: return "已批准";
        case RentalStatus::REJECTED: return "已拒绝";
        case RentalStatus::COMPLETED: return "已完成";
        case RentalStatus::CANCELLED: return "已取消";
        default: return "未知状态";
    }
}

/**
 * @class RentalRecord
 * @brief 表示资源租赁记录。
 *
 * 包含租赁的基本信息和状态管理。
 */
class RentalRecord {
private:
    std::string rentalId;
    std::string userId;
    std::string resourceId;
    RentalStatus status;
    std::chrono::system_clock::time_point requestTime;
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point endTime;
    std::string reviewComment;
    bool billed;

public:
    // 构造函数
    RentalRecord(std::string id, std::string uid, std::string rid)
        : rentalId(id), userId(uid), resourceId(rid), status(RentalStatus::PENDING),
          requestTime(std::chrono::system_clock::now()), billed(false) {}
    
    // 为反序列化添加默认构造函数
    RentalRecord() : rentalId(""), userId(""), resourceId(""), status(RentalStatus::PENDING), billed(false) {}
    
    // 获取器
    std::string getRentalId() const { return rentalId; }
    std::string getUserId() const { return userId; }
    std::string getResourceId() const { return resourceId; }
    RentalStatus getStatus() const { return status; }
    std::chrono::system_clock::time_point getRequestTime() const { return requestTime; }
    std::chrono::system_clock::time_point getStartTime() const { return startTime; }
    std::chrono::system_clock::time_point getEndTime() const { return endTime; }
    std::string getReviewComment() const { return reviewComment; }
    bool isBilled() const { return billed; }
    
    // 设置器
    void setStatus(RentalStatus stat) { status = stat; }
    void setReviewComment(const std::string& comment) { reviewComment = comment; }
    void setBilled(bool b) { billed = b; }
    
    // 开始租赁
    void start() {
        if (status != RentalStatus::APPROVED) {
            throw std::logic_error("只有已批准的租赁才能开始");
        }
        startTime = std::chrono::system_clock::now();
    }
    
    // 结束租赁
    void end() {
        if (status != RentalStatus::APPROVED) {
            throw std::logic_error("只有已批准的租赁才能结束");
        }
        endTime = std::chrono::system_clock::now();
        status = RentalStatus::COMPLETED;
    }
    
    // 取消租赁
    void cancel() {
        if (status != RentalStatus::PENDING && status != RentalStatus::APPROVED) {
            throw std::logic_error("只有申请中或已批准的租赁才能取消");
        }
        status = RentalStatus::CANCELLED;
    }
    
    // 审核租赁
    void review(bool approved, const std::string& comment) {
        if (status != RentalStatus::PENDING) {
            throw std::logic_error("只有申请中的租赁才能审核");
        }
        status = approved ? RentalStatus::APPROVED : RentalStatus::REJECTED;
        reviewComment = comment;
    }
    
    // 计算使用时长（小时）
    double getUsageHours() const {
        if (status != RentalStatus::COMPLETED) {
            return 0.0;
        }
        
        auto duration = std::chrono::duration_cast<std::chrono::hours>(endTime - startTime).count();
        return static_cast<double>(duration);
    }
    
    // 显示租赁详情
    void displayDetails() const {
        std::cout << "===== 租赁详情 =====\n";
        std::cout << "租赁ID: " << rentalId << "\n";
        std::cout << "用户ID: " << userId << "\n";
        std::cout << "资源ID: " << resourceId << "\n";
        std::cout << "状态: " << RentalStatusToString(status) << "\n";
        
        // 格式化时间
        auto requestTimeT = std::chrono::system_clock::to_time_t(requestTime);
        std::tm* requestTimeInfo = std::localtime(&requestTimeT);
        char requestTimeStr[20];
        strftime(requestTimeStr, sizeof(requestTimeStr), "%Y-%m-%d %H:%M", requestTimeInfo);
        std::cout << "申请时间: " << requestTimeStr << "\n";
        
        if (status == RentalStatus::APPROVED || status == RentalStatus::COMPLETED) {
            auto startTimeT = std::chrono::system_clock::to_time_t(startTime);
            std::tm* startTimeInfo = std::localtime(&startTimeT);
            char startTimeStr[20];
            strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d %H:%M", startTimeInfo);
            std::cout << "开始时间: " << startTimeStr << "\n";
        }
        
        if (status == RentalStatus::COMPLETED) {
            auto endTimeT = std::chrono::system_clock::to_time_t(endTime);
            std::tm* endTimeInfo = std::localtime(&endTimeT);
            char endTimeStr[20];
            strftime(endTimeStr, sizeof(endTimeStr), "%Y-%m-%d %H:%M", endTimeInfo);
            std::cout << "结束时间: " << endTimeStr << "\n";
            std::cout << "使用时长: " << getUsageHours() << " 小时\n";
        }
        
        if (!reviewComment.empty()) {
            std::cout << "审核备注: " << reviewComment << "\n";
        }
        
        std::cout << "账单状态: " << (billed ? "已生成" : "未生成") << "\n";
    }
    
    // 序列化/反序列化方法（用于数据持久化）
    void serialize(std::ostream& os) const {
        os.write(rentalId.c_str(), rentalId.size() + 1);
        os.write(userId.c_str(), userId.size() + 1);
        os.write(resourceId.c_str(), resourceId.size() + 1);
        os.write(reinterpret_cast<const char*>(&status), sizeof(RentalStatus));
        
        // 序列化时间点
        auto requestTimeT = std::chrono::system_clock::to_time_t(requestTime);
        os.write(reinterpret_cast<const char*>(&requestTimeT), sizeof(std::time_t));
        
        auto startTimeT = std::chrono::system_clock::to_time_t(startTime);
        os.write(reinterpret_cast<const char*>(&startTimeT), sizeof(std::time_t));
        
        auto endTimeT = std::chrono::system_clock::to_time_t(endTime);
        os.write(reinterpret_cast<const char*>(&endTimeT), sizeof(std::time_t));
        
        os.write(reviewComment.c_str(), reviewComment.size() + 1);
        os.write(reinterpret_cast<const char*>(&billed), sizeof(bool));
    }
    
    void deserialize(std::istream& is) {
        char buffer[256];
        is.getline(buffer, 256, '\0');
        rentalId = buffer;
        
        is.getline(buffer, 256, '\0');
        userId = buffer;
        
        is.getline(buffer, 256, '\0');
        resourceId = buffer;
        
        is.read(reinterpret_cast<char*>(&status), sizeof(RentalStatus));
        
        // 反序列化时间点
        std::time_t requestTimeT;
        is.read(reinterpret_cast<char*>(&requestTimeT), sizeof(std::time_t));
        requestTime = std::chrono::system_clock::from_time_t(requestTimeT);
        
        std::time_t startTimeT;
        is.read(reinterpret_cast<char*>(&startTimeT), sizeof(std::time_t));
        startTime = std::chrono::system_clock::from_time_t(startTimeT);
        
        std::time_t endTimeT;
        is.read(reinterpret_cast<char*>(&endTimeT), sizeof(std::time_t));
        endTime = std::chrono::system_clock::from_time_t(endTimeT);
        
        is.getline(buffer, 256, '\0');
        reviewComment = buffer;
        
        is.read(reinterpret_cast<char*>(&billed), sizeof(bool));
    }
};

/**
 * @class RentalManager
 * @brief 管理系统中的所有租赁记录。
 *
 * 负责租赁的创建、查找、审核和数据持久化。
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
        try {
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
        } catch (const std::exception& e) {
            std::cerr << "加载租赁数据时发生错误: " << e.what() << "\n";
        }
    }

    // 保存租赁数据
    void saveRentals() const {
        try {
            // 确保data目录存在
            #ifdef _WIN32
                _mkdir("data");
            #else
                mkdir("data", 0777);
            #endif
            
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
        } catch (const std::exception& e) {
            std::cerr << "保存租赁数据时发生错误: " << e.what() << "\n";
        }
    }

    // 创建新租赁
    std::string createRental(const std::string& userId, const std::string& resourceId) {
        // 生成租赁ID
        char idStr[6];
        sprintf(idStr, "R%04d", nextRentalId++);
        std::string rentalId = idStr;

        // 创建新租赁记录
        RentalRecord* rental = new RentalRecord(rentalId, userId, resourceId);
        rentals.push_back(rental);

        return rentalId;
    }

    // 审核租赁
    bool reviewRental(const std::string& rentalId, bool approved, const std::string& comment) {
        RentalRecord* rental = findRentalById(rentalId);
        if (!rental || rental->getStatus() != RentalStatus::PENDING) {
            return false;
        }

        try {
            rental->review(approved, comment);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "审核租赁时发生错误: " << e.what() << "\n";
            return false;
        }
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
            std::cerr << "开始租赁时发生错误: " << e.what() << "\n";
            return false;
        }
    }

    // 结束租赁
    bool endRental(const std::string& rentalId) {
        RentalRecord* rental = findRentalById(rentalId);
        if (!rental || rental->getStatus() != RentalStatus::APPROVED) {
            return false;
        }

        try {
            rental->end();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "结束租赁时发生错误: " << e.what() << "\n";
            return false;
        }
    }

    // 取消租赁
    bool cancelRental(const std::string& rentalId) {
        RentalRecord* rental = findRentalById(rentalId);
        if (!rental || (rental->getStatus() != RentalStatus::PENDING && rental->getStatus() != RentalStatus::APPROVED)) {
            return false;
        }

        try {
            rental->cancel();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "取消租赁时发生错误: " << e.what() << "\n";
            return false;
        }
    }

    // 根据ID查找租赁记录
    RentalRecord* findRentalById(const std::string& id) const {
        for (auto rental : rentals) {
            if (rental->getRentalId() == id) {
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

    // 获取所有待审核的租赁记录
    std::vector<RentalRecord*> getPendingRentals() const {
        std::vector<RentalRecord*> result;
        for (auto rental : rentals) {
            if (rental->getStatus() == RentalStatus::PENDING) {
                result.push_back(rental);
            }
        }
        return result;
    }

    // 获取所有已批准的租赁记录
    std::vector<RentalRecord*> getApprovedRentals() const {
        std::vector<RentalRecord*> result;
        for (auto rental : rentals) {
            if (rental->getStatus() == RentalStatus::APPROVED) {
                result.push_back(rental);
            }
        }
        return result;
    }

    // 获取所有已完成的租赁记录
    std::vector<RentalRecord*> getCompletedRentals() const {
        std::vector<RentalRecord*> result;
        for (auto rental : rentals) {
            if (rental->getStatus() == RentalStatus::COMPLETED) {
                result.push_back(rental);
            }
        }
        return result;
    }

    // 检查资源是否被租用
    bool isResourceRented(const std::string& resourceId) const {
        for (auto rental : rentals) {
            if (rental->getResourceId() == resourceId && 
                (rental->getStatus() == RentalStatus::PENDING || 
                 rental->getStatus() == RentalStatus::APPROVED)) {
                return true;
            }
        }
        return false;
    }
};

#endif // RENTAL_HPP
