#include <fstream> // 包含文件流库
#include <iostream> // 用于标准输出

int main() {
    // 创建一个ofstream对象并打开文件 "append_test.txt"
    // 使用 std::ios::app 模式打开文件
    // std::ios::app (append): 在写入之前，将文件写入位置移动到文件末尾。所有写入操作都会追加到文件末尾。
    // 如果文件不存在，仍会创建文件。
    // std::ios::out 模式是默认包含的，但明确写出可以提高可读性。
    std::ofstream outFile("output.txt", std::ios::out | std::ios::app);

    // 检查文件是否成功打开
    if (outFile.is_open()) {
        // 写入数据
        outFile << "append 1" << std::endl;
        outFile << "append again" << std::endl;

        // 关闭文件
        outFile.close();

        std::cout << "data has been successfully appended to append_test.txt" << std::endl;

    } else {
        std::cerr << "错误：无法打开文件进行追加写入！" << std::endl;
        return 1;
    }

    return 0;
}