#include <iostream>
#include <cstring>

class MyString {
private:
    char* str;
    int length;
public:
    // 构造函数
    MyString(const char* s = "") {
        // 请在此处补充代码
        length = strlen(s);
        str = new char[length + 1];
        strcpy(str, s);
    }

    // 拷贝构造函数
    MyString(const MyString& other) {
        // 请在此处补充代码
        length = other.length;
        str = new char[length + 1];
        strcpy(str, other.str);

    }

    // 赋值运算符
    MyString& operator=(const MyString& other) {
        // 请在此处补充代码
        if (this != &other) {
            delete[] str;
            length = other.length;
            str = new char[length + 1]; 
            strcpy(str, other.str);
        }
        

        return *this;
    }

    // 字符串拼接
    MyString operator+(const MyString& other) const {
        // 请在此处补充代码
        MyString result;
        result.length = length + other.length;
        result.str = new char[result.length + 1];
        strcpy(result.str, str);
        strcat(result.str, other.str);
        return result;
    }

    // 字符串比较
    bool operator==(const MyString& other) const {
        // 请在此处补充代码
        return strcmp(str, other.str) == 0;

    }

    // 获取C风格字符串
    const char* c_str() const {
        // 请在此处补充代码
        return str;

    }

    // 获取字符串长度
    int size() const {
        // 请在此处补充代码
        return length;

    }

    // 析构函数
    ~MyString() {
        // 请在此处补充代码
        delete[] str;
        
    }
};

// 测试用例
int main() {
    MyString s1("Hello");
    MyString s2("World");
    
    MyString s3 = s1 + " " + s2;
    std::cout << s3.c_str() << std::endl; // 应输出: Hello World
    
    if (s1 == "Hello") {
        std::cout << "s1 equals 'Hello'" << std::endl;
    }
    
    MyString s4;
    s4 = s3;
    std::cout << s4.size() << std::endl; // 应输出: 11
    
    return 0;
}
