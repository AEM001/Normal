#ifndef SEGMENT_H
#define SEGMENT_H
#include "point.h"

class Segment {
    Point p1, p2;
public:
    Segment();
    Segment(double x1, double y1, double x2, double y2);
    void show();
    void setP1(double a, double b);
    void getP1(double &a, double &b);
    void getP2(Point &pt);
};

#endif
