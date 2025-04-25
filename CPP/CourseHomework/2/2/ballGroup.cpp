#include "ball.h"

int ballgroup::total = 0;  // 在这里定义

ballgroup::ballgroup(std::string c, int n):color(c),num(n){
    total+=n;
}
std::istream& operator>>(std::istream& is,ballgroup &b)
{
    is>>b.color>>b.num;
    return is;
}
std::ostream& operator<<(std::ostream& os,ballgroup &b)
{
    os<<b.color<<' '<<b.num<<' '<<b.calRatio();
    return os;
}
void ballgroup::setNum(int newnum)
{
    total-=num;
    num=newnum;
    total+=num;
}