#include<iostream>
#include"list.h"
#include<cmath>
using namespace std;

// Complex类的完整定义移到main函数之前
class Complex
{
    double re, im;       //实部和虚部
    double len;          //复数的模

public:
    Complex(double re = 0, double im = 0);  // 构造函数声明
    
    // 运算符重载声明
    friend ostream& operator << (ostream& os, const Complex& c);
    friend istream& operator >> (istream& is, Complex& c);
    friend bool operator==(const Complex& c1, const Complex& c2);
    friend bool operator<(const Complex& c1, const Complex& c2);
};

int main()
{
    List<Complex> clist1, clist2;           //创建空的复数链表（不带链头空节点）

    cout << clist1 << endl;
    cout << clist2 << endl;
    cout << endl;
    
    Complex c;
    
    cout << "...建立复数链表..." << endl << endl;
    cout << "输入6组复数，分别按输入顺序建立链表、按复数的模升序建立链表:" << endl;
    for (int i = 0; i < 6; i++)
    {
        cin >> c;                          //读入复数
        clist1.insertRear(c);              //将复数插在链尾
        clist2.insertSort(c);              //建立升序链表
    }
    cout << "按输入顺序建立的链表：" << endl;
    cout << clist1 << endl << endl;
    
    cout << "升序链表：" << endl;
    cout << clist2 << endl << endl;
   
    cout << "...查找一个关键字节点的前节点..." << endl << endl;
    cout << "输入待查找复数：" << endl;
    cin >> c;
    
    Node<Complex>* pFind = 0, *pFront = 0; //关键字节点及其前节点的指针
    pFind = clist1.find(c);
    if (pFind)
    {
        pFront = clist1.findFront(pFind);
        if(pFront)	cout << "节点 " << c << " 的前节点为：" << pFront->info << endl;  //输出前节点值
        else cout << "节点 " << c << " 无前节点" << endl;
    }
    else cout << "链表无节点 " << c << endl << endl;

    cout << "...删除关键字节点..." << endl << endl;
    cout << "输入待查找复数：" << endl;
    cin >> c;
    
    Node<Complex>* pDel = clist1.del(c);  //删除链表中数据c的节点，返回c节点地址
    if (pDel == 0)  cout << "链表无 " << c << " 节点" << endl << endl;
    
    cout << clist1 << endl;                //输出删除节点后的链表
    
    return 0;
}

// Complex类的成员函数和运算符实现
Complex::Complex(double re, double im) : re(re), im(im), len(sqrt(re*re + im*im)) {}

ostream& operator << (ostream& os, const Complex& c)
{
    os << c.re;
    if (c.im > 0) os << " + " << c.im << "i";
    else if (c.im < 0) os << " - " << -c.im << "i";
    return os;
}

istream& operator >> (istream& is, Complex& c)
{
    is >> c.re >> c.im;
    c.len = sqrt((c.re * c.re + c.im * c.im));
    return is;
}

bool operator==(const Complex& c1, const Complex& c2)
{
    return (c1.re == c2.re && c1.im == c2.im);
}

bool operator<(const Complex& c1, const Complex& c2)
{
    return c1.len < c2.len;
}
