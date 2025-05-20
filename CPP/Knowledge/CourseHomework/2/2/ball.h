#include <string>
#include<istream>
#include<ostream>
#include"fraction.h"
class ballgroup
{
    std::string color;
    int num;
    static int total;  // 这只是声明
    public:
    ballgroup(){}
    ballgroup(std::string c, int n);  // 声明但不实现
    friend std::istream& operator>>(std::istream& is,ballgroup &b);
    friend std::ostream& operator<<(std::ostream& os,ballgroup &b);
    fraction calRatio(){
        fraction ratio(num,total);
        return ratio;
    }
    void setNum(int);

};