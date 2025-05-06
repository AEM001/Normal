#include <fstream> // 文件流
#include <iostream> // 标准输入输出
#include <string> // 字符串

int main() {
    // 使用 fstream 打开文件，同时支持读写
    std::fstream file("seek_test_revised.txt", std::ios::in | std::ios::out | std::ios::trunc);

    if (file.is_open()) {
        std::string data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        // 将写入位置移动到文件开头
        file.seekp(0, std::ios::beg);
        file << data; // 写入数据

        // 获取当前写入指针位置
        std::cout << "After writing '" << data << "', write pointer position: " << file.tellp() << std::endl;

        // 将读取位置移动到文件开头
        file.seekg(0, std::ios::beg);

        // 获取当前读取指针位置
        std::cout << "\nAfter moving read pointer to start, read pointer position: " << file.tellg() << std::endl;

        char buffer[10]; // 用于读取数据
        // 从当前读取位置读取 5 个字节
        file.read(buffer, 5);

        // 读取 5 个字节后，读取指针位置
        std::cout << "After reading 5 bytes from start, read pointer position: " << file.tellg() << std::endl;

        // 将读取位置移动到文件开头 + 10
        file.seekg(10, std::ios::beg);
        std::cout << "After moving read pointer to position 10, read pointer position: " << file.tellg() << std::endl;

        char singleChar;
        // 从当前读取位置读取一个字符
        file >> singleChar;
        std::cout << "After reading one character ('" << singleChar << "') from position 10, read pointer position: " << file.tellg() << std::endl;

        // 将读取位置移动到文件末尾前 5 个字节
        file.seekg(-5, std::ios::end);
        std::cout << "After moving read pointer to 5 bytes before end, read pointer position: " << file.tellg() << std::endl;

        // 关闭文件
        file.close();
        std::cout << "\nFile closed successfully." << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for reading and writing!" << std::endl;
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
         std::cerr << "Error: Unable to open seek_test_revised.txt to read final content!" << std::endl;
    }
    return 0;
}