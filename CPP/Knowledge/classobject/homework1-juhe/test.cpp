#include "segment.h"
#include <iostream>

using namespace std;

int main()
{ 
    Segment s1(2, 2, 8, 10);              // 线段s1
    s1.show();                            // 输出s1端点坐标和长度
    s1.setP1(2, 1);                       // 修改s1的端点1坐标
    s1.show();

    double x1, y1;
    s1.getP1(x1, y1);                     // 获取s1的端点1坐标
    Point px(-3, 2);                      // 点px
    cout << px.dis(x1, y1) << endl;       // 计算并输出px和端点1的距离

    Point p2;                             // 点p2
    s1.getP2(p2);                         // 用p2获取s1的端点2的值
    cout << px.dis(p2) << endl;           // 计算输出px和p2的距离           

    return 0;
}