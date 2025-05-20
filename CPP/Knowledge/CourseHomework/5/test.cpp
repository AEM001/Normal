
#include <iostream>
#include"myString.h"
using namespace std;

int main()
{
	myString s1("World"), s2(s1), s3, s4, s5, s6;          //缺省构造为空串	                                                                                      
	cout << "s1: " << s1 << endl;       //输出： World	
	cout << "s2: " << s2 << endl;       //输出：World
		                                    
	cout << "读入一个字符串：";
	cin >> s3;
	cout << "s3: " << s3 << endl;       

	s4 = s3;                            //重载 = 运算符
	cout << "s4: " << s4 << endl;          

	myString s5("rl");                    

	int pos = s2.isIn(s5);             //s5是否是s2的子串，是则返回子串s5首字母在s2中的下标位置，否则返回-1
	if (pos ！ = -1)
		cout << s5 << " 是 " << s2 << " 子串，首字母下标： " << pos << endl;   
	else 
		cout << s5 << " 不是 " << s2 << "子串 " << endl;
	
	s6 = s3.subStr(2, 3);              //从下标为2处开始，提取长度为3的子串。如果子串长度超过原串范围，则给出提示，并返回"?"串
	cout << s6 << endl;                //输出结果：llo  

	s6 = s3.subStr(7, 6);              //测试字串超限，
	cout << s6 << endl;                //输出结果：？，提示子串超限  

	system("pause");
	return 0;
}

