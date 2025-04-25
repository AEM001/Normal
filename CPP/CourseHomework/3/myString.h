#ifndef MYSTRING_H
#define MYSTRING_H

#include <iostream>
#include <cstring>
#include <stdexcept>

class myString {
    char *str;
    
public:
    // 默认构造函数
    myString() : str(nullptr) {
        str = new char[1];
        str[0] = '\0';
    }

    // 构造函数（深拷贝）
    myString(const char* s) {
        if (s == nullptr) {
            str = new char[1];
            str[0] = '\0';
        } else {
            str = new char[strlen(s) + 1];
            strcpy(str, s);
        }
    }

    // 析构函数
    ~myString() {
        delete[] str;
    }

    // 拷贝构造函数
    myString(const myString &other) {
        str = new char[strlen(other.str) + 1];
        strcpy(str, other.str);
    }

    // 移动构造函数 (C++11)
    myString(myString&& other) noexcept : str(other.str) {
        other.str = nullptr;
    }

    // 赋值运算符重载
    myString& operator=(const char* s) {
        if (s == nullptr) {
            delete[] str;
            str = new char[1];
            str[0] = '\0';
        } else {
            char* temp = new char[strlen(s) + 1];
            delete[] str;
            str = temp;
            strcpy(str, s);
        }
        return *this;
    }

    myString& operator=(const myString &other) {
        if (this != &other) {
            char* temp = new char[strlen(other.str) + 1];
            delete[] str;
            str = temp;
            strcpy(str, other.str);
        }
        return *this;
    }

    // 移动赋值运算符 (C++11)
    myString& operator=(myString&& other) noexcept {
        if (this != &other) {
            delete[] str;
            str = other.str;
            other.str = nullptr;
        }
        return *this;
    }

    // 获取字符串长度
    size_t length() const {
        return str ? strlen(str) : 0;
    }

    // 判断子串
    int isIn(const myString &other) const {
        if (other.str == nullptr) return -1;
        
        size_t len1 = length();
        size_t len2 = other.length();
        if (len2 > len1) return -1;

        for (size_t i = 0; i <= len1 - len2; i++) {
            if (strncmp(str + i, other.str, len2) == 0)
                return static_cast<int>(i);
        }
        return -1;
    }

    // 子串提取
    myString subStr(size_t start, size_t length) const {
        if (start >= this->length()) {
            throw std::out_of_range("起始位置超出字符串长度");
        }
        
        if (start + length > this->length()) {
            length = this->length() - start;
        }
        
        char* temp = new char[length + 1];
        strncpy(temp, str + start, length);
        temp[length] = '\0';
        myString result(temp);
        delete[] temp;
        return result;
    }

    // +运算符重载
    myString operator+(const myString &other) const {
        char* temp = new char[length() + other.length() + 1];
        strcpy(temp, str);
        strcat(temp, other.str);
        myString result(temp);
        delete[] temp;
        return result;
    }

    // []运算符重载
    char operator[](size_t index) const {
        if (index >= length()) {
            throw std::out_of_range("下标越界");
        }
        return str[index];
    }

    // 输入输出运算符重载
    friend std::ostream& operator<<(std::ostream& os, const myString &s) {
        if (s.str) os << s.str;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, myString &s) {
        std::string temp;
        is >> temp;
        s = temp.c_str();
        return is;
    }

    // 比较运算符重载
    friend bool operator>(const myString &s1, const myString &s2) {
        return strcmp(s1.str, s2.str) > 0;
    }
    
    friend bool operator==(const myString &s1, const myString &s2) {
        return strcmp(s1.str, s2.str) == 0;
    }
};

#endif // MYSTRING_H
