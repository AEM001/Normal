# C++ 文件输入输出操作规范

根据您提供的代码示例，我整理了C++文件输入输出的主要语法规范和用法，方便您在需要时快速查阅。

## 1. 文件流类型

### 头文件
```cpp
#include <fstream>  // 包含文件流库
#include <iostream> // 用于标准输出和错误输出
#include <string>   // 用于使用std::string
```

### 主要文件流类
- `std::ofstream`: 输出文件流，用于写入文件
- `std::ifstream`: 输入文件流，用于读取文件
- `std::fstream`: 同时支持读写的文件流

## 2. 打开文件

### 使用构造函数打开
```cpp
std::ofstream outFile("output.txt");                      // 写入模式
std::ifstream inFile("input.txt");                        // 读取模式
std::fstream file("file.txt", std::ios::in | std::ios::out); // 读写模式
```

### 使用open()方法打开
```cpp
std::ofstream outFile;
outFile.open("output.txt", std::ios::out);
```

## 3. 文件打开模式

| 模式 | 描述 |
|------|------|
| `std::ios::in` | 读取模式（ifstream默认） |
| `std::ios::out` | 写入模式（ofstream默认） |
| `std::ios::app` | 追加模式，在文件末尾添加内容 |
| `std::ios::trunc` | 截断模式，打开时清空文件内容（ofstream默认） |
| `std::ios::binary` | 二进制模式 |
| `std::ios::ate` | 打开后立即定位到文件末尾 |

### 模式组合示例
```cpp
// 追加模式
std::ofstream outFile("output.txt", std::ios::out | std::ios::app);

// 截断模式（清空文件内容后写入）
std::ofstream outFile("truncate_test.txt", std::ios::out | std::ios::trunc);
```

## 4. 检查文件是否成功打开

```cpp
if (file.is_open()) {
    // 文件成功打开，执行操作
} else {
    std::cerr << "错误：无法打开文件！" << std::endl;
    return 1;
}
```

## 5. 文件写入操作

### 使用插入运算符 `<<`
```cpp
outFile << "Hello, world" << std::endl;
outFile << "第二行" << std::endl;
outFile << 42 << std::endl;  // 写入数字
```

## 6. 文件读取操作

### 使用提取运算符 `>>`（按空白分隔读取）
```cpp
std::string word1, word2;
int number;

inFile >> word1;  // 读取第一个"单词"
inFile >> word2;  // 读取第二个"单词"
inFile >> number; // 读取一个整数
```

### 使用 `getline()` 逐行读取
```cpp
std::string line;
while (std::getline(inFile, line)) {
    std::cout << line << std::endl;
}
```

## 7. 文件指针操作

### 获取当前位置
```cpp
// 获取读取指针位置
std::streampos readPos = file.tellg();

// 获取写入指针位置
std::streampos writePos = file.tellp();
```

### 移动文件指针
```cpp
// 移动读取指针
file.seekg(0, std::ios::beg);      // 移动到文件开头
file.seekg(10, std::ios::beg);     // 移动到距离文件开头10个字节处
file.seekg(-5, std::ios::end);     // 移动到距离文件末尾5个字节处
file.seekg(0, std::ios::end);      // 移动到文件末尾

// 移动写入指针
file.seekp(0, std::ios::beg);      // 移动到文件开头
```

## 8. 文件状态检查

```cpp
// 检查文件状态
file.good();  // 如果流状态良好（无错误），返回true
file.fail();  // 如果发生格式错误或提取失败，返回true
file.eof();   // 如果到达文件末尾，返回true
file.bad();   // 如果发生严重的I/O错误，返回true
```

## 9. 关闭文件

```cpp
file.close();
```

## 10. 完整读写示例

### 写入文件示例
```cpp
std::ofstream outFile("output.txt");
if (outFile.is_open()) {
    outFile << "第一行" << std::endl;
    outFile << "第二行" << std::endl;
    outFile.close();
    std::cout << "文件写入成功" << std::endl;
} else {
    std::cerr << "无法打开文件进行写入！" << std::endl;
}
```

### 读取文件示例
```cpp
std::ifstream inFile("input.txt");
if (inFile.is_open()) {
    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;
    }
    inFile.close();
} else {
    std::cerr << "无法打开文件进行读取！" << std::endl;
}
```

## 注意事项

1. 始终检查文件是否成功打开
2. 操作完成后关闭文件
3. 使用适当的错误处理机制
4. 对于大文件，考虑使用二进制模式和缓冲区读写
5. 文件路径可以是相对路径或绝对路径

这份规范涵盖了您提供的示例代码中的主要文件I/O操作，希望对您有所帮助！

        