#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
#include <map>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <limits>

// 用户状态枚举
enum UserStatus {
    ACTIVE = 0,
    SUSPENDED = 1
};

// 用户类型枚举
enum UserType {
    STUDENT = 0,
    TEACHER = 1,
    ADMIN = 2
};

// 资源类型枚举
enum ResourceType {
    CPU = 0,
    GPU = 1,
    STORAGE = 2
};

// 资源状态枚举
enum ResourceStatus {
    IDLE = 0,
    IN_USE = 1
};

// 租用状态枚举
enum RentalStatus {
    PENDING = 0,
    APPROVED = 1,
    REJECTED = 2,
    USING = 3,
    COMPLETED = 4,
    CANCELLED = 5
};

// 通知优先级枚举
enum NotificationPriority {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2
};

// 通知状态枚举
enum NotificationStatus {
    UNREAD = 0,
    READ = 1
};

// 账单状态枚举
enum BillStatus {
    UNPAID = 0,
    PAID = 1
};

// 全局常量
const int MAX_LOGIN_ATTEMPTS = 3;
const int MAX_RENTAL_DAYS = 15;
const int MIN_BILLING_HOUR = 1;
const double DEFAULT_BALANCE = 1000.0;

// 全局工具函数
std::string getCurrentTime();
std::string generateId();
void clearScreen();
void pauseScreen();
std::string getPasswordInput();
bool confirmOperation(const std::string& message);

#endif // COMMON_H
