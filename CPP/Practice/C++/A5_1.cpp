#include "A5.h"

// 随机生成一种颜色
BGR RandBGR()
{
    int b = rand() % 256;
    int g = rand() % 256;
    int r = rand() % 256;
    return BGR(b, g, r);
}

// 随机生成一个指定像素平面范围内的点
Point NewRandPoint(int a, int b)
{
	int mode = abs(a - b) + 1;
	int left = min(a, b);
	int x = rand() % mode + left;
	int y = rand() % mode + left;
	return Point(x, y);
}

int main()
{
	// 步骤一：创建800*800的位图对象作为画板
	Bitmap bmp(800,800);
	srand(static_cast<unsigned int>(time(nullptr)));
	// 步骤二：依次随机生成100个平面上的点，相邻点用线段相连，每次绘制线段的颜色随机
	Point a=NewRandPoint(800,0);
	for(int i=0;i<99;++i)
	{
	 Point b=NewRandPoint(800,0);
	 bmp.DrawLine(a,b,RandBGR());
	 a.x=b.x;
	 a.y=b.y;
	}

	// 步骤三：保存成名为“A5_1.bmp”的位图文件
	bmp.SaveBitmap("A5_1.bmp");

}