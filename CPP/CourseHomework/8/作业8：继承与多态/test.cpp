//������Ŀ�������Լ�������ʾ��Ҫ��ֱ���ڱ��ļ�����Ӵ��롣���ļ����޸�Ϊ����ѧ�ź��ύ��
//���屾������
#include<iostream>
#include<cmath>
#define PI 3.1415926

using namespace std;

class Shape        
{	
	//����ĿҪ����ͼ�γ�����
	public:
		virtual double calGirth()=0;
		virtual double calArea()=0;
		virtual void print()=0;
		virtual void move(double dx,double dy)=0;
		virtual void enlarge(double dl)=0;

}; 

class Rectangle : public Shape
{
	//����ĿҪ���������
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
		void print() override{//��ӡ���ε����½����ꡢ�������ܳ������
			cout<<"�������½�����Ϊ"
			<<"("<<x<<','<<y<<')'<<"��Ϊ"<<dx<<' '<<"��Ϊ"<<dy<<' '<<"�ܳ�Ϊ"<<calGirth()<<' '<<"���Ϊ"<<calArea()<<endl;
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
	//����ĿҪ����Բ��
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
		}////��ӡԲ�����ꡢ�뾶���ܳ������
		void print() override{
			cout<<"Բ������Ϊ"<<"("<<x<<','<<y<<')'<<"�뾶Ϊ"<<r<<' '<<"�ܳ�Ϊ"<<calGirth()<<' '<<"���Ϊ"<<calArea()<<endl;

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
	//����ĿҪ�󲹳䶨����룬ʵ��ͼ��ƽ��
	p->move(x,y);

}

//������룺�ο�Move_GeoObj�������û���������������ʵ��ͼ������
void Move_GeoObj(Shape& p,double dl)
{
	p.enlarge(dl);
}

//������룺����������������ʽ�������ӡͼ�εĺ���
void Print_GeoObj(Shape& p){
	p.print();

}


int main()
{
	Rectangle rec(3, 4, 8, 3);     //���½����꣨3,4������8����3	
	
	Print_GeoObj(rec);             //��ӡ���ε����½����ꡢ�������ܳ������
	cout << endl;

	Move_GeoObj(&rec, 3, 5);      //ƽ�ƾ���

	Print_GeoObj(rec);             
	cout << endl;

	Circle cir(1, 2, 4);           //Բ�����꣨1,2�����뾶4//�������

	Print_GeoObj(cir);             //��ӡԲ�����ꡢ�뾶���ܳ������
	cout << endl;

	//������룬���ú�����Բ�İ뾶�Ŵ�Ϊ2��
	cir.enlarge(4);

	Print_GeoObj(cir);             
	cout << endl;

	return 0;
}

