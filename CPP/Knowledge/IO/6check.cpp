#include <fstream>
#include <iostream>
#include <string>

int main() {
    // 1. 检查文件打开失败状态
    std::ifstream nonExistentFile("non_existent_file.txt");
    std::cout << "检查不存在的文件状态:" << std::endl;
    std::cout << "  good(): " << nonExistentFile.good() << std::endl;
    std::cout << "  fail(): " << nonExistentFile.fail() << std::endl;
    nonExistentFile.close();

    // 2. 检查正常文件读取状态
    std::cout << "\n检查正常文件读取:" << std::endl;
    std::ifstream existingFile("my_input_file.txt");
    
    if (existingFile.is_open()) {
        std::string word;
        while (existingFile >> word) {
            // 循环读取文件内容
        }
        
        std::cout << "读取结束状态:" << std::endl;
        std::cout << "  eof(): " << existingFile.eof() << std::endl;
        std::cout << "  fail(): " << existingFile.fail() << std::endl;
        
        existingFile.close();
    } else {
        std::cerr << "无法打开文件!" << std::endl;
        return 1;
    }

    // 3. 检查类型不匹配错误
    std::ifstream numberFile("number_test.txt");
    if (numberFile.is_open()) {
        int num;
       numberFile >> num; // 故意读取错误类型
        
        std::cout << "\n类型错误状态:" << std::endl;
        std::cout << "  fail(): " << numberFile.fail() << std::endl;
        
        numberFile.close();
    }

    return 0;
}