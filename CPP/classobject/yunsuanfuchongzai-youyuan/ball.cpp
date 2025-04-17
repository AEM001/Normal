#include "ball.h"

int BallGroup::total = 0;

BallGroup::BallGroup() : color(""), num(0) {
    ratio = fraction(0, 1); // 初始化比例为0
}

BallGroup::BallGroup(std::string c, int n) : color(c), num(n) {
    total += num;
    calRatio(); // 更新后重新计算比例
}

std::istream& operator>>(std::istream &in, BallGroup &b) {
    in >> b.color >> b.num;
    BallGroup::total += b.num;
    b.calRatio();
    return in;
}

std::ostream& operator<<(std::ostream &out, const BallGroup &b) {
    out << b.color << std::setw(10) << b.num << std::setw(10) << BallGroup::total << std::setw(10) << b.ratio;
    return out;
}

void BallGroup::calRatio() {
    if (total == 0) {
        ratio = fraction(0, 1); // Prevent division by zero
    } else {
        ratio = fraction(num, total);
    }
}

void BallGroup::setNum(int a) {
    total = total - num + a;
    num = a;
    calRatio();
}