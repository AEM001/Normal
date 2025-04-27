//根据题目描述，以及以下提示和要求，直接在本文件中添加代码。将文件名修改为本人学号后提交。
//定义本科生类
#include<iostream>
#include<cmath>
#define PI 3.1415926

using namespace std;

class Shape        
{	
	//按题目要求定义图形抽象类
	public:
		virtual double calGirth()=0;
		virtual double calArea()=0;
		virtual void print()=0;
		virtual void move(double dx,double dy)=0;
		virtual void enlarge(double dl)=0;

}; 

class Rectangle : public Shape
{
	//按题目要求定义矩形类
	private:
		double x,y;
		double dx,dy;
	public:
		Rectangle(double a,double b,double c,double d):x(a),y(b),dx(c),dy(d){}
		double calGirth()override
		{
			return 2*(x+y);
		}
		double calArea()override{
			return x*y;
		}
		void print() override{//打印矩形的左下角坐标、长、宽、周长、面积
			cout<<"矩形左下角坐标为"
			<<"("<<x<<','<<y<<')'<<"长为"<<dx<<' '<<"宽为"<<dy<<' '<<"周长为"<<calGirth()<<' '<<"面积为"<<calArea()<<endl;
		}
		void move(double dx,double dy)override{
			this->x+=dx;
			this->y+=dy;
		}
		void enlarge(double dl)override{
			dx*=dl;
			dy*=dl;
		}

	                  
};

class Circle : public Shape
{
	//按题目要求定义圆类
	private:
	double x,y;
	double r;
	public:
		Circle(double a,double b,double c):x(a),y(b),r(c){}
		double calGirth()override
		{
			return 2*PI*r;
		}
		double calArea()override{
			return PI*r*r;
		}////打印圆心坐标、半径、周长、面积
		void print() override{
			cout<<"圆心坐标为"<<"("<<x<<','<<y<<')'<<"半径为"<<r<<' '<<"周长为"<<calGirth()<<' '<<"面积为"<<calArea()<<endl;

		}
		void move(double dx,double dy)override{
			this->x+=dx;
			this->y+=dy;
		}
		void enlarge(double dl)override{
			r*=dl;
		}


};

void Move_GeoObj(Shape* p, double x, double y)
{
	//按题目要求补充定义代码，实现图形平移
	p->move(x,y);

}

//补充代码：参考Move_GeoObj函数，用基类引用做参数，实现图形缩放
void Move_GeoObj(Shape& p,double dl)
{
	p.enlarge(dl);
}

//补充代码：根据主函数调用形式，定义打印图形的函数
void Print_GeoObj(Shape& p){
	p.print();

}


int main()
{
	Rectangle rec(3, 4, 8, 3);     //左下角坐标（3,4），长8，宽3	
	
	Print_GeoObj(rec);             //打印矩形的左下角坐标、长、宽、周长、面积
	cout << endl;

	Move_GeoObj(&rec, 3, 5);      //平移矩形

	Print_GeoObj(rec);             
	cout << endl;

	Circle cir(1, 2, 4);           //圆心坐标（1,2），半径4//补充代码

	Print_GeoObj(cir);             //打印圆心坐标、半径、周长、面积
	cout << endl;

	//补充代码，调用函数将圆的半径放大为2倍
	cir.enlarge(4);

	Print_GeoObj(cir);             
	cout << endl;

	return 0;
}

