#include "point.h"
#include <cmath>

Point::Point() : x(0), y(0) {}
Point::Point(double a, double b) : x(a), y(b) {}

double Point::dis(double a, double b) {
    double x_dis = x-a;
    double y_dis = y-b;
    return sqrt(x_dis*x_dis + y_dis*y_dis);
}

double Point::dis(Point other) {
    double x_dis = x-other.getx();
    double y_dis = y-other.gety();
    return sqrt(x_dis*x_dis + y_dis*y_dis);
}

double Point::getx() { return x; }
double Point::gety() { return y; }
void Point::setx(double a) { x = a; }
void Point::sety(double b) { y = b; }