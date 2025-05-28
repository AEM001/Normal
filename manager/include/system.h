#ifndef SYSTEM_H
#define SYSTEM_H

#include "user_manager.h"
#include "resource_manager.h"
#include "rental_manager.h"
#include "billing_manager.h"
#include "notification_manager.h"
#include "data_manager.h"
#include <memory>
#include <iostream>
#include <string>
#include <cstdlib>

// 全局工具函数实现
std::string getCurrentTime() {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

std::string generateId() {
    static int counter = 0;
    std::string timestamp = std::to_string(time(nullptr));
    std::string uniqueId = timestamp + "_" + std::to_string(++counter);
    return uniqueId;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    std::cout << "\n按回车键继续...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string getPasswordInput() {
    std::string password;
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        password.push_back(ch);
        std::cout << '*';
    }
    return password;
}

bool confirmOperation(const std::string& message) {
    std::cout << message << " (y/n): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return (choice == 'y' || choice == 'Y');
}

// 系统类
class System {
private:
    std::shared_ptr<DataManager> dataManager;
    std::shared_ptr<UserManager> userManager;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<RentalManager> rentalManager;
    std::shared_ptr<BillingManager> billingManager;
    std::shared_ptr<NotificationManager> notificationManager;
    
    bool running;
    
    // 显示主菜单
    void showMainMenu() {
        clearScreen();
        std::cout << "===== 云算力资源租用系统 =====\n";
        std::cout << "1. 登录\n";
        std::cout << "2. 注册\n";
        std::cout << "0. 退出\n";
        std::cout << "请选择: ";
    }
    
