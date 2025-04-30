#include <iostream>
#include <cstring> // for strcpy, strlen

class String {
private:
    char* data; // 动态分配的字符数组
    int length; // 字符串长度

public:
    // 普通构造函数
    String(const char* str = "") {
        length = strlen(str);
        data = new char[length + 1]; // +1 用于存放 '\0'
        strcpy(data, str);
    }

    // TODO 1: 拷贝构造函数（深拷贝）
    String(const String& other) {
        // 补充代码
        data=new char[other.length+1];
        strcpy(data,other.data);
        length=other.length;
    }

    // TODO 2: 拷贝赋值运算符（深拷贝）
    String& operator=(const String& other) {
        // 补充代码
        if(this==&other)
        {
            return *this;
        }
        delete[]data;
        length=other.length;
        data=new char[length+1];
        strcpy(data,other.data);
        return *this;
    }

    // 析构函数
    ~String() {
        delete[] data;
    }

    // 打印字符串
    void print() const {
        std::cout << data << std::endl;
    }
};

int main() {
    String s1("Hello");
    String s2 = s1; // 调用拷贝构造函数
    String s3;
    s3 = s1;        // 调用拷贝赋值运算符

    s1.print(); // 输出: Hello
    s2.print(); // 输出: Hello
    s3.print(); // 输出: Hello

    return 0;
}