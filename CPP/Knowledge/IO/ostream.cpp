#include<iostream>
#include <fstream>
#include <string>

int main() {
    // 创建一个ofstream对象并关联到文件 "output.txt"
    // 如果文件不存在，则创建；如果文件已存在，则清空内容
    std::ofstream outFile("output.txt");

    // 检查文件是否成功打开
    if (outFile.is_open()) {
        // 向文件写入数据
        outFile << "这是一行文本。" << std::endl;
        outFile << "这是另一行数字：" << 12345 << std::endl;
        outFile << "还可以写入字符串: " << "Hello File!" << std::endl;

        // 关闭文件
        outFile.close();

        std::cout << "数据已成功写入 output.txt" << std::endl;
    } else {
        std::cerr << "无法打开文件进行写入！" << std::endl;
        return 1; // 返回非零表示出错
    }

    return 0;
}