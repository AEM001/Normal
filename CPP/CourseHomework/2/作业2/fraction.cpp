#include"fraction.h"
fraction::fraction(int a=0,int b=0):fz(a),fm(b){};
void fraction ::reduce()
{
	int a=(fz>fm?fz:fm);
	int b=(fz<fm?fz:fm);
}