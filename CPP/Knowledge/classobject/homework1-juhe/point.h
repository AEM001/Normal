#include<cmath>
class Point
{double x,y;
public:
Point(double a=0,double b=0):x(a),y(b){}
double dis(double &a,double & b)
{return sqrt((a-x)*(a-x)+(b-y)*(b-y));}
double dis(Point &a)
{
    return sqrt((a.x-x)*(a.x-x)+(a.y-y)*(a.y-y));
}
double getx(){return x;}
double gety(){return y;}
void setx(double a){x=a;}
void sety(double b){y=b;}
};