#include <fstream> // 包含文件流库
#include <iostream> // 用于标准输出

int main() {
    // 创建一个ofstream对象并打开文件 "truncate_test.txt"
    // 使用 std::ios::trunc 模式打开文件
    // std::ios::trunc (truncate): 如果文件已存在，则在打开时清空文件内容。
    // 此模式通常与 std::ios::out 或 std::ios::in | std::ios::out 结合使用。
    // 对于 ofstream 默认就是 std::ios::out | std::ios::trunc。
    std::ofstream outFile("truncate_test.txt", std::ios::out | std::ios::trunc);
    // 检查文件是否成功打开
    if (outFile.is_open()) {
        // 写入少量数据
        outFile << "this the content after cleared" << std::endl;

        // 关闭文件
        outFile.close();

        std::cout << "data has been cleared and rewrited truncate_test.txt" << std::endl;

    } else {
        std::cerr << "错误：无法打开文件进行写入！" << std::endl;
        return 1;
    }

    return 0;
}
