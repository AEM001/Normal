#include <fstream>
#include <string>
#include <iostream> // 用于输出读取的内容和错误信息

int main() {
    // 创建一个ifstream对象并关联到文件 "input.txt"
    std::ifstream inFile("output.txt");

    // 检查文件是否成功打开
    if (inFile.is_open()) {
        std::string line;
        // 逐行从文件读取数据，直到文件末尾
        while (std::getline(inFile, line)) {
            // 将读取到的行输出到标准输出
            std::cout << line << std::endl;
        }

        // 关闭文件
        inFile.close();

        std::cout << "数据已从 input.txt 读取完毕。" << std::endl;
    } else {
        std::cerr << "无法打开文件进行读取！请确保 input.txt 文件存在。" << std::endl;
        return 1; // 返回非零表示出错
    }

    return 0;
}