#include <fstream> // 包含文件流库
#include <iostream> // 用于标准输出和错误输出
#include <string> // 用于使用 std::string

int main() {
    // 打开文件 "my_input_file.txt" 进行读取
    std::ifstream inFile("output.txt"); // 默认模式 std::ios::in

    // 检查文件是否成功打开
    if (inFile.is_open()) {
        std::string line;

        // 1. 逐行读取文件内容
        // std::getline(stream, string_variable) 从指定的输入流读取字符，直到遇到换行符 (默认) 或指定的分隔符
        // 换行符被读取但不存储在字符串中。
        // getline 函数返回流对象本身，在循环条件中使用时，它会在读取成功时评估为 true，
        // 在到达文件末尾 (EOF) 或发生错误时评估为 false。
        while (std::getline(inFile, line)) {
            // 2. 输出读取到的每一行到标准输出
            std::cout << line << std::endl;
        }

        // 3. 关闭文件
        inFile.close();

        std::cout << "the content has been gotten by line" << std::endl;

    } else {
        std::cerr << "错误：无法打开文件进行读取！请确保 my_input_file.txt 文件存在。" << std::endl;
        return 1;
    }

    return 0;
}