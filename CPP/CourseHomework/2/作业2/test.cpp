#include<iostream>
#include "fraction.h"
#include "ball.h"
using namespace std;

int main()
{
	//测试分数类       
	Fraction f1(1, 1), f2, f3;         //创建分数（分子、分母），得到化简的分数
	
	cout << "读入分子、分母值：";
	cin >> f2;                         //重载提取运算符，从键盘读入分数
	cout << "f2 = " << f2 << endl;     //重载插入运算符，输出分数值，形式为“分子/分母”，且符合题目要求的格式

	f1.set(2, -6);                     //修改f1的值	
	cout << "f1 = " << f1 << endl;     //输出分数

	int k = 3;
	cout << "k = " << k << endl;
	f3 = f1 + k;                       //重载+，分数+整数
	cout << f1 << " + " << k << " = " << f3 << endl;

	f3 = f1 + f2;                     //重载+，分数+分数
	cout << f1 << " + " << f2 << " = " << f3 << endl;

	f3 = f1 / f2;                     //重载/，分数相除，对除数为0的情况做出提示和恰当处理
	cout << f1 << "   /   " << f2 << " = " << f3 << endl;

	f3 = f2++;                       //重载后置++：分子+1、分母不变
	cout << "f2 =" << f2 << ",  f2++ = " << f3 << endl;

	f3 = ++f2;                       //重载前置++：分子+1、分母不变
	cout << "f2 =" << f2 << ",  ++f2 = " << f3 << endl;

	double r = f2;                   //分数转换为实数（即分子/分母的结果）
	cout << "f2 = " << f2 << ",  r = " << r << endl;

	/****************************************************/

	//测试小球组
	BallGroup ballgroup1, ballgroup2("yellow", 8), ballgroup("blue", 10);    //定义3种颜色小球

	cout << "读入第一组小球的颜色和个数：";
	cin >> ballgroup1;             //需在BallGroup中重载提取运算符

	ballgroup2.calRatio();         //重新计算其他颜色小球的占比
	ballgroup3.calRatio();

	cout << "输出3组小球数据：" << endl;
	cout << setw(10) << "color" << setw(10) << "num" << setw(10) << "total" << setw(10) << "ratio" << endl;
	cout << ballgroup1 << endl;    //需在BallGroup中重载插入运算符
	cout << ballgroup2 << endl;
	cout << ballgroup3 << endl;

	cout << "修改黄色组小球个数" << endl;
	ballgroup2.setNum(20);

	ballgroup1.calRatio();         //重新计算其他颜色小球的占比
	ballgroup3.calRatio();

	cout << "输出修改后3组小球数据：" << endl;
	cout << setw(10) << "color" << setw(10) << "num" << setw(10) << "total" << setw(10) << "ratio" << endl;
	cout << ballgroup1 << endl;
	cout << ballgroup2 << endl;
	cout << ballgroup3 << endl;

	return 0;
}
	