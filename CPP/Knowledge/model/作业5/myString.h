#include <iostream>
#include <cstring>
using namespace std;

class myString {
    char *str;
    
public:
    // 默认构造函数
    myString() {
        str = new char[1];  // 申请最小内存
        str[0] = '\0';
    }

    // 构造函数（深拷贝）
    myString(const char* s) {

        str = new char[strlen(s) + 1];
        strcpy(str, s);
    }

    // 析构函数
    ~myString() {
        delete[] str;
    }

    // 拷贝构造函数
    myString(const myString &other) {
        if(other.str == nullptr) {  // 添加空指针检查
            str = new char[1];
            str[0] = '\0';
        } else {
            str = new char[strlen(other.str) + 1];
            strcpy(str, other.str);
        }
    }

    // 赋值运算符重载（深拷贝)
    myString& operator=(const char* s) {
        if (str) delete[] str;  // 释放旧内存
        str = new char[strlen(s) + 1];
        strcpy(str, s);
        return *this;
    }

    myString& operator=(const myString &other) {
        if (this != &other) {
            delete[] str;
            if(other.str == nullptr) {  // 添加空指针检查
                str = new char[1];
                str[0] = '\0';
            } else {
                str = new char[strlen(other.str) + 1];
                strcpy(str, other.str);
            }
        }
        return *this;
    }

    // 判断子串
    int isIn(const myString &other) {
        int len1 = strlen(str);
        int len2 = strlen(other.str);
        if (len2 > len1) return -1;

        for (int i = 0; i <= len1 - len2; i++) {
            if (strncmp(str + i, other.str, len2) == 0)
                return i;
        }
        return -1;
    }

    // 子串提取
    myString subStr(int a, int b) {
        if (a + b > strlen(str)) {
            cout << "子串超限" << endl;
            return myString("?");
        }
        char *s = new char[b + 1];
        strncpy(s, str + a, b);
        s[b] = '\0';
        myString temp(s);
        delete[] s;
        return temp;
    }

    // +运算符重载
    myString operator+(const myString &other) const {
        char *s = new char[strlen(str) + strlen(other.str) + 1];
        strcpy(s, str);
        strcat(s, other.str);
        myString temp(s);
        delete[] s;
        return temp;
    }

    // []运算符重载
    char operator[](int i) const {
        if (i >= strlen(str)) {
            cout << "下标越界" << endl;
            return '?';
        }
        return str[i];
    }

    // 输入输出运算符重载
    friend ostream& operator<<(ostream& os, const myString &s) {
        os << s.str;
        return os;
    }

    friend istream& operator>>(istream& is, myString &s) {
        char temp[1000];  // 假设最多输入1000个字符,避免内存泄漏
        is >> temp;
        s = temp;  // 使用 `operator=`
        return is;
    }

    // >运算符重载
    friend bool operator>(const myString &s1, const myString &s2) {
        return strcmp(s1.str, s2.str) > 0;
    }
};
