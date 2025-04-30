#include <iostream>
#include"myString.h"
using namespace std;

int main()
{
	myString s1("World"), s2, s3, s5;   //缺省构造为空串	                                                                                     
	cout << "s1: " << s1 << endl;       //输出： World
	
	char cstr[20] = "Hello";   
	s2 = cstr;                          //重载赋值运算符，用C风格串为对象赋值
	cout << "s2: " << s2 << endl;       //输出：Hello
		                                    
	cout << "读入一个字符串：";
	cin >> s3;
	cout << "s3: " << s3 << endl;       

	s3 = s2 + s1;                       //重载 + 运算符，将s1拼接到s2后面
	cout << "s3 = s2+s1: " << s3 << endl;        //输出结果：HelloWorld

	cout << "s1[2] = " << s1[2] << endl;         //重载下标运算符[]，输出：r 
	cout << "s1[20] = " << s1[20] << endl;       //提示下标越界，同时获得‘？’字符   

	myString s4("el");                    

	int pos = s2.isIn(s4);             //s4是否是s2的子串，是则返回子串s4首字母在s2中的下标位置，否则返回-1
	if (pos != -1)
		cout << s4 << " 是 " << s2 << " 子串，首字母下标： " << pos << endl;   //输出：el 是 Hello子串，首字母下标：1
	else 
		cout << s4 << " 不是 " << s2 << "子串 " << endl;
	
	s5 = s3.subStr(2, 3);              //从下标为2处开始，提取长度为3的子串。如果子串长度超过原串范围，则给出提示，并返回"?"串
	cout << s5 << endl;                //输出结果：llo  

	s5 = s3.subStr(7, 6);              //测试字串超限，
	cout << s5 << endl;                //输出结果：？，提示子串超限  

	if (s1 > s2) swap(s1, s2);         //重载 >运算符, 按字典序比较。
	if (s2 > s3) swap(s2, s3);
	if (s1 > s2) swap(s1, s2);
	
	cout << "s1,s2,s3升序排列：" << s1 << "   " << s2 << "   " << s3 << endl;    //输出结果：Hello  HelloWorld  World
	
	system("pause");
	return 0;
}

//在此处添加代码：定义字符串交换函数
void swap(myString &s1, myString &s2)
{
	myString temp = s1;
	s1 = s2;
	s2 = temp;
}