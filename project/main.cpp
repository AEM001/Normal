#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <iomanip>
#include <conio.h> // 用于不回显密码输入
#include <filesystem>

#include "User.hpp"
#include "Resource.hpp"
#include "Rental.hpp"
#include "Notification.hpp"
#include "Billing.hpp"
#include "DataManager.hpp"

// 全局变量
DataManager dataManager;
UserManager userManager("data/users.dat");
ResourceManager resourceManager("data/resources.dat");
RentalManager rentalManager("data/rentals.dat");
NotificationManager notificationManager;
BillingManager billingManager("data/bills.dat", "data/billing_rules.dat");
User* currentUser = nullptr;

// 辅助函数
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    std::cout << "\n按任意键继续...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// 安全的密码输入函数（不回显）
std::string getPasswordInput() {
    std::string password;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') { // 处理退格键
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b"; // 删除屏幕上的字符
            }
        } else {
            password.push_back(ch);
            std::cout << '*'; // 显示星号
        }
    }
    std::cout << std::endl;
    return password;
}

// 系统初始化
void initializeSystem() {
    // 确保数据目录存在
    // std::filesystem::path dataDir = "data";
    // if (!std::filesystem::exists(dataDir)) {
    //     std::filesystem::create_directory(dataDir);
    // }
    
    // 加载所有数据
    try {
        userManager.loadUsers();
        resourceManager.loadResources();
        rentalManager.loadRentals();
        notificationManager.loadFromFile("data/notifications.dat");
        billingManager.loadBills();
        billingManager.loadBillingRules();
        
        std::cout << "系统数据加载成功！\n";
    } catch (const std::exception& e) {
        std::cerr << "加载数据时发生错误: " << e.what() << "\n";
        std::cout << "将使用默认数据初始化系统。\n";
    }
}

// 保存系统数据
void saveSystemData() {
    try {
        userManager.saveUsers();
        resourceManager.saveResources();
        rentalManager.saveRentals();
        notificationManager.saveToFile("data/notifications.dat");
        billingManager.saveBills();
        billingManager.saveBillingRules();
        
        std::cout << "系统数据保存成功！\n";
    } catch (const std::exception& e) {
        std::cerr << "保存数据时发生错误: " << e.what() << "\n";
    }
}

// 用户注册
void registerUser() {
    clearScreen();
    std::cout << "===== 用户注册 =====\n";
    
    std::string username, password, confirmPassword;
    int roleChoice;
    
    std::cout << "请输入用户名: ";
    std::cin >> username;
    
    // 检查用户名是否已存在
    if (userManager.findUserByUsername(username)) {
        std::cout << "\n用户名已存在，请选择其他用户名。\n";
        pauseScreen();
        return;
    }
    
    std::cout << "请输入密码: ";
    password = getPasswordInput();
    
    std::cout << "请再次输入密码: ";
    confirmPassword = getPasswordInput();
    
    if (password != confirmPassword) {
        std::cout << "\n两次输入的密码不一致，注册失败。\n";
        pauseScreen();
        return;
    }
    
    std::cout << "请选择用户角色:\n";
    std::cout << "1. 学生\n";
    std::cout << "2. 教师\n";
    std::cout << "请选择: ";
    std::cin >> roleChoice;
    
    UserRole role;
    switch (roleChoice) {
        case 1:
            role = UserRole::STUDENT;
            break;
        case 2:
            role = UserRole::TEACHER;
            break;
        default:
            std::cout << "\n无效的选择，注册失败。\n";
            pauseScreen();
            return;
    }
    
    // 注册用户
    if (userManager.registerUser(username, password, role)) {
        std::cout << "\n注册成功！您现在可以登录系统。\n";
        
        // 保存用户数据
        userManager.saveUsers();
    } else {
        std::cout << "\n注册失败，请稍后再试。\n";
    }
    
    pauseScreen();
}

// 用户登录
bool login() {
    clearScreen();
    std::cout << "===== 用户登录 =====\n";
    
    std::string username, password;
    
    std::cout << "请输入用户名: ";
    std::cin >> username;
    
    std::cout << "请输入密码: ";
    password = getPasswordInput();
    
    // 验证登录
    currentUser = userManager.loginUser(username, password);
    
    if (currentUser) {
        std::cout << "\n登录成功！欢迎回来，" << currentUser->getUsername() << "。\n";
        
        // 检查用户状态
        if (currentUser->getStatus() == UserStatus::SUSPENDED) {
            std::cout << "\n您的账户已被暂停使用，请联系管理员。\n";
            currentUser = nullptr;
            pauseScreen();
            return false;
        }
        
        // 显示未读通知数量
        auto unreadNotifications = notificationManager.getUnreadUserNotifications(currentUser->getUserId());
        if (!unreadNotifications.empty()) {
            std::cout << "\n您有 " << unreadNotifications.size() << " 条未读通知。\n";
        }
        
        pauseScreen();
        return true;
    } else {
        std::cout << "\n登录失败，用户名或密码错误。\n";
        pauseScreen();
        return false;
    }
}

