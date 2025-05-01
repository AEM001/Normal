#include <fstream> // 文件流
#include <iostream> // 标准输入输出
#include <string> // 字符串

int main() {
    // 使用 fstream 打开文件，同时支持读写
    std::fstream file("seek_test_revised.txt", std::ios::in | std::ios::out | std::ios::trunc);

    if (file.is_open()) {
        std::string data = "11111111111111111111111111";

        // Helper function: print current file content
        auto printContent = [&]() {
            std::streampos oldPos = file.tellg(); // Save current position
            file.seekg(0, std::ios::beg);
            std::string content((std::istreambuf_iterator<char>(file)), 
                               std::istreambuf_iterator<char>());
            std::cout << "Current file content: " << content << std::endl;
            file.seekg(oldPos); // Restore position
        };

        file.seekp(0, std::ios::beg);
        file << data;
        std::cout << "After initial write: ";
        printContent();

        file.seekp(5, std::ios::beg);
        file << "456";
        std::cout << "After writing '456' at position 5: ";
        printContent();

        file.seekp(-1, std::ios::cur);
        file << "#";
        std::cout << "After writing '#': ";
        printContent();

        // 获取当前写入指针位置
        std::cout << "After writing '" << data << "', put pointer position: " << file.tellp() << std::endl;

        // 将写入位置移动到文件开头 + 5
        file.seekp(5, std::ios::beg);
        file << "456"; // 在位置 5 写入 "123"
        std::cout<< "After writing '123' at position 5, put pointer position: " << data << std::endl;
        // 从当前位置向前移动 2 个字节
        file.seekp(-1, std::ios::cur);
        file << "#"; // 覆盖最后两个字符
        std::cout << "After writing '#' at position -1, put pointer position: " << data << std::endl;
        // 将读取位置移动到文件开头
        file.seekg(0, std::ios::beg);

        // 获取当前读取指针位置
        std::cout << "\nAfter moving get pointer to start, position: " << file.tellg() << std::endl;

        char buffer[10]; // 用于读取数据
        // 从当前读取位置读取 5 个字节
        file.read(buffer, 5);

        // 读取 5 个字节后，读取指针位置
        std::cout << "After reading 5 bytes from start, get pointer position: " << file.tellg() << std::endl;

        // 将读取位置移动到文件开头 + 10
        file.seekg(10, std::ios::beg);
        std::cout << "After moving get pointer to position 10, position: " << file.tellg() << std::endl;

        char singleChar;
        // 从当前读取位置读取一个字符
        file >> singleChar;
        std::cout << "After reading one character ('" << singleChar << "') from position 10, get pointer position: " << file.tellg() << std::endl;

        // 将读取位置移动到文件末尾前 5 个字节
        file.seekg(-5, std::ios::end);
        std::cout << "After moving get pointer to 5 bytes before end, position: " << file.tellg() << std::endl;

        // 关闭文件
        file.close();
        std::cout << "\nFile closed." << std::endl;

    } else {
        std::cerr << "Error: Failed to open file for read/write!" << std::endl;
        return 1;
    }

    // 读取最终文件内容以验证 seekp 的效果
    std::ifstream finalReader("seek_test_revised.txt");
    if (finalReader.is_open()) {
         std::string content((std::istreambuf_iterator<char>(finalReader)),
                              std::istreambuf_iterator<char>());
         std::cout << "\nFinal content of seek_test_revised.txt:\n" << content << std::endl;
         finalReader.close();
    } else {
         std::cerr << "Error: Failed to open seek_test_revised.txt to read final content!" << std::endl;
    }

    return 0;
}