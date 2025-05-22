#include <iostream>
#include <chrono>
#include <thread>
#include <limits>
#include <string>

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// 泄露内存的函数 - 分配但不释放
void leakMemory(size_t sizeInMB, size_t& totalLeaked) {
    try {
        // 计算字节大小
        size_t sizeInBytes = sizeInMB * 1024 * 1024;
        
        // 分配内存并有意泄露（不使用delete）
        std::cout << "正在分配 " << sizeInMB << " MB 内存..." << std::endl;
        char* leakedMemory = new char[sizeInBytes];
        
        // 填充内存以确保它被实际使用
        for (size_t i = 0; i < sizeInBytes; i++) {
            leakedMemory[i] = static_cast<char>(i % 256);
        }
        
        // 更新总泄露量
        totalLeaked += sizeInMB;
        
        // 打印消息表示内存分配
        std::cout << "成功泄露了 " << sizeInMB << " MB 内存！" << std::endl;
        std::cout << "到目前为止总共泄露: " << totalLeaked << " MB" << std::endl;
        
        // 注意：我们故意不调用 delete[] leakedMemory; 这就造成了内存泄露
    } catch (const std::bad_alloc& e) {
        std::cout << "内存分配失败！您可能已经耗尽了内存。" << std::endl;
        std::cout << "错误: " << e.what() << std::endl;
        throw; // 重新抛出给调用者处理
    }
}

// 正确管理内存的函数
void properMemoryManagement(size_t sizeInMB) {
    try {
        // 计算字节大小
        size_t sizeInBytes = sizeInMB * 1024 * 1024;
        
        // 分配内存
        std::cout << "正在分配 " << sizeInMB << " MB 内存..." << std::endl;
        char* memory = new char[sizeInBytes];
        
        // 填充内存以确保它被实际使用
        for (size_t i = 0; i < sizeInBytes; i++) {
            memory[i] = static_cast<char>(i % 256);
        }
        
        std::cout << "成功分配了 " << sizeInMB << " MB 内存。" << std::endl;
        
        // 正确释放内存
        std::cout << "现在释放 " << sizeInMB << " MB 内存..." << std::endl;
        delete[] memory;
        
        std::cout << "成功释放了 " << sizeInMB << " MB 内存！" << std::endl;
        std::cout << "这部分内存不会造成内存泄露。" << std::endl;
    } catch (const std::bad_alloc& e) {
        std::cout << "内存分配失败！您可能已经耗尽了内存。" << std::endl;
        std::cout << "错误: " << e.what() << std::endl;
        throw; // 重新抛出给调用者处理
    }
}

int main() {
    size_t totalLeaked = 0;
    const size_t DEFAULT_MB = 100; // 默认每次分配100MB

    std::cout << "=== 内存管理演示程序 ===" << std::endl;
    std::cout << "本程序演示了以下两种情况的区别:" << std::endl;
    std::cout << "1. 内存泄露（分配但不释放）" << std::endl;
    std::cout << "2. 正确的内存管理（分配并释放）" << std::endl;
    std::cout << "请观察您系统的任务管理器来查看内存使用模式。" << std::endl;
    std::cout << "随时按Ctrl+C退出程序。\n" << std::endl;

    while (true) {
        std::cout << "\n-----------------------------------------" << std::endl;
        std::cout << "当前总泄露内存: " << totalLeaked << " MB" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        
        // 显示菜单
        std::cout << "\n您想做什么？" << std::endl;
        std::cout << "1. 创建内存泄露" << std::endl;
        std::cout << "2. 演示正确的内存管理" << std::endl;
        std::cout << "3. 退出程序" << std::endl;
        std::cout << "请输入您的选择 (1-3): ";
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "输入无效。请输入一个数字。" << std::endl;
            clearInputBuffer();
            continue;
        }
        
        if (choice == 3) {
            std::cout << "退出程序。总泄露内存: " << totalLeaked << " MB" << std::endl;
            std::cout << "注意: 这些内存将在程序退出时被操作系统回收。" << std::endl;
            break;
        }
        
        // 获取要分配的内存量
        size_t mbToAllocate = DEFAULT_MB;
        std::cout << "您想分配多少MB内存？（按Enter使用默认值 " 
                  << DEFAULT_MB << "MB，或输入一个数字）: ";
        
        clearInputBuffer(); // 清除之前的输入
        
        if (std::cin.peek() == '\n') {
            clearInputBuffer();
        } else if (!(std::cin >> mbToAllocate)) {
            std::cout << "输入无效。使用默认值 " << DEFAULT_MB << "MB。" << std::endl;
            clearInputBuffer();
            mbToAllocate = DEFAULT_MB;
        }
        
        try {
            if (choice == 1) {
                // 创建内存泄露
                leakMemory(mbToAllocate, totalLeaked);
            } else if (choice == 2) {
                // 演示正确的内存管理
                properMemoryManagement(mbToAllocate);
            }
        } catch (const std::bad_alloc& e) {
            // 异常已在函数中处理
            std::cout << "由于内存分配错误返回菜单。" << std::endl;
        }
        
        std::cout << "\n请查看任务管理器观察内存使用模式。" << std::endl;
        std::cout << "按Enter继续...";
        clearInputBuffer();
        std::cin.get();
    }

    return 0;
}