// 显示资源列表
void displayResources() {
    clearScreen();
    std::cout << "===== 资源列表 =====\n";
    
    const auto& resources = resourceManager.getAllResources();
    if (resources.empty()) {
        std::cout << "当前没有可用资源。\n";
        pauseScreen();
        return;
    }
    
    std::cout << "ID\t类型\t名称\t\t\t状态\t小时费率\n";
    std::cout << "--------------------------------------------------------------\n";
    
    for (const auto& resource : resources) {
        std::cout << resource->getResourceId() << "\t"
                  << ResourceTypeToString(resource->getResourceType()) << "\t"
                  << std::left << std::setw(20) << resource->getResourceName() << "\t"
                  << ResourceStatusToString(resource->getStatus()) << "\t"
                  << resource->getHourlyRate() << " 元/小时\n";
    }
    
    pauseScreen();
}

// 显示资源详情
void displayResourceDetails() {
    clearScreen();
    std::cout << "===== 资源详情 =====\n";
    
    std::string resourceId;
    std::cout << "请输入要查看的资源ID: ";
    std::cin >> resourceId;
    
    const Resource* resource = resourceManager.findResourceById(resourceId);
    if (resource) {
        resource->displayDetails();
    } else {
        std::cout << "未找到ID为 " << resourceId << " 的资源。\n";
    }
    
    pauseScreen();
}

// 申请租用资源
void applyForRental() {
    clearScreen();
    std::cout << "===== 申请租用资源 =====\n";
    
    // 显示可用资源
    std::cout << "可用资源列表:\n";
    std::cout << "ID\t类型\t名称\t\t\t小时费率\n";
    std::cout << "--------------------------------------------------------------\n";
    
    bool hasAvailableResources = false;
    for (const auto& resource : resourceManager.getAllResources()) {
        if (resource->isAvailable()) {
            std::cout << resource->getResourceId() << "\t"
                      << ResourceTypeToString(resource->getResourceType()) << "\t"
                      << std::left << std::setw(20) << resource->getResourceName() << "\t"
                      << resource->getHourlyRate() << " 元/小时\n";
            hasAvailableResources = true;
        }
    }
    
    if (!hasAvailableResources) {
        std::cout << "当前没有可用资源。\n";
        pauseScreen();
        return;
    }
    
    // 选择资源
    std::string resourceId;
    std::cout << "\n请输入要租用的资源ID: ";
    std::cin >> resourceId;
    
    Resource* selectedResource = resourceManager.findResourceById(resourceId);
    if (!selectedResource) {
        std::cout << "未找到ID为 " << resourceId << " 的资源。\n";
        pauseScreen();
        return;
    }
    
    if (!selectedResource->isAvailable()) {
        std::cout << "该资源当前不可用。\n";
        pauseScreen();
        return;
    }
    
    // 创建租赁申请
    std::string rentalId = rentalManager.createRental(currentUser->getUserId(), resourceId);
    
    // 创建通知
    notificationManager.createNotification(
        currentUser->getUserId(),
        "您已成功申请租用资源 " + selectedResource->getResourceName() + 
        " (ID: " + selectedResource->getResourceId() + "). 租用ID: " + rentalId,
        NotificationPriority::MEDIUM
    );
    
    // 为管理员创建通知
    auto adminUsers = userManager.getAllUsers();
    for (const auto& user : adminUsers) {
        if (user->getRole() == UserRole::ADMIN) {
            notificationManager.createNotification(
                user->getUserId(),
                "新的租赁申请: 用户 " + currentUser->getUsername() + 
                " (ID: " + currentUser->getUserId() + ") 申请租用资源 " + 
                selectedResource->getResourceName() + " (ID: " + selectedResource->getResourceId() + 
                "). 租用ID: " + rentalId,
                NotificationPriority::HIGH
            );
        }
    }
    
    std::cout << "\n资源 " << selectedResource->getResourceName() << " (ID: " << selectedResource->getResourceId() << ") 租用申请已提交！\n";
    std::cout << "租用ID: " << rentalId << "\n";
    std::cout << "请等待管理员审核。\n";
    
    pauseScreen();
}

// 查看用户的租赁记录
void viewUserRentals() {
    clearScreen();
    std::cout << "===== 我的租赁记录 =====\n";
    
    auto rentals = rentalManager.getUserRentals(currentUser->getUserId());
    if (rentals.empty()) {
        std::cout << "您暂无租赁记录。\n";
        pauseScreen();
        return;
    }
    
    for (const auto& rental : rentals) {
        rental->displayDetails();
        std::cout << "------------------------\n";
    }
    
    pauseScreen();
}

// 查看账户余额
void viewAccountBalance() {
    clearScreen();
    std::cout << "===== 账户余额 =====\n";
    
    std::cout << "当前余额: " << currentUser->getAccountBalance() << " 元\n";
    
    // 显示充值选项
    std::cout << "\n是否要充值？\n";
    std::cout << "1. 是\n";
    std::cout << "2. 否\n";
    std::cout << "请选择: ";
    
    int choice;
    std::cin >> choice;
    
    if (choice == 1) {
        double amount;
        std::cout << "请输入充值金额: ";
        std::cin >> amount;
        
        try {
            currentUser->deposit(amount);
            std::cout << "充值成功！当前余额: " << currentUser->getAccountBalance() << " 元\n";
            
            // 保存用户数据
            userManager.saveUsers();
        } catch (const std::exception& e) {
            std::cout << "充值失败: " << e.what() << "\n";
        }
    }
    
    pauseScreen();
}

