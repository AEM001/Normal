#include <iostream>
#include<vector>
using namespace std;
#define PI 3.14

class Shape{

    public:
        virtual double area()const=0;
        virtual double perimeter()const=0;
        virtual  void display()const{}
        virtual ~Shape(){}
};
class Rectangle :public Shape{
    double width,height;
    public:
        Rectangle(double a,double b):width(a),height(b){}
        double area()const override{//important, const override
            return width*height;
        }
        double perimeter()const override{
            return 2*width*height;
        }
        void display()const override{
            cout<<width<<' '<<height<<area()<<endl;
        }
};
class Circle :public Shape{
    double r;
    public:
        Circle(double x):r(x){}
    double area()const override{
        return PI*r*r;
    }
    double perimeter()const override{
        return PI*2*r;
    }
    void display()const override{
        cout<<r<<' '<<area();
    }

};
int main(){
    Rectangle r(2,3);
    Circle c(3);
    c.display();
    cout<<endl;
    Shape *b=new Circle(2);
    b->display();
    cout<<endl;
    vector<Shape*>xx{&r,&c};
    for(auto x:xx){
        x->display();
    }
}