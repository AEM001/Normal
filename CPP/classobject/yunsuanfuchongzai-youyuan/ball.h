#ifndef BALL_H
#define BALL_H
#include "fraction.h"
#include <string>
#include <iomanip>

class BallGroup {
public:
    std::string color; 
    int num;
    fraction ratio;
    static int total;
    BallGroup();
    BallGroup(std::string c, int n);
    void calRatio();
    void setNum(int a);
};

std::istream& operator>>(std::istream &in, BallGroup &b);
std::ostream& operator<<(std::ostream &out, const BallGroup &b);

#endif // BALL_H