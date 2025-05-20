#include"fraction.h"
fraction::fraction(int a,int b):fz(a),fm(b){};
void reduce(int& fz,int &fm)
{
	if(fz==0){
		fm=1;
	}
	else{
	int a=abs(fz>fm?fz:fm);
	int b=abs(fz<fm?fz:fm);
	int r=a%b;
	while(r!=0)
	{a=b;
	b=r;
	r=a%b;}
	fz=fz*fm/(abs(fz)*abs(fm)*b);
	fm=abs(fm)/b;
	}
}
istream& operator>>(istream& is,fraction & frac)
{
	is>>frac.fz>>frac.fm;
	if(frac.fm==0)
	{std::cout<<"can not be  divided by zero";}
	frac.fm=1;
	return is;

}
ostream& operator<<(ostream& os,fraction &frac)
{
	if(frac.fz==0)
	{
		os<<0;
		return os;
	}
	else if(frac.fm=1)
	{
		os<<frac.fz;
		return os;
	}
	else {os<<frac.fz<<'/'<<frac.fm;
	return os;
	}
}
void fraction::set(int a,int b)
{	fz=a;
	fm=b;
	reduce(fz,fm);
}
fraction operator+(fraction frac,int k)
{
	fraction ftemp;
	ftemp.fz=frac.fm*k+frac.fz;
	ftemp.fm=frac.fm;
	reduce(ftemp.fz,ftemp.fm);
	return ftemp;
}
fraction operator+(fraction f1,fraction f2)
{
	fraction ftemp;
	ftemp.fz=f1.fm*f2.fz+f1.fz*f2.fm;
	ftemp.fm=f1.fm*f2.fm;
	reduce(ftemp.fz,ftemp.fm);
	return ftemp;
	
}
fraction operator/(fraction f1,fraction f2)
{
	if(f2.fz==0)
	{
		std::cout<<"can not devided by zero, default make the num 1";
		return f1;
	}
	else{
	fraction ftemp;
	ftemp.fz=f1.fz*f2.fm;
	ftemp.fm=f1.fm*f2.fz;
	reduce(ftemp.fz,ftemp.fm);
	return ftemp;	
	}
}
fraction operator++(fraction& frac)
{
	frac.fz+=1;
	return frac;
}
fraction operator++(fraction& frac,int)
{
	frac.fz+=1;
	return frac;
}
// 删除以下错误实现
// double fraction::operator=(fraction& frac) {
//     return static_cast<double>(frac.fz*1.0/frac.fm);
// }

fraction::operator double() const {
    return static_cast<double>(fz) / fm; 
}