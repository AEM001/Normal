#include<iostream>
using namespace std;

class vector2{
double x,y;
public:
    vector2(double a=0,double b=0):x(a),y(b){}
    double getx(){return x;}
    double gety(){return y;}

    vector2& operator+(vector2& b)
    {x+=b.getx();
    y+=b.gety();
    return *this;}

    friend bool operator ==(const vector2& a,const vector2& b)
    {return a.x==b.x&&a.y==b.y;}

    vector2& operator*(double n)
    {x*=n;
    y*=n;
    return *this;}

    friend vector2& operator *(double n,vector2& a)
    {a.x*=n;
    a.y*=n;
    return a;}

    friend ostream& operator<<(ostream& os,const vector2& a)
    {os<<a.x<<' '<<a.y<<endl;}
};

int main(){
    vector2 a(1.0, 2.0), b(3.0, 4.0);
    a + b;         // (4.0, 6.0)
    cout << a << endl;          // 输出 (4, 6)
    vector2 d = a * 2.0;       // (2.0, 4.0)
    vector2 e = 2.0 * a;       // (2.0, 4.0)
    cout << (a == vector2(4, 6)); // 输出 1

}