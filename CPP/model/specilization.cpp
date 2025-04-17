#include <iostream>
#include <string>
using namespace std;

// 1. 通用模板类
template <typename T>
class MyProcessor {
public:
    void process(const T& data) const {
        cout << "Default processing: " << data << endl;
    }
};

// 2. 全特化：针对 std::string 类型
template <>
class MyProcessor<string> {
public:
    void process(const string& data) const {
        // 将字符串转换为大写
        string upperData = data;
        for (char& c : upperData) {
            c = toupper(c);
        }
        cout << "String processing: " << upperData << endl;
    }
};

// 3. 偏特化：针对指针类型
template <typename T>
class MyProcessor<T*> {
public:
    void process(T* data) const {
        if (data != nullptr) {
            cout << "Pointer processing: " << *data << endl;
        } else {
            cout << "Pointer is null." << endl;
        }
    }
};

int main() {
    // 测试通用模板
    MyProcessor<int> intProcessor;
    intProcessor.process(42); // 输出：Default processing: 42

    // 测试全特化
    MyProcessor<string> stringProcessor;
    stringProcessor.process("hello"); // 输出：String processing: HELLO

    // 测试偏特化
    double value = 3.14;
    MyProcessor<double*> pointerProcessor;
    pointerProcessor.process(&value); // 输出：Pointer processing: 3.14

    double* nullPtr = nullptr;
    pointerProcessor.process(nullPtr); // 输出：Pointer is null.

    return 0;
}