    // 显示学生/教师菜单
    void showUserMenu() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 用户菜单 [" << currentUser->getName() << "] =====\n";
        std::cout << "1. 个人信息管理\n";
        std::cout << "2. 余额查询与充值\n";
        std::cout << "3. 资源浏览\n";
        std::cout << "4. 租用申请\n";
        std::cout << "5. 租用记录查询\n";
        std::cout << "6. 账单查询\n";
        std::cout << "7. 通知查看\n";
        std::cout << "0. 退出登录\n";
        std::cout << "请选择: ";
    }
    
    // 显示管理员菜单
    void showAdminMenu() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr || currentUser->getType() != ADMIN) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 管理员菜单 [" << currentUser->getName() << "] =====\n";
        std::cout << "1. 用户管理\n";
        std::cout << "2. 资源管理\n";
        std::cout << "3. 租用审核\n";
        std::cout << "4. 租用记录查询\n";
        std::cout << "5. 账单管理\n";
        std::cout << "6. 计费规则设置\n";
        std::cout << "7. 系统日志查看\n";
        std::cout << "8. 数据备份与恢复\n";
        std::cout << "0. 退出登录\n";
        std::cout << "请选择: ";
    }
    
    // 处理登录
    void handleLogin() {
        clearScreen();
        std::cout << "===== 用户登录 =====\n";
        
        std::string username, password;
        std::cout << "用户名: ";
        std::cin >> username;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::cout << "密码: ";
        password = getPasswordInput();
        
        if (userManager->login(username, password)) {
            std::cout << "\n登录成功！\n";
            
            // 检查用户状态
            auto currentUser = userManager->getCurrentUser();
            if (!currentUser->isActive()) {
                std::cout << "警告：您的账号已被暂停，请联系管理员。\n";
                pauseScreen();
                userManager->logout();
                return;
            }
            
            // 显示未读通知
            auto unreadNotifications = notificationManager->getUserUnreadNotifications(currentUser->getId());
            if (!unreadNotifications.empty()) {
                std::cout << "\n您有 " << unreadNotifications.size() << " 条未读通知：\n";
                for (const auto& notification : unreadNotifications) {
                    if (notification->isHighPriority()) {
                        std::cout << "[重要] ";
                    }
                    std::cout << notification->getContent() << " (" << notification->getCreateTime() << ")\n";
                    notification->markAsRead();
                }
                notificationManager->saveNotifications();
            }
            
            pauseScreen();
        } else {
            std::cout << "\n登录失败，请检查用户名和密码。\n";
            pauseScreen();
        }
    }
    
    // 处理注册
    void handleRegister() {
        clearScreen();
        std::cout << "===== 用户注册 =====\n";
        std::cout << "请选择注册类型：\n";
        std::cout << "1. 学生\n";
        std::cout << "2. 教师\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == 0) {
            return;
        }
        
        if (choice != 1 && choice != 2) {
            std::cout << "无效选择，请重试。\n";
            pauseScreen();
            return;
        }
        
        UserType type = (choice == 1) ? STUDENT : TEACHER;
        
        std::string username, password, name, idNumber, specialInfo;
        
        std::cout << "用户名: ";
        std::cin >> username;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::cout << "密码: ";
        password = getPasswordInput();
        
        std::cout << "\n姓名: ";
        std::getline(std::cin, name);
        
        if (type == STUDENT) {
            std::cout << "学号: ";
            std::cin >> idNumber;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            std::cout << "专业: ";
            std::getline(std::cin, specialInfo);
        } else {
            std::cout << "工号: ";
            std::cin >> idNumber;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            std::cout << "职称: ";
            std::getline(std::cin, specialInfo);
        }
        
        if (userManager->registerUser(username, password, name, type, idNumber, specialInfo)) {
            std::cout << "\n注册成功！\n";
        } else {
            std::cout << "\n注册失败，请检查输入信息。\n";
        }
        
        pauseScreen();
    }
    
    // 处理用户菜单
    void handleUserMenu() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr) {
            return;
        }
        
        int choice;
        do {
            showUserMenu();
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            switch (choice) {
                case 1:
                    handleUserInfo();
                    break;
                case 2:
                    handleBalance();
                    break;
                case 3:
                    handleBrowseResources();
                    break;
                case 4:
                    handleRentalApplication();
                    break;
                case 5:
                    handleRentalRecords();
                    break;
                case 6:
                    handleBills();
                    break;
                case 7:
                    handleNotifications();
                    break;
                case 0:
                    userManager->logout();
                    std::cout << "已退出登录。\n";
                    pauseScreen();
                    break;
                default:
                    std::cout << "无效选择，请重试。\n";
                    pauseScreen();
            }
        } while (choice != 0 && userManager->getCurrentUser() != nullptr);
    }
    
    // 处理管理员菜单
    void handleAdminMenu() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr || currentUser->getType() != ADMIN) {
            return;
        }
        
        int choice;
        do {
            showAdminMenu();
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            switch (choice) {
                case 1:
                    handleUserManagement();
                    break;
                case 2:
                    handleResourceManagement();
                    break;
                case 3:
                    handleRentalReview();
                    break;
                case 4:
                    handleAllRentalRecords();
                    break;
                case 5:
                    handleBillManagement();
                    break;
                case 6:
                    handlePricingRules();
                    break;
                case 7:
                    handleSystemLogs();
                    break;
                case 8:
                    handleDataBackup();
                    break;
                case 0:
                    userManager->logout();
                    std::cout << "已退出登录。\n";
                    pauseScreen();
                    break;
                default:
                    std::cout << "无效选择，请重试。\n";
                    pauseScreen();
            }
        } while (choice != 0 && userManager->getCurrentUser() != nullptr);
    }
    
    // 处理个人信息管理
    void handleUserInfo() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 个人信息管理 =====\n";
        currentUser->displayInfo();
        
        std::cout << "\n1. 修改姓名\n";
        std::cout << "2. 修改密码\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                std::string name;
                std::cout << "新姓名: ";
                std::getline(std::cin, name);
                
                if (userManager->updateUserInfo(name)) {
                    std::cout << "姓名修改成功！\n";
                } else {
                    std::cout << "姓名修改失败。\n";
                }
                break;
            }
            case 2: {
                std::string oldPassword, newPassword;
                std::cout << "原密码: ";
                oldPassword = getPasswordInput();
                
                std::cout << "\n新密码: ";
                newPassword = getPasswordInput();
                
                if (userManager->changePassword(oldPassword, newPassword)) {
                    std::cout << "\n密码修改成功！\n";
                } else {
                    std::cout << "\n密码修改失败，请检查原密码。\n";
                }
                break;
            }
            case 0:
                return;
            default:
                std::cout << "无效选择，请重试。\n";
        }
        
        pauseScreen();
    }
    
    // 处理余额查询与充值
    void handleBalance() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 余额查询与充值 =====\n";
        std::cout << "当前余额: " << std::fixed << std::setprecision(2) << currentUser->getBalance() << std::endl;
        
        std::cout << "\n1. 充值\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == 1) {
            double amount;
            std::cout << "充值金额: ";
            std::cin >> amount;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            if (amount <= 0) {
                std::cout << "充值金额必须大于0。\n";
            } else if (userManager->addBalance(amount)) {
                std::cout << "充值成功！当前余额: " << std::fixed << std::setprecision(2) << currentUser->getBalance() << std::endl;
            } else {
                std::cout << "充值失败。\n";
            }
        }
        
        pauseScreen();
    }
    
    // 处理资源浏览
    void handleBrowseResources() {
        clearScreen();
        std::cout << "===== 资源浏览 =====\n";
        std::cout << "请选择资源类型：\n";
        std::cout << "1. 所有资源\n";
        std::cout << "2. CPU资源\n";
        std::cout << "3. GPU资源\n";
        std::cout << "4. 存储资源\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::vector<std::shared_ptr<Resource>> resources;
        
        switch (choice) {
            case 1:
                resources = resourceManager->getAllResources();
                break;
            case 2:
                resources = resourceManager->getResourcesByType(CPU);
                break;
            case 3:
                resources = resourceManager->getResourcesByType(GPU);
                break;
            case 4:
                resources = resourceManager->getResourcesByType(STORAGE);
                break;
            case 0:
                return;
            default:
                std::cout << "无效选择，请重试。\n";
                pauseScreen();
                return;
        }
        
        if (resources.empty()) {
            std::cout << "没有找到符合条件的资源。\n";
            pauseScreen();
            return;
        }
        
        clearScreen();
        std::cout << "找到 " << resources.size() << " 个资源：\n\n";
        
        for (size_t i = 0; i < resources.size(); ++i) {
            std::cout << "资源 " << (i + 1) << ":\n";
            resources[i]->displayInfo();
            std::cout << "------------------------\n";
        }
        
        pauseScreen();
    }
    
    // 处理租用申请
    void handleRentalApplication() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 租用申请 =====\n";
        
        // 显示可用资源
        auto idleResources = resourceManager->getIdleResources();
        if (idleResources.empty()) {
            std::cout << "当前没有可用资源。\n";
            pauseScreen();
            return;
        }
        
        std::cout << "可用资源列表：\n\n";
        for (size_t i = 0; i < idleResources.size(); ++i) {
            std::cout << "资源 " << (i + 1) << ":\n";
            idleResources[i]->displayInfo();
            std::cout << "------------------------\n";
        }
        
        // 选择资源
        std::cout << "请选择要租用的资源编号 (1-" << idleResources.size() << ")，或输入0返回: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == 0) {
            return;
        }
        
        if (choice < 1 || choice > static_cast<int>(idleResources.size())) {
            std::cout << "无效选择，请重试。\n";
            pauseScreen();
            return;
        }
        
        auto selectedResource = idleResources[choice - 1];
        
        // 输入租用时间
        std::string startTime, endTime;
        std::cout << "请输入租用开始时间 (格式: YYYY-MM-DD HH:MM:SS): ";
        std::getline(std::cin, startTime);
        
        std::cout << "请输入租用结束时间 (格式: YYYY-MM-DD HH:MM:SS): ";
        std::getline(std::cin, endTime);
        
        // 创建租用申请
        if (rentalManager->applyRental(selectedResource->getId(), startTime, endTime)) {
            std::cout << "租用申请提交成功，等待管理员审核。\n";
        } else {
            std::cout << "租用申请提交失败，请检查输入信息。\n";
        }
        
        pauseScreen();
    }
    
    // 处理租用记录查询
    void handleRentalRecords() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 租用记录查询 =====\n";
        
        auto userRentals = rentalManager->getUserRentals(currentUser->getId());
        if (userRentals.empty()) {
            std::cout << "您没有租用记录。\n";
            pauseScreen();
            return;
        }
        
        std::cout << "找到 " << userRentals.size() << " 条租用记录：\n\n";
        
        for (size_t i = 0; i < userRentals.size(); ++i) {
            std::cout << "记录 " << (i + 1) << ":\n";
            userRentals[i]->displayInfo();
            
            // 显示资源信息
            auto resource = resourceManager->getResource(userRentals[i]->getResourceId());
            if (resource != nullptr) {
                std::cout << "资源信息：\n";
                std::cout << "名称: " << resource->getName() << std::endl;
                std::cout << "类型: ";
                switch (resource->getType()) {
                    case CPU: std::cout << "CPU"; break;
                    case GPU: std::cout << "GPU"; break;
                    case STORAGE: std::cout << "存储"; break;
                }
                std::cout << std::endl;
            }
            
            std::cout << "------------------------\n";
        }
        
        // 操作菜单
        std::cout << "1. 取消租用申请\n";
        std::cout << "2. 开始使用资源\n";
        std::cout << "3. 完成租用\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == 0) {
            return;
        }
        
        std::cout << "请输入要操作的记录编号 (1-" << userRentals.size() << "): ";
        int recordChoice;
        std::cin >> recordChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (recordChoice < 1 || recordChoice > static_cast<int>(userRentals.size())) {
            std::cout << "无效选择，请重试。\n";
            pauseScreen();
            return;
        }
        
        auto selectedRental = userRentals[recordChoice - 1];
        
        switch (choice) {
            case 1:
                if (rentalManager->cancelRental(selectedRental->getId())) {
                    std::cout << "租用申请已取消。\n";
                } else {
                    std::cout << "取消失败，只能取消申请中或已审核通过的租用。\n";
                }
                break;
            case 2:
                if (rentalManager->startRental(selectedRental->getId())) {
                    std::cout << "已开始使用资源。\n";
                } else {
                    std::cout << "操作失败，只能开始已审核通过的租用。\n";
                }
                break;
            case 3:
                if (rentalManager->completeRental(selectedRental->getId())) {
                    std::cout << "租用已完成。\n";
                    
                    // 生成账单
                    if (billingManager->generateBill(selectedRental)) {
                        std::cout << "账单已生成。\n";
                    }
                } else {
                    std::cout << "操作失败，只能完成使用中的租用。\n";
                }
                break;
            default:
                std::cout << "无效选择，请重试。\n";
        }
        
        pauseScreen();
    }
    
    // 处理账单查询
    void handleBills() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 账单查询 =====\n";
        
        auto userBills = billingManager->getUserBills(currentUser->getId());
        if (userBills.empty()) {
            std::cout << "您没有账单记录。\n";
            pauseScreen();
            return;
        }
        
        std::cout << "找到 " << userBills.size() << " 条账单记录：\n\n";
        
        for (size_t i = 0; i < userBills.size(); ++i) {
            std::cout << "账单 " << (i + 1) << ":\n";
            userBills[i]->displayInfo();
            std::cout << "------------------------\n";
        }
        
        // 操作菜单
        std::cout << "1. 支付账单\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == 1) {
            std::cout << "请输入要支付的账单编号 (1-" << userBills.size() << "): ";
            int billChoice;
            std::cin >> billChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            if (billChoice < 1 || billChoice > static_cast<int>(userBills.size())) {
                std::cout << "无效选择，请重试。\n";
                pauseScreen();
                return;
            }
            
            auto selectedBill = userBills[billChoice - 1];
            
            if (selectedBill->isPaid()) {
                std::cout << "该账单已支付。\n";
            } else if (billingManager->payBill(selectedBill->getId())) {
                std::cout << "账单支付成功。\n";
            } else {
                std::cout << "账单支付失败，请检查余额。\n";
            }
        }
        
        pauseScreen();
    }
    
    // 处理通知查看
    void handleNotifications() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 通知查看 =====\n";
        
        auto userNotifications = notificationManager->getUserNotifications(currentUser->getId());
        if (userNotifications.empty()) {
            std::cout << "您没有通知。\n";
            pauseScreen();
            return;
        }
        
        std::cout << "找到 " << userNotifications.size() << " 条通知：\n\n";
        
        for (size_t i = 0; i < userNotifications.size(); ++i) {
            std::cout << "通知 " << (i + 1) << ":\n";
            userNotifications[i]->displayInfo();
            std::cout << "------------------------\n";
        }
        
        // 操作菜单
        std::cout << "1. 标记通知为已读\n";
        std::cout << "2. 删除通知\n";
        std::cout << "3. 清理所有已读通知\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                std::cout << "请输入要标记的通知编号 (1-" << userNotifications.size() << "): ";
                int notificationChoice;
                std::cin >> notificationChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (notificationChoice < 1 || notificationChoice > static_cast<int>(userNotifications.size())) {
                    std::cout << "无效选择，请重试。\n";
                    break;
                }
                
                auto selectedNotification = userNotifications[notificationChoice - 1];
                
                if (notificationManager->markAsRead(selectedNotification->getId())) {
                    std::cout << "通知已标记为已读。\n";
                } else {
                    std::cout << "操作失败。\n";
                }
                break;
            }
            case 2: {
                std::cout << "请输入要删除的通知编号 (1-" << userNotifications.size() << "): ";
                int notificationChoice;
                std::cin >> notificationChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (notificationChoice < 1 || notificationChoice > static_cast<int>(userNotifications.size())) {
                    std::cout << "无效选择，请重试。\n";
                    break;
                }
                
                auto selectedNotification = userNotifications[notificationChoice - 1];
                
                if (notificationManager->deleteNotification(selectedNotification->getId())) {
                    std::cout << "通知已删除。\n";
                } else {
                    std::cout << "删除失败。\n";
                }
                break;
            }
            case 3:
                if (notificationManager->cleanReadNotifications()) {
                    std::cout << "已清理所有已读通知。\n";
                } else {
                    std::cout << "清理失败。\n";
                }
                break;
            case 0:
                return;
            default:
                std::cout << "无效选择，请重试。\n";
        }
        
        pauseScreen();
    }
    
    // 处理用户管理
    void handleUserManagement() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr || currentUser->getType() != ADMIN) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 用户管理 =====\n";
        
        auto allUsers = userManager->getAllUsers();
        std::cout << "系统中共有 " << allUsers.size() << " 个用户：\n\n";
        
        for (size_t i = 0; i < allUsers.size(); ++i) {
            std::cout << "用户 " << (i + 1) << ":\n";
            allUsers[i]->displayInfo();
            std::cout << "------------------------\n";
        }
        
        // 操作菜单
        std::cout << "1. 添加管理员\n";
        std::cout << "2. 修改用户状态\n";
        std::cout << "3. 删除用户\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                std::string username, password, name, permissionLevel;
                
                std::cout << "用户名: ";
                std::cin >> username;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                std::cout << "密码: ";
                password = getPasswordInput();
                
                std::cout << "\n姓名: ";
                std::getline(std::cin, name);
                
                std::cout << "权限级别 (1-5): ";
                std::cin >> permissionLevel;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (userManager->registerUser(username, password, name, ADMIN, "0", permissionLevel)) {
                    std::cout << "管理员添加成功！\n";
                } else {
                    std::cout << "管理员添加失败。\n";
                }
                break;
            }
            case 2: {
                std::cout << "请输入要修改的用户编号 (1-" << allUsers.size() << "): ";
                int userChoice;
                std::cin >> userChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (userChoice < 1 || userChoice > static_cast<int>(allUsers.size())) {
                    std::cout << "无效选择，请重试。\n";
                    break;
                }
                
                auto selectedUser = allUsers[userChoice - 1];
                
                std::cout << "当前状态: " << (selectedUser->getStatus() == ACTIVE ? "激活" : "暂停") << std::endl;
                std::cout << "请选择新状态 (0-激活, 1-暂停): ";
                int statusChoice;
                std::cin >> statusChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (statusChoice != 0 && statusChoice != 1) {
                    std::cout << "无效选择，请重试。\n";
                    break;
                }
                
                UserStatus newStatus = static_cast<UserStatus>(statusChoice);
                
                if (userManager->updateUserStatus(selectedUser->getId(), newStatus)) {
                    std::cout << "用户状态已更新。\n";
                    
                    // 创建通知
                    notificationManager->createAccountStatusNotification(selectedUser->getId(), newStatus);
                } else {
                    std::cout << "更新失败。\n";
                }
                break;
            }
            case 3: {
                std::cout << "请输入要删除的用户编号 (1-" << allUsers.size() << "): ";
                int userChoice;
                std::cin >> userChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (userChoice < 1 || userChoice > static_cast<int>(allUsers.size())) {
                    std::cout << "无效选择，请重试。\n";
                    break;
                }
                
                auto selectedUser = allUsers[userChoice - 1];
                
                if (selectedUser->getId() == currentUser->getId()) {
                    std::cout << "不能删除当前登录的用户。\n";
                    break;
                }
                
                if (confirmOperation("确定要删除用户 " + selectedUser->getUsername() + " 吗？")) {
                    if (userManager->deleteUser(selectedUser->getId())) {
                        std::cout << "用户已删除。\n";
                    } else {
                        std::cout << "删除失败。\n";
                    }
                }
                break;
            }
            case 0:
                return;
            default:
                std::cout << "无效选择，请重试。\n";
        }
        
        pauseScreen();
    }
    
    // 处理资源管理
    void handleResourceManagement() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr || currentUser->getType() != ADMIN) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 资源管理 =====\n";
        
        auto allResources = resourceManager->getAllResources();
        std::cout << "系统中共有 " << allResources.size() << " 个资源：\n\n";
        
        for (size_t i = 0; i < allResources.size(); ++i) {
            std::cout << "资源 " << (i + 1) << ":\n";
            allResources[i]->displayInfo();
            std::cout << "------------------------\n";
        }
        
        // 操作菜单
        std::cout << "1. 添加CPU资源\n";
        std::cout << "2. 添加GPU资源\n";
        std::cout << "3. 添加存储资源\n";
        std::cout << "4. 删除资源\n";
        std::cout << "5. 更新资源价格\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                std::string name, description;
                int cores;
                double frequency, pricePerHour;
                
                std::cout << "名称: ";
                std::getline(std::cin, name);
                
                std::cout << "描述: ";
                std::getline(std::cin, description);
                
                std::cout << "核心数: ";
                std::cin >> cores;
                
                std::cout << "频率 (GHz): ";
                std::cin >> frequency;
                
                std::cout << "每小时价格: ";
                std::cin >> pricePerHour;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (resourceManager->addCPUResource(name, description, cores, frequency, pricePerHour)) {
                    std::cout << "CPU资源添加成功！\n";
                } else {
                    std::cout << "CPU资源添加失败。\n";
                }
                break;
            }
            case 2: {
                std::string name, description, model;
                int memory;
                double computePower, pricePerHour;
                
                std::cout << "名称: ";
                std::getline(std::cin, name);
                
                std::cout << "描述: ";
                std::getline(std::cin, description);
                
                std::cout << "型号: ";
                std::getline(std::cin, model);
                
                std::cout << "显存 (GB): ";
                std::cin >> memory;
                
                std::cout << "计算能力 (TFLOPS): ";
                std::cin >> computePower;
                
                std::cout << "每小时价格: ";
                std::cin >> pricePerHour;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (resourceManager->addGPUResource(name, description, model, memory, computePower, pricePerHour)) {
                    std::cout << "GPU资源添加成功！\n";
                } else {
                    std::cout << "GPU资源添加失败。\n";
                }
                break;
            }
            case 3: {
                std::string name, description, storageType;
                int capacity;
                double readSpeed, writeSpeed, pricePerHour;
                
                std::cout << "名称: ";
                std::getline(std::cin, name);
                
                std::cout << "描述: ";
                std::getline(std::cin, description);
                
                std::cout << "容量 (GB): ";
                std::cin >> capacity;
                
                std::cout << "读取速度 (MB/s): ";
                std::cin >> readSpeed;
                
                std::cout << "写入速度 (MB/s): ";
                std::cin >> writeSpeed;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                std::cout << "存储类型 (SSD/HDD): ";
                std::getline(std::cin, storageType);
                
                std::cout << "每小时价格: ";
                std::cin >> pricePerHour;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (resourceManager->addStorageResource(name, description, capacity, readSpeed, writeSpeed, storageType, pricePerHour)) {
                    std::cout << "存储资源添加成功！\n";
                } else {
                    std::cout << "存储资源添加失败。\n";
                }
                break;
            }
            case 4: {
                std::cout << "请输入要删除的资源编号 (1-" << allResources.size() << "): ";
                int resourceChoice;
                std::cin >> resourceChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (resourceChoice < 1 || resourceChoice > static_cast<int>(allResources.size())) {
                    std::cout << "无效选择，请重试。\n";
                    break;
                }
                
                auto selectedResource = allResources[resourceChoice - 1];
                
                if (confirmOperation("确定要删除资源 " + selectedResource->getName() + " 吗？")) {
                    if (resourceManager->deleteResource(selectedResource->getId())) {
                        std::cout << "资源已删除。\n";
                    } else {
                        std::cout << "删除失败，资源可能正在使用中。\n";
                    }
                }
                break;
            }
            case 5: {
                std::cout << "请输入要更新价格的资源编号 (1-" << allResources.size() << "): ";
                int resourceChoice;
                std::cin >> resourceChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (resourceChoice < 1 || resourceChoice > static_cast<int>(allResources.size())) {
                    std::cout << "无效选择，请重试。\n";
                    break;
                }
                
                auto selectedResource = allResources[resourceChoice - 1];
                
                std::cout << "当前每小时价格: " << std::fixed << std::setprecision(2) << selectedResource->getPricePerHour() << std::endl;
                std::cout << "新的每小时价格: ";
                double newPrice;
                std::cin >> newPrice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (resourceManager->updateResourcePrice(selectedResource->getId(), newPrice)) {
                    std::cout << "价格已更新。\n";
                } else {
                    std::cout << "更新失败。\n";
                }
                break;
            }
            case 0:
                return;
            default:
                std::cout << "无效选择，请重试。\n";
        }
        
        pauseScreen();
    }
    
    // 处理租用审核
    void handleRentalReview() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr || currentUser->getType() != ADMIN) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 租用审核 =====\n";
        
        auto pendingRentals = rentalManager->getPendingRentals();
        if (pendingRentals.empty()) {
            std::cout << "没有待审核的租用申请。\n";
            pauseScreen();
            return;
        }
        
        std::cout << "找到 " << pendingRentals.size() << " 条待审核的租用申请：\n\n";
        
        for (size_t i = 0; i < pendingRentals.size(); ++i) {
            std::cout << "申请 " << (i + 1) << ":\n";
            pendingRentals[i]->displayInfo();
            
            // 显示用户信息
            auto user = userManager->findUser(pendingRentals[i]->getUserId());
            if (user != nullptr) {
                std::cout << "申请用户: " << user->getUsername() << " (" << user->getName() << ")\n";
            }
            
            // 显示资源信息
            auto resource = resourceManager->getResource(pendingRentals[i]->getResourceId());
            if (resource != nullptr) {
                std::cout << "申请资源: " << resource->getName() << " (" << resource->getDescription() << ")\n";
            }
            
            std::cout << "------------------------\n";
        }
        
        // 审核操作
        std::cout << "请输入要审核的申请编号 (1-" << pendingRentals.size() << ")，或输入0返回: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == 0) {
            return;
        }
        
        if (choice < 1 || choice > static_cast<int>(pendingRentals.size())) {
            std::cout << "无效选择，请重试。\n";
            pauseScreen();
            return;
        }
        
        auto selectedRental = pendingRentals[choice - 1];
        
        std::cout << "请选择审核结果 (1-通过, 2-拒绝): ";
        int reviewChoice;
        std::cin >> reviewChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (reviewChoice != 1 && reviewChoice != 2) {
            std::cout << "无效选择，请重试。\n";
            pauseScreen();
            return;
        }
        
        bool approved = (reviewChoice == 1);
        
        std::cout << "审核备注: ";
        std::string comment;
        std::getline(std::cin, comment);
        
        if (rentalManager->reviewRental(selectedRental->getId(), approved, comment)) {
            std::cout << "审核完成。\n";
            
            // 创建通知
            notificationManager->createRentalReviewNotification(selectedRental->getUserId(), selectedRental->getId(), approved);
        } else {
            std::cout << "审核失败。\n";
        }
        
        pauseScreen();
    }
    
    // 处理所有租用记录查询
    void handleAllRentalRecords() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr || currentUser->getType() != ADMIN) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 所有租用记录 =====\n";
        
        auto allRentals = rentalManager->getAllRentals();
        if (allRentals.empty()) {
            std::cout << "没有租用记录。\n";
            pauseScreen();
            return;
        }
        
        std::cout << "找到 " << allRentals.size() << " 条租用记录：\n\n";
        
        for (size_t i = 0; i < allRentals.size(); ++i) {
            std::cout << "记录 " << (i + 1) << ":\n";
            allRentals[i]->displayInfo();
            
            // 显示用户信息
            auto user = userManager->findUser(allRentals[i]->getUserId());
            if (user != nullptr) {
                std::cout << "用户: " << user->getUsername() << " (" << user->getName() << ")\n";
            }
            
            // 显示资源信息
            auto resource = resourceManager->getResource(allRentals[i]->getResourceId());
            if (resource != nullptr) {
                std::cout << "资源: " << resource->getName() << " (" << resource->getDescription() << ")\n";
            }
            
            std::cout << "------------------------\n";
        }
        
        pauseScreen();
    }
    
    // 处理账单管理
    void handleBillManagement() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr || currentUser->getType() != ADMIN) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 账单管理 =====\n";
        
        auto allBills = billingManager->getAllBills();
        if (allBills.empty()) {
            std::cout << "没有账单记录。\n";
            pauseScreen();
            return;
        }
        
        std::cout << "找到 " << allBills.size() << " 条账单记录：\n\n";
        
        for (size_t i = 0; i < allBills.size(); ++i) {
            std::cout << "账单 " << (i + 1) << ":\n";
            allBills[i]->displayInfo();
            
            // 显示用户信息
            auto user = userManager->findUser(allBills[i]->getUserId());
            if (user != nullptr) {
                std::cout << "用户: " << user->getUsername() << " (" << user->getName() << ")\n";
            }
            
            // 显示资源信息
            auto resource = resourceManager->getResource(allBills[i]->getResourceId());
            if (resource != nullptr) {
                std::cout << "资源: " << resource->getName() << " (" << resource->getDescription() << ")\n";
            }
            
            std::cout << "------------------------\n";
        }
        
        pauseScreen();
    }
    
    // 处理计费规则设置
    void handlePricingRules() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr || currentUser->getType() != ADMIN) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 计费规则设置 =====\n";
        
        auto allRules = billingManager->getAllPricingRules();
        std::cout << "当前计费规则：\n\n";
        
        for (size_t i = 0; i < allRules.size(); ++i) {
            std::cout << "规则 " << (i + 1) << ":\n";
            allRules[i]->displayInfo();
            std::cout << "------------------------\n";
        }
        
        // 操作菜单
        std::cout << "1. 修改CPU资源计费规则\n";
        std::cout << "2. 修改GPU资源计费规则\n";
        std::cout << "3. 修改存储资源计费规则\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice < 1 || choice > 3) {
            if (choice != 0) {
                std::cout << "无效选择，请重试。\n";
                pauseScreen();
            }
            return;
        }
        
        ResourceType type;
        switch (choice) {
            case 1: type = CPU; break;
            case 2: type = GPU; break;
            case 3: type = STORAGE; break;
            default: return;
        }
        
        auto rule = billingManager->getPricingRule(type);
        if (rule != nullptr) {
            std::cout << "当前每小时价格: " << std::fixed << std::setprecision(2) << rule->getPricePerHour() << std::endl;
            std::cout << "当前最小计费小时: " << rule->getMinBillingHour() << std::endl;
            std::cout << "当前最长租用天数: " << rule->getMaxRentalDays() << std::endl;
        }
        
        double pricePerHour;
        int minBillingHour, maxRentalDays;
        
        std::cout << "新的每小时价格: ";
        std::cin >> pricePerHour;
        
        std::cout << "新的最小计费小时: ";
        std::cin >> minBillingHour;
        
        std::cout << "新的最长租用天数: ";
        std::cin >> maxRentalDays;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (billingManager->setPricingRule(type, pricePerHour, minBillingHour, maxRentalDays)) {
            std::cout << "计费规则已更新。\n";
        } else {
            std::cout << "更新失败。\n";
        }
        
        pauseScreen();
    }
    
    // 处理系统日志查看
    void handleSystemLogs() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr || currentUser->getType() != ADMIN) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 系统日志查看 =====\n";
        
        // 读取日志文件
        std::ifstream logFile("./data/logs.dat");
        if (!logFile.is_open()) {
            std::cout << "无法打开日志文件。\n";
            pauseScreen();
            return;
        }
        
        std::string line;
        std::vector<std::string> logs;
        
        while (std::getline(logFile, line)) {
            logs.push_back(line);
        }
        
        logFile.close();
        
        if (logs.empty()) {
            std::cout << "日志为空。\n";
            pauseScreen();
            return;
        }
        
        // 显示日志
        std::cout << "系统日志 (" << logs.size() << " 条记录)：\n\n";
        
        for (const auto& log : logs) {
            std::cout << log << std::endl;
        }
        
        pauseScreen();
    }
    
    // 处理数据备份与恢复
    void handleDataBackup() {
        auto currentUser = userManager->getCurrentUser();
        if (currentUser == nullptr || currentUser->getType() != ADMIN) {
            return;
        }
        
        clearScreen();
        std::cout << "===== 数据备份与恢复 =====\n";
        std::cout << "1. 创建备份\n";
        std::cout << "2. 恢复备份\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                if (dataManager->backupData()) {
                    std::cout << "数据备份成功。\n";
                } else {
                    std::cout << "数据备份失败。\n";
                }
                break;
            case 2: {
                auto backupList = dataManager->getBackupList();
                if (backupList.empty()) {
                    std::cout << "没有可用的备份。\n";
                    break;
                }
                
                std::cout << "可用备份：\n";
                for (size_t i = 0; i < backupList.size(); ++i) {
                    std::cout << (i + 1) << ". " << backupList[i] << std::endl;
                }
                
                std::cout << "请选择要恢复的备份编号 (1-" << backupList.size() << ")，或输入0返回: ";
                int backupChoice;
                std::cin >> backupChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (backupChoice == 0) {
                    break;
                }
                
                if (backupChoice < 1 || backupChoice > static_cast<int>(backupList.size())) {
                    std::cout << "无效选择，请重试。\n";
                    break;
                }
                
                if (confirmOperation("确定要恢复备份 " + backupList[backupChoice - 1] + " 吗？这将覆盖当前数据。")) {
                    if (dataManager->restoreData(backupList[backupChoice - 1])) {
                        std::cout << "数据恢复成功，系统将重新启动。\n";
                        pauseScreen();
                        running = false;
                    } else {
                        std::cout << "数据恢复失败。\n";
                    }
                }
                break;
            }
            case 0:
                return;
            default:
                std::cout << "无效选择，请重试。\n";
        }
        
        pauseScreen();
    }
    