// 查看用户的账单
void viewUserBills() {
    clearScreen();
    std::cout << "===== 我的账单 =====\n";
    
    auto bills = billingManager.getUserBills(currentUser->getUserId());
    if (bills.empty()) {
        std::cout << "您暂无账单记录。\n";
        pauseScreen();
        return;
    }
    
    for (const auto& bill : bills) {
        bill->displayDetails();
        
        // 如果账单未支付，提供支付选项
        if (!bill->isPaid()) {
            std::cout << "\n是否要支付此账单？\n";
            std::cout << "1. 是\n";
            std::cout << "2. 否\n";
            std::cout << "请选择: ";
            
            int choice;
            std::cin >> choice;
            
            if (choice == 1) {
                if (billingManager.payBill(bill->getBillId(), currentUser)) {
                    std::cout << "支付成功！\n";
                    
                    // 创建通知
                    notificationManager.createNotification(
                        currentUser->getUserId(),
                        "您已成功支付账单 " + bill->getBillId() + "，金额: " + 
                        std::to_string(bill->getAmount()) + " 元。",
                        NotificationPriority::MEDIUM
                    );
                    
                    // 保存用户和账单数据
                    userManager.saveUsers();
                    billingManager.saveBills();
                } else {
                    std::cout << "支付失败，余额不足或账单已支付。\n";
                    
                    // 如果余额不足，创建通知
                    if (currentUser->getAccountBalance() < bill->getAmount()) {
                        notificationManager.createLowBalanceNotification(
                            currentUser->getUserId(), 
                            currentUser->getAccountBalance()
                        );
                    }
                }
            }
        }
        
        std::cout << "------------------------\n";
    }
    
    pauseScreen();
}

// 管理员审核租赁申请
void reviewRentalRequests() {
    clearScreen();
    std::cout << "===== 审核租赁申请 (管理员) =====\n";
    
    auto pendingRentals = rentalManager.getPendingRentals();
    if (pendingRentals.empty()) {
        std::cout << "当前没有待审核的租赁申请。\n";
        pauseScreen();
        return;
    }
    
    std::cout << "待审核的租赁申请:\n";
    std::cout << "序号\t租赁ID\t\t用户ID\t\t资源ID\t\t申请时间\n";
    std::cout << "--------------------------------------------------------------\n";
    
    int index = 1;
    for (const auto& rental : pendingRentals) {
        auto requestTime = std::chrono::system_clock::to_time_t(rental->getRequestTime());
        std::tm* timeInfo = std::localtime(&requestTime);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M", timeInfo);
        
        std::cout << index << "\t"
                  << rental->getRentalId() << "\t"
                  << rental->getUserId() << "\t"
                  << rental->getResourceId() << "\t"
                  << timeStr << "\n";
        index++;
    }
    
    int choice;
    std::cout << "\n请输入要审核的申请序号 (0 返回): ";
    std::cin >> choice;
    
    if (choice <= 0 || choice > pendingRentals.size()) {
        return;
    }
    
    auto rental = pendingRentals[choice - 1];
    rental->displayDetails();
    
    // 显示用户信息
    auto user = userManager.findUserById(rental->getUserId());
    if (user) {
        std::cout << "\n申请用户信息:\n";
        std::cout << "ID: " << user->getUserId() << "\n";
        std::cout << "用户名: " << user->getUsername() << "\n";
        std::cout << "角色: " << UserRoleToString(user->getRole()) << "\n";
        std::cout << "账户余额: " << user->getAccountBalance() << " 元\n";
    }
    
    // 显示资源信息
    auto resource = resourceManager.findResourceById(rental->getResourceId());
    if (resource) {
        std::cout << "\n申请资源信息:\n";
        resource->displayDetails();
    }
    
    // 审核选项
    std::cout << "\n请选择审核结果:\n";
    std::cout << "1. 批准\n";
    std::cout << "2. 拒绝\n";
    std::cout << "请选择: ";
    
    int reviewChoice;
    std::cin >> reviewChoice;
    
    std::string comment;
    std::cout << "请输入审核备注: ";
    std::cin.ignore();
    std::getline(std::cin, comment);
    
    bool approved = (reviewChoice == 1);
    if (rentalManager.reviewRental(rental->getRentalId(), approved, comment)) {
        if (approved) {
            // 设置资源状态为使用中
            resourceManager.setResourceStatus(rental->getResourceId(), ResourceStatus::IN_USE);
            
            // 开始使用资源
            rentalManager.startRental(rental->getRentalId());
            
            std::cout << "\n已批准租赁申请 " << rental->getRentalId() << "。\n";
            
            // 创建通知
            notificationManager.createRentalStatusNotification(
                rental->getUserId(),
                rental->getRentalId(),
                RentalStatus::APPROVED
            );
        } else {
            std::cout << "\n已拒绝租赁申请 " << rental->getRentalId() << "。\n";
            
            // 创建通知
            notificationManager.createRentalStatusNotification(
                rental->getUserId(),
                rental->getRentalId(),
                RentalStatus::REJECTED
            );
        }
        
        // 保存数据
        rentalManager.saveRentals();
        resourceManager.saveResources();
        notificationManager.saveToFile("data/notifications.dat");
    } else {
        std::cout << "\n审核失败，请稍后再试。\n";
    }
    
    pauseScreen();
}

