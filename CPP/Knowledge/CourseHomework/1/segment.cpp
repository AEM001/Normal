#include "segment.h"
#include <iostream>
#include <cmath>

Segment::Segment() {}
Segment::Segment(double x1, double y1, double x2, double y2) : p1(x1,y1), p2(x2,y2) {}

void Segment::show() {
    std::cout << p1.getx() << ' ' << p1.gety() << ' ';
    std::cout << p2.getx() << ' ' << p2.gety() << ' ';
    double x_dis = p1.getx()-p2.getx();
    double y_dis = p1.gety()-p2.gety();
    std::cout << sqrt(x_dis*x_dis + y_dis*y_dis) << std::endl;
}

void Segment::setP1(double a, double b) {
    p1.setx(a);
    p1.sety(b);
}

void Segment::getP1(double &a, double &b) {
    a = p1.getx();
    b = p1.gety();
}

void Segment::getP2(Point &pt) {
    pt.setx(p2.getx());
    pt.sety(p2.gety());
}