public:
    System() : running(false) {
        // 创建数据管理器
        dataManager = std::make_shared<DataManager>();
        
        // 创建用户管理器
        userManager = std::make_shared<UserManager>(dataManager);
        
        // 创建资源管理器
        resourceManager = std::make_shared<ResourceManager>(dataManager);
        
        // 创建租用管理器
        rentalManager = std::make_shared<RentalManager>(dataManager, resourceManager, userManager);
        
        // 创建计费管理器
        billingManager = std::make_shared<BillingManager>(dataManager, resourceManager, userManager);
        
        // 创建通知管理器
        notificationManager = std::make_shared<NotificationManager>(dataManager, userManager);
    }
    
    // 运行系统
    void run() {
        running = true;
        
        while (running) {
            auto currentUser = userManager->getCurrentUser();
            
            if (currentUser == nullptr) {
                // 未登录，显示主菜单
                showMainMenu();
                
                int choice;
                std::cin >> choice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                switch (choice) {
                    case 1:
                        handleLogin();
                        break;
                    case 2:
                        handleRegister();
                        break;
                    case 0:
                        running = false;
                        std::cout << "感谢使用，再见！\n";
                        break;
                    default:
                        std::cout << "无效选择，请重试。\n";
                        pauseScreen();
                }
            } else if (currentUser->getType() == ADMIN) {
                // 管理员已登录，显示管理员菜单
                handleAdminMenu();
            } else {
                // 普通用户已登录，显示用户菜单
                handleUserMenu();
            }
        }
    }
};

#endif // SYSTEM_H