// 管理员管理资源
void manageResources() {
    while (true) {
        clearScreen();
        std::cout << "===== 资源管理 (管理员) =====\n";
        std::cout << "1. 查看所有资源\n";
        std::cout << "2. 添加新资源\n";
        std::cout << "3. 修改资源\n";
        std::cout << "4. 删除资源\n";
        std::cout << "5. 设置计费标准\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 0:
                return;
            case 1:
                // 查看所有资源
                clearScreen();
                std::cout << "===== 所有资源 =====\n";
                
                for (const auto& resource : resourceManager.getAllResources()) {
                    resource->displayDetails();
                    std::cout << "------------------------\n";
                }
                
                pauseScreen();
                break;
            case 2:
                // 添加新资源
                {
                    clearScreen();
                    std::cout << "===== 添加新资源 =====\n";
                    
                    int typeChoice;
                    std::cout << "请选择资源类型:\n";
                    std::cout << "1. CPU\n";
                    std::cout << "2. GPU\n";
                    std::cout << "请选择: ";
                    std::cin >> typeChoice;
                    
                    ResourceType type;
                    switch (typeChoice) {
                        case 1:
                            type = ResourceType::CPU;
                            break;
                        case 2:
                            type = ResourceType::GPU;
                            break;
                        default:
                            std::cout << "\n无效的选择。\n";
                            pauseScreen();
                            continue;
                    }
                    
                    std::string name;
                    double rate;
                    
                    std::cout << "请输入资源名称: ";
                    std::cin.ignore();
                    std::getline(std::cin, name);
                    
                    std::cout << "请输入小时费率: ";
                    std::cin >> rate;
                    
                    std::string resourceId = resourceManager.generateUniqueResourceId(type);
                    
                    if (type == ResourceType::CPU) {
                        int cores;
                        double frequency;
                        double storage;
                        
                        std::cout << "请输入CPU核心数: ";
                        std::cin >> cores;
                        
                        std::cout << "请输入CPU频率 (GHz): ";
                        std::cin >> frequency;
                        
                        std::cout << "请输入存储容量 (GB): ";
                        std::cin >> storage;
                        
                        auto resource = std::make_unique<CPUResource>(
                            resourceId, name, rate, cores, frequency, ResourceStatus::IDLE, storage
                        );
                        
                        if (resourceManager.addResource(std::move(resource))) {
                            std::cout << "\n成功添加CPU资源 " << name << " (ID: " << resourceId << ")。\n";
                            resourceManager.saveResources();
                        } else {
                            std::cout << "\n添加资源失败。\n";
                        }
                    } else {
                        int cudaCores;
                        int vram;
                        double storage;
                        
                        std::cout << "请输入CUDA核心数: ";
                        std::cin >> cudaCores;
                        
                        std::cout << "请输入显存容量 (GB): ";
                        std::cin >> vram;
                        
                        std::cout << "请输入存储容量 (GB): ";
                        std::cin >> storage;
                        
                        auto resource = std::make_unique<GPUResource>(
                            resourceId, name, rate, cudaCores, vram, ResourceStatus::IDLE, storage
                        );
                        
                        if (resourceManager.addResource(std::move(resource))) {
                            std::cout << "\n成功添加GPU资源 " << name << " (ID: " << resourceId << ")。\n";
                            resourceManager.saveResources();
                        } else {
                            std::cout << "\n添加资源失败。\n";
                        }
                    }
                    
                    pauseScreen();
                }
                break;
            case 3:
                // 修改资源
                {
                    clearScreen();
                    std::cout << "===== 修改资源 =====\n";
                    
                    std::string resourceId;
                    std::cout << "请输入要修改的资源ID: ";
                    std::cin >> resourceId;
                    
                    Resource* resource = resourceManager.findResourceById(resourceId);
                    if (!resource) {
                        std::cout << "未找到ID为 " << resourceId << " 的资源。\n";
                        pauseScreen();
                        continue;
                    }
                    
                    resource->displayDetails();
                    
                    std::string name;
                    double rate;
                    
                    std::cout << "\n请输入新的资源名称 (留空保持不变): ";
                    std::cin.ignore();
                    std::getline(std::cin, name);
                    
                    std::cout << "请输入新的小时费率 (输入0保持不变): ";
                    std::cin >> rate;
                    
                    if (!name.empty()) {
                        resource->setResourceName(name);
                    }
                    
                    if (rate > 0) {
                        resource->setHourlyRate(rate);
                    }
                    
                    // 根据资源类型修改特定属性
                    if (resource->getResourceType() == ResourceType::CPU) {
                        CPUResource* cpuResource = dynamic_cast<CPUResource*>(resource);
                        if (cpuResource) {
                            int cores;
                            double frequency;
                            double storage;
                            
                            std::cout << "请输入新的CPU核心数 (输入0保持不变): ";
                            std::cin >> cores;
                            
                            std::cout << "请输入新的CPU频率 (GHz) (输入0保持不变): ";
                            std::cin >> frequency;
                            
                            std::cout << "请输入新的存储容量 (GB) (输入0保持不变): ";
                            std::cin >> storage;
                            
                            // 更新属性
                            // 注意：这里需要在CPUResource类中添加相应的setter方法
                        }
                    } else {
                        GPUResource* gpuResource = dynamic_cast<GPUResource*>(resource);
                        if (gpuResource) {
                            int cudaCores;
                            int vram;
                            double storage;
                            
                            std::cout << "请输入新的CUDA核心数 (输入0保持不变): ";
                            std::cin >> cudaCores;
                            
                            std::cout << "请输入新的显存容量 (GB) (输入0保持不变): ";
                            std::cin >> vram;
                            
                            std::cout << "请输入新的存储容量 (GB) (输入0保持不变): ";
                            std::cin >> storage;
                            
                            // 更新属性
                            // 注意：这里需要在GPUResource类中添加相应的setter方法
                        }
                    }
                    
                    std::cout << "\n资源 " << resourceId << " 已更新。\n";
                    resourceManager.saveResources();
                    
                    pauseScreen();
                }
                break;
            case 4:
                // 删除资源
                {
                    clearScreen();
                    std::cout << "===== 删除资源 =====\n";
                    
                    std::string resourceId;
                    std::cout << "请输入要删除的资源ID: ";
                    std::cin >> resourceId;
                    
                    Resource* resource = resourceManager.findResourceById(resourceId);
                    if (!resource) {
                        std::cout << "未找到ID为 " << resourceId << " 的资源。\n";
                        pauseScreen();
                        continue;
                    }
                    
                    resource->displayDetails();
                    
                    // 检查资源是否正在使用
                    if (resource->getStatus() == ResourceStatus::IN_USE) {
                        std::cout << "\n该资源当前正在使用中，无法删除。\n";
                        pauseScreen();
                        continue;
                    }
                    
                    // 检查是否有与该资源相关的租赁记录
                    if (rentalManager.isResourceRented(resourceId)) {
                        std::cout << "\n该资源有相关的租赁记录，无法删除。\n";
                        pauseScreen();
                        continue;
                    }
                    
                    std::cout << "\n确定要删除该资源吗？\n";
                    std::cout << "1. 是\n";
                    std::cout << "2. 否\n";
                    std::cout << "请选择: ";
                    
                    int confirmChoice;
                    std::cin >> confirmChoice;
                    
                    if (confirmChoice == 1) {
                        if (resourceManager.deleteResource(resourceId)) {
                            std::cout << "\n资源 " << resourceId << " 已删除。\n";
                            resourceManager.saveResources();
                        } else {
                            std::cout << "\n删除资源失败。\n";
                        }
                    }
                    
                    pauseScreen();
                }
                break;
            case 5:
                // 设置计费标准
                {
                    clearScreen();
                    std::cout << "===== 设置计费标准 =====\n";
                    
                    // 显示当前计费规则
                    billingManager.displayAllBillingRules();
                    
                    int typeChoice;
                    std::cout << "\n请选择要修改的资源类型:\n";
                    std::cout << "1. CPU\n";
                    std::cout << "2. GPU\n";
                    std::cout << "请选择: ";
                    std::cin >> typeChoice;
                    
                    ResourceType type;
                    switch (typeChoice) {
                        case 1:
                            type = ResourceType::CPU;
                            break;
                        case 2:
                            type = ResourceType::GPU;
                            break;
                        default:
                            std::cout << "\n无效的选择。\n";
                            pauseScreen();
                            continue;
                    }
                    
                    double baseRate, studentDiscount, teacherDiscount;
                    
                    std::cout << "请输入基础小时费率: ";
                    std::cin >> baseRate;
                    
                    std::cout << "请输入学生折扣 (0.0-1.0): ";
                    std::cin >> studentDiscount;
                    
                    std::cout << "请输入教师折扣 (0.0-1.0): ";
                    std::cin >> teacherDiscount;
                    
                    // 更新计费规则
                    billingManager.setResourceTypeBillingRule(type, baseRate, studentDiscount, teacherDiscount);
                    
                    std::cout << "\n计费标准已更新。\n";
                    billingManager.saveBillingRules();
                    
                    pauseScreen();
                }
                break;
            default:
                std::cout << "\n无效的选择！\n";
                pauseScreen();
                break;
        }
    }
}

