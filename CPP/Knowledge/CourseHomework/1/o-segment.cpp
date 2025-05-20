#include<iostream>
#include"point.cpp"
using namespace std;
class Segment{
Point p1,p2;
public:
    Segment(){};
    Segment(double x1,double y1,double x2,double y2) : p1(x1,y1), p2(x2,y2) {
        // 使用成员初始化列表正确初始化p1和p2
    }
        // Point(x1,y1);
        // Point(x2,y2);
        
    
    void show(){
        // cout<<p1.getx()<<' '<<p1.gety();
        // 需要检类似这样的细节，最后检查的方法，就是对照着一行行问自己有没有实现
        cout<<p1.getx()<<' '<<p1.gety()<<' ';  // 显示p1坐标
        cout<<p2.getx()<<' '<<p2.gety()<<' ';  // 显示p2坐标
        // 计算并显示线段长度
        double x_dis = p1.getx()-p2.getx();
        double y_dis = p1.gety()-p2.gety();
        cout<<sqrt(x_dis*x_dis+y_dis*y_dis)<<endl;
        
    }
    void setP1(double a,double b)
    {
        p1.setx(a);
        p1.sety(b);
    }
    void getP1(double &a,double &b)
    {
        a=p1.getx();
        b=p1.gety();
    }
    void getP2(Point & pt)
    {
        pt.setx(p2.getx());
        pt.sety(p2.gety());
        
    }
};