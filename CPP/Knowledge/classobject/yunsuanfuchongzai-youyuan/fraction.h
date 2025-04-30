#ifndef FRACTION_H
#define FRACTION_H
#include <istream>
#include <ostream>
#include <iostream>
using namespace std;

class fraction {
    int fz, fm;
public:
    fraction();                           // 默认构造函数
    fraction(int, int);                   // 带参数构造函数
    friend istream& operator>>(istream &in, fraction &); // 重载输入运算符
    friend ostream& operator<<(ostream &out, const fraction &f); // 重载输出运算符
    friend fraction operator+(fraction, fraction);  // 重载+运算符：分数 + 分数
    friend fraction operator+(fraction, int);       // 重载+运算符：分数 + 整数
    friend fraction operator/(fraction, fraction);  // 重载/运算符：分数 / 分数
    operator double() const;                   // 重载转换为double类型运算符
    fraction operator++();                     // 前置++运算符
    fraction operator++(int);                  // 后置++运算符
    void set(int, int);                        // 设置分子和分母
    int getmax(int, int);                      // 求最大公约数
    void simplify();                           // 简化分数
};

#endif // FRACTION_H