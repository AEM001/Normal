#include "segment.h"
#include <iostream>
using namespace std;

int main() {
    Segment s1(2, 2, 8, 10);
    s1.show();
    s1.setP1(2, 1);
    s1.show();

    double x1, y1;
    s1.getP1(x1, y1);
    Point px(-3, 2);
    cout<< px.dis(x1, y1) << endl;

    Point p2;
    s1.getP2(p2);
    cout << px.dis(p2) << endl;

    return 0;
}