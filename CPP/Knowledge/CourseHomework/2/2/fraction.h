#ifndef FRACTION_H
#define FRACTION_H

#include<cmath>
#include<iostream>
#include<ostream>
#include<istream>
using namespace std;

class fraction
{
	int fz, fm;
	
public:
	fraction(int a = 0, int b = 1);  // 默认参数只在声明中指定
	
	friend istream& operator>>(istream& is,fraction & frac);
	friend ostream& operator<<(ostream& os,fraction & frac);
	void set(int,int);
	friend fraction operator+(fraction,int);
	friend fraction operator+(fraction,fraction);
	friend fraction operator/(fraction,fraction);
	friend fraction operator++(fraction&);
	friend fraction operator++(fraction&,int);
	operator double() const;  // 修改为类型转换运算符
};

void reduce(int &fz,int &fm);

#endif