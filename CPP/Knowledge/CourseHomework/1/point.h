#ifndef POINT_H
#define POINT_H

class Point {
    double x, y;
public:
    Point();
    Point(double a, double b);
    double dis(double a, double b);
    double dis(Point other);
    double getx();
    double gety();
    void setx(double a);
    void sety(double b);
};

#endif
