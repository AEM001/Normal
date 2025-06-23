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

	// 步骤二：以圆心(400,400)为起点，每延顺时针方向旋转1度，绘制长度为半径的白色直线，可得到圆周上的一个点
	//         旋转360度后，可以得到该圆周上等间隔的360个离散点，用线段把圆周上相邻的点连接起来。

	// 生成绘制圆的随机色
	// 创建圆心

	// 根据上述提示，自行设计具体绘制流程，完成圆形的绘制
	Point center(400,400);
	Point start=bmp.DrawLine(center,0,300,WHITE);
	for(int i=1;i<360;++i)
	{Point next=bmp.DrawLine(center,i,300,WHITE);
	bmp.DrawLine(start,next,RandBGR());
	start.x=next.x;
	start.y=next.y;
}
	bmp.SaveBitmap("A5_2.bmp");


	// 步骤三：保存成名为“A5_2.bmp”的位图文件


}