// 管理员管理用户
void manageUsers() {
    while (true) {
        clearScreen();
        std::cout << "===== 用户管理 (管理员) =====\n";
        std::cout << "1. 查看所有用户\n";
        std::cout << "2. 修改用户状态\n";
        std::cout << "3. 添加管理员用户\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 0:
                return;
            case 1:
                // 查看所有用户
                clearScreen();
                std::cout << "===== 所有用户 =====\n";
                
                for (const auto& user : userManager.getAllUsers()) {
                    std::cout << "ID: " << user->getUserId() << "\n";
                    std::cout << "用户名: " << user->getUsername() << "\n";
                    std::cout << "角色: " << UserRoleToString(user->getRole()) << "\n";
                    std::cout << "状态: " << UserStatusToString(user->getStatus()) << "\n";
                    std::cout << "余额: " << user->getAccountBalance() << " 元\n";
                    std::cout << "------------------------\n";
                }
                
                pauseScreen();
                break;
            case 2:
                // 修改用户状态
                {
                    clearScreen();
                    std::cout << "===== 修改用户状态 =====\n";
                    
                    std::string userId;
                    std::cout << "请输入要修改的用户ID: ";
                    std::cin >> userId;
                    
                    User* user = userManager.findUserById(userId);
                    if (!user) {
                        std::cout << "未找到ID为 " << userId << " 的用户。\n";
                        pauseScreen();
                        continue;
                    }
                    
                    std::cout << "用户信息:\n";
                    std::cout << "ID: " << user->getUserId() << "\n";
                    std::cout << "用户名: " << user->getUsername() << "\n";
                    std::cout << "角色: " << UserRoleToString(user->getRole()) << "\n";
                    std::cout << "当前状态: " << UserStatusToString(user->getStatus()) << "\n";
                    
                    std::cout << "\n请选择新的用户状态:\n";
                    std::cout << "1. 正常\n";
                    std::cout << "2. 暂停\n";
                    std::cout << "请选择: ";
                    
                    int statusChoice;
                    std::cin >> statusChoice;
                    
                    UserStatus newStatus;
                    switch (statusChoice) {
                        case 1:
                            newStatus = UserStatus::ACTIVE;
                            break;
                        case 2:
                            newStatus = UserStatus::SUSPENDED;
                            break;
                        default:
                            std::cout << "\n无效的选择。\n";
                            pauseScreen();
                            continue;
                    }
                    
                    if (userManager.setUserStatus(userId, newStatus)) {
                        std::cout << "\n用户 " << user->getUsername() << " 的状态已更新为 " 
                                  << UserStatusToString(newStatus) << "。\n";
                        
                        // 创建通知
                        notificationManager.createNotification(
                            userId,
                            "您的账户状态已被管理员更新为 " + std::string(UserStatusToString(newStatus)) + "。",
                            NotificationPriority::HIGH
                        );
                        
                        // 保存用户数据
                        userManager.saveUsers();
                        notificationManager.saveToFile("data/notifications.dat");
                    } else {
                        std::cout << "\n更新用户状态失败。\n";
                    }
                    
                    pauseScreen();
                }
                break;
            case 3:
                // 添加管理员用户
                {
                    clearScreen();
                    std::cout << "===== 添加管理员用户 =====\n";
                    
                    std::string username, password, confirmPassword;
                    
                    std::cout << "请输入用户名: ";
                    std::cin >> username;
                    
                    // 检查用户名是否已存在
                    if (userManager.findUserByUsername(username)) {
                        std::cout << "\n用户名已存在，请选择其他用户名。\n";
                        pauseScreen();
                        continue;
                    }
                    
                    std::cout << "请输入密码: ";
                    password = getPasswordInput();
                    
                    std::cout << "请再次输入密码: ";
                    confirmPassword = getPasswordInput();
                    
                    if (password != confirmPassword) {
                        std::cout << "\n两次输入的密码不一致，添加失败。\n";
                        pauseScreen();
                        continue;
                    }
                    
                    // 注册管理员用户
                    if (userManager.registerUser(username, password, UserRole::ADMIN)) {
                        std::cout << "\n管理员用户 " << username << " 添加成功！\n";
                        
                        // 保存用户数据
                        userManager.saveUsers();
                    } else {
                        std::cout << "\n添加管理员用户失败，请稍后再试。\n";
                    }
                    
                    pauseScreen();
                }
                break;
            default:
                std::cout << "\n无效的选择！\n";
                pauseScreen();
                break;
        }
    }
}

