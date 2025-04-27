#include<iostream>
#include<vector>
using namespace std;
class Shape{
public:
    virtual ~Shape() = default;
	virtual void area()=0;
	virtual void print()=0;
};

class Circle:public Shape{
double r,s;
public:
	Circle(double input):r(input){}
	void area()override{s=3.14*r*r;}
	void print()override{
    area();    
    cout<<"Circle"<<' '<<s<<endl;}
};

class Rectangle:public Shape{
double a,b,s;
public:
	Rectangle(double in1,double in2):a(in1),b(in2){}
	void area()override{s=a*b;}
	void print()override{
    area();    
    cout<<"Rectangle"<<' ' <<s<<endl;}
};

int main(){
    vector<Shape*> fold;
    
    // 创建对象并添加到vector
    fold.push_back(new Circle(3));
    fold.push_back(new Rectangle(2,3));
    
    // 遍历打印
    for(auto x : fold){
        x->print();
    }
    
    // 释放内存
    for(auto x : fold){
        delete x;
    }
    
    return 0;
}