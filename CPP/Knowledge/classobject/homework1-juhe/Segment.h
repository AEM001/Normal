#include"point.h"
#include<iostream>
using namespace std;
class Segment
{
    Point p1,p2;
    public:
    Segment(double a=0,double b=0,double c=0,double d=0):p1(a,b),p2(c,d){}
    void show()
    {cout<<p1.getx()<<p1.gety();
        double x_dis=p1.getx()-p2.getx();
        double y_dis=p1.gety()-p2.gety();
        cout<<sqrt(x_dis*x_dis+y_dis*y_dis)<<endl;}
    void setP1(double a,double b)
    {
        p1.setx(a);
        p1.sety(b);
    }
    void getP1(double &a,double &b)
    {a=p1.getx();
     b=p1.gety();
    
    }
    void getP2(Point &a)
    {
        a.setx(p2.getx());
        a.sety(p2.gety());
    }
};