// 完成租赁
void completeRental() {
    clearScreen();
    std::cout << "===== 完成租赁 =====\n";
    
    // 显示用户的活跃租赁
    auto activeRentals = rentalManager.getActiveRentals();
    std::vector<RentalRecord*> userActiveRentals;
    
    for (auto rental : activeRentals) {
        if (rental->getUserId() == currentUser->getUserId()) {
            userActiveRentals.push_back(rental);
        }
    }
    
    if (userActiveRentals.empty()) {
        std::cout << "您当前没有活跃的租赁。\n";
        pauseScreen();
        return;
    }
    
    std::cout << "您的活跃租赁:\n";
    std::cout << "序号\t租赁ID\t\t资源ID\t\t开始时间\n";
    std::cout << "--------------------------------------------------------------\n";
    
    int index = 1;
    for (const auto& rental : userActiveRentals) {
        auto startTime = std::chrono::system_clock::to_time_t(rental->getStartTime());
        std::tm* timeInfo = std::localtime(&startTime);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M", timeInfo);
        
        std::cout << index << "\t"
                  << rental->getRentalId() << "\t"
                  << rental->getResourceId() << "\t"
                  << timeStr << "\n";
        index++;
    }
    
    int choice;
    std::cout << "\n请输入要完成的租赁序号 (0 返回): ";
    std::cin >> choice;
    
    if (choice <= 0 || choice > userActiveRentals.size()) {
        return;
    }
    
    auto rental = userActiveRentals[choice - 1];
    
    // 获取资源信息
    Resource* resource = resourceManager.findResourceById(rental->getResourceId());
    if (!resource) {
        std::cout << "未找到相关资源信息，无法完成租赁。\n";
        pauseScreen();
        return;
    }
    
    // 完成租赁
    if (rentalManager.completeRental(rental->getRentalId(), resource->getHourlyRate())) {
        // 设置资源状态为空闲
        resourceManager.setResourceStatus(rental->getResourceId(), ResourceStatus::IDLE);
        
        // 创建账单
        std::string billId = billingManager.createBill(
            rental->getRentalId(), 
            rental->getUserId(), 
            rental->getTotalCost()
        );
        
        std::cout << "\n租赁 " << rental->getRentalId() << " 已完成。\n";
        std::cout << "使用时长: " << rental->calculateDurationHours() << " 小时\n";
        std::cout << "总费用: " << rental->getTotalCost() << " 元\n";
        std::cout << "已生成账单 " << billId << "，请及时支付。\n";
        
        // 创建通知
        notificationManager.createRentalStatusNotification(
            rental->getUserId(),
            rental->getRentalId(),
            RentalStatus::COMPLETED
        );
        
        notificationManager.createNotification(
            rental->getUserId(),
            "您的租赁 " + rental->getRentalId() + " 已完成，总费用: " + 
            std::to_string(rental->getTotalCost()) + " 元。已生成账单 " + billId + "，请及时支付。",
            NotificationPriority::HIGH
        );
        
        // 保存数据
        rentalManager.saveRentals();
        resourceManager.saveResources();
        billingManager.saveBills();
        notificationManager.saveToFile("data/notifications.dat");
    } else {
        std::cout << "\n完成租赁失败，请稍后再试。\n";
    }
    
    pauseScreen();
}

