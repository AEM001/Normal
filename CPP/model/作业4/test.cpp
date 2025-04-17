#include<iostream>
#include "seqlist.h"
#include "Complex.h"
using namespace std;

int main()
{
	// //（1）实例化为实型数组
	// SeqList<double, 10> dArr;            //构造空表

	// cout << "dArr:  " << dArr << endl;    //输出结果：dArr:  空表	

	// cout << endl;

	// cout << "...建立数据表..." << endl;
	// double x;
	// cout << "读入5个实数：";
	// for (int i = 0; i < 5; i++)          //将5个实数分别按降序和读入顺序放入表中
	// {
	// 	cin >> x;                        //读入一个实数
	// 	dArr.insert(x, i);               //实数插入在下标i处
	// }

	// cout << "...输出数组..." << endl;
	// cout << "dArr：" << dArr << endl;

	// cout << endl;

	// cout << "...查找和删除元素..." << endl;
	// double dkey;                         //待查找数据
	// cout << "输入待查找实数：";
	// cin >> dkey;

	// if (dArr.del(dkey) == false)
	// 	cout << "数组无此元素！" << endl;
	// cout << dArr << endl;

	// cout << endl << endl;
	

	//（2）实例化为复数数组
	SeqList<Complex, 10> cDesArr;          //构造降序空表

	cout << "cDesArr:  " << cDesArr << endl;     //输出结果：dDesArr:  空表

	cout << endl;

	cout << "...建立降序表..." << endl; 
	Complex c;
	cout << "读入5个实数：";
	for (int i = 0; i < 5; i++)          //将5个复数分别按降序和读入顺序放入表中
	{
		cin >> c;                        //读入一个复数
		cDesArr.insert(c);               //复数按降序插入到数组中
	}

	cout << "...输出降序表..." << endl;
	cout << "cDesArr：" << cDesArr << endl;

	cout << endl;

	cout << "...查找元素并修改..." << endl;
	Complex ckey;                        //待查找数据
	cout << "输入待查找复数";
	cin >> ckey;
	int pos;

	pos = cDesArr.biSearch(ckey);       //查找，返回下标
	if (pos == -1) cout << "数组无此元素！" << endl;
	else
	{
		cout << "读入一个复数作为修改该元素的值：";
		cin >> cDesArr[pos];
		cout << "重新排序" << endl;
		cDesArr.sort();
		cout << cDesArr << endl;
	}

	system("pause");
	return 0;
}