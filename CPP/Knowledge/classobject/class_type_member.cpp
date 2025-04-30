#include <iostream>
#include<cmath>
using namespace std;
// 当包含类类型的成员时，如果该成员不存在默认构造函数，那么必须通过参数列表初始化
class point{
private:
    double a,b;
public:
    point(double a,double b) : a(a),b(b) {}
   double dis(point other)const{
        double dx=a-other.a; double dy=b-other.b;
        return sqrt(dx*dx+dy*dy);
    }
};
class segment{
    point m;
public:
    segment(double x,double y):m(x,y){}

};