// 取消租赁申请
void cancelRental() {
    clearScreen();
    std::cout << "===== 取消租赁申请 =====\n";
    
    // 显示用户的待审核租赁
    auto userRentals = rentalManager.getUserRentals(currentUser->getUserId());
    std::vector<RentalRecord*> pendingRentals;
    
    for (auto rental : userRentals) {
        if (rental->getStatus() == RentalStatus::PENDING) {
            pendingRentals.push_back(rental);
        }
    }
    
    if (pendingRentals.empty()) {
        std::cout << "您当前没有待审核的租赁申请。\n";
        pauseScreen();
        return;
    }
    
    std::cout << "您的待审核租赁申请:\n";
    std::cout << "序号\t租赁ID\t\t资源ID\t\t申请时间\n";
    std::cout << "--------------------------------------------------------------\n";
    
    int index = 1;
    for (const auto& rental : pendingRentals) {
        auto requestTime = std::chrono::system_clock::to_time_t(rental->getRequestTime());
        std::tm* timeInfo = std::localtime(&requestTime);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M", timeInfo);
        
        std::cout << index << "\t"
                  << rental->getRentalId() << "\t"
                  << rental->getResourceId() << "\t"
                  << timeStr << "\n";
        index++;
    }
    
    int choice;
    std::cout << "\n请输入要取消的申请序号 (0 返回): ";
    std::cin >> choice;
    
    if (choice <= 0 || choice > pendingRentals.size()) {
        return;
    }
    
    auto rental = pendingRentals[choice - 1];
    
    // 取消租赁申请
    if (rentalManager.cancelRental(rental->getRentalId())) {
        std::cout << "\n租赁申请 " << rental->getRentalId() << " 已取消。\n";
        
        // 创建通知
        notificationManager.createRentalStatusNotification(
            rental->getUserId(),
            rental->getRentalId(),
            RentalStatus::CANCELLED
        );
        
        // 保存数据
        rentalManager.saveRentals();
        notificationManager.saveToFile("data/notifications.dat");
    } else {
        std::cout << "\n取消租赁申请失败，请稍后再试。\n";
    }
    
    pauseScreen();
}

// 查看通知
void viewNotifications() {
    clearScreen();
    std::cout << "===== 我的通知 =====\n";
    
    notificationManager.displayUserNotifications(currentUser->getUserId(), true);
    
    // 将所有通知标记为已读
    for (const auto& notification : notificationManager.getUserNotifications(currentUser->getUserId(), true)) {
        notificationManager.markNotificationAsRead(notification.notificationId);
    }
    
    // 保存通知数据
    notificationManager.saveToFile("data/notifications.dat");
    
    pauseScreen();
}

// 用户菜单
void showUserMenu() {
    while (currentUser) {
        clearScreen();
        
        // 显示用户信息
        std::cout << "===== 用户控制面板 =====\n";
        std::cout << "用户名: " << currentUser->getUsername() << "\n";
        std::cout << "角色: " << UserRoleToString(currentUser->getRole()) << "\n";
        std::cout << "余额: " << currentUser->getAccountBalance() << " 元\n";
        
        // 检查未读通知
        auto unreadNotifications = notificationManager.getUnreadUserNotifications(currentUser->getUserId());
        if (!unreadNotifications.empty()) {
            std::cout << "您有 " << unreadNotifications.size() << " 条未读通知。\n";
        }
        
        // 根据用户角色显示不同的菜单选项
        UserRole role = currentUser->getRole();
        
        if (role == UserRole::STUDENT || role == UserRole::TEACHER) {
            std::cout << "\n===== 菜单 =====\n";
            std::cout << "1. 浏览资源\n";
            std::cout << "2. 查看资源详情\n";
            std::cout << "3. 申请租用资源\n";
            std::cout << "4. 查看我的租赁记录\n";
            std::cout << "5. 完成租赁\n";
            std::cout << "6. 取消租赁申请\n";
            std::cout << "7. 查看账户余额\n";
            std::cout << "8. 查看我的账单\n";
            std::cout << "9. 查看通知\n";
            std::cout << "0. 退出登录\n";
        } else if (role == UserRole::ADMIN) {
            std::cout << "\n===== 管理员菜单 =====\n";
            std::cout << "1. 浏览资源\n";
            std::cout << "2. 查看资源详情\n";
            std::cout << "3. 审核租赁申请\n";
            std::cout << "4. 管理用户\n";
            std::cout << "5. 管理资源\n";
            std::cout << "6. 查看所有租赁记录\n";
            std::cout << "7. 查看所有账单\n";
            std::cout << "8. 设置计费规则\n";
            std::cout << "9. 查看通知\n";
            std::cout << "0. 退出登录\n";
        }
        
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        
        if (choice == 0) {
            std::cout << "\n已退出登录。\n";
            currentUser = nullptr;
            pauseScreen();
            break;
        }
        
        if (role == UserRole::STUDENT || role == UserRole::TEACHER) {
            switch (choice) {
                case 1:
                    displayResources();
                    break;
                case 2:
                    displayResourceDetails();
                    break;
                case 3:
                    applyForRental();
                    break;
                case 4:
                    viewUserRentals();
                    break;
                case 5:
                    completeRental();
                    break;
                case 6:
                    cancelRental();
                    break;
                case 7:
                    viewAccountBalance();
                    break;
                case 8:
                    viewUserBills();
                    break;
                case 9:
                    viewNotifications();
                    break;
                default:
                    std::cout << "\n无效的选择！\n";
                    pauseScreen();
                    break;
            }
        } else if (role == UserRole::ADMIN) {
            switch (choice) {
                case 1:
                    displayResources();
                    break;
                case 2:
                    displayResourceDetails();
                    break;
                case 3:
                    reviewRentalRequests();
                    break;
                case 4:
                    manageUsers();
                    break;
                case 5:
                    manageResources();
                    break;
                case 6:
                    // 查看所有租赁记录
                    clearScreen();
                    std::cout << "===== 所有租赁记录 =====\n";
                    rentalManager.displayAllRentals();
                    pauseScreen();
                    break;
                case 7:
                    // 查看所有账单
                    clearScreen();
                    std::cout << "===== 所有账单记录 =====\n";
                    billingManager.displayAllBills();
                    pauseScreen();
                    break;
                case 8:
                    // 设置计费规则
                    clearScreen();
                    std::cout << "===== 设置计费规则 =====\n";
                    billingManager.displayAllBillingRules();
                    
                    int typeChoice;
                    std::cout << "\n请选择要修改的资源类型:\n";
                    std::cout << "1. CPU\n";
                    std::cout << "2. GPU\n";
                    std::cout << "请选择: ";
                    std::cin >> typeChoice;
                    
                    ResourceType type;
                    switch (typeChoice) {
                        case 1:
                            type = ResourceType::CPU;
                            break;
                        case 2:
                            type = ResourceType::GPU;
                            break;
                        default:
                            std::cout << "\n无效的选择。\n";
                            pauseScreen();
                            continue;
                    }
                    
                    double baseRate, studentDiscount, teacherDiscount;
                    
                    std::cout << "请输入基础小时费率: ";
                    std::cin >> baseRate;
                    
                    std::cout << "请输入学生折扣 (0.0-1.0): ";
                    std::cin >> studentDiscount;
                    
                    std::cout << "请输入教师折扣 (0.0-1.0): ";
                    std::cin >> teacherDiscount;
                    
                    // 更新计费规则
                    billingManager.setResourceTypeBillingRule(type, baseRate, studentDiscount, teacherDiscount);
                    
                    std::cout << "\n计费标准已更新。\n";
                    billingManager.saveBillingRules();
                    
                    pauseScreen();
                    break;
                case 9:
                    viewNotifications();
                    break;
                default:
                    std::cout << "\n无效的选择！\n";
                    pauseScreen();
                    break;
            }
        }
    }
}

// 主菜单
void showMainMenu() {
    while (true) {
        clearScreen();
        std::cout << "===== 云算力资源租用系统 =====\n";
        std::cout << "1. 登录\n";
        std::cout << "2. 注册\n";
        std::cout << "0. 退出系统\n";
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 0:
                std::cout << "\n感谢使用云算力资源租用系统，再见！\n";
                saveSystemData();
                return;
            case 1:
                if (login()) {
                    showUserMenu();
                }
                break;
            case 2:
                registerUser();
                break;
            default:
                std::cout << "\n无效的选择！\n";
                pauseScreen();
                break;
        }
    }
}

int main() {
    // 初始化随机数生成器
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // 初始化系统
    initializeSystem();
    
    // 显示主菜单
    showMainMenu();
    
    // 保存系统数据
    saveSystemData();
    
    return 0;
}
