#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdint>  // 添加这个头文件

#define PI 3.14159265358979323846

// 定义几种常用颜色的宏
#define RED BGR(0,0,255)
#define GREEN BGR(0,255,0)
#define YELLOW BGR(0, 255, 255)
#define BLUE BGR(255,0,0)
#define GRAY BGR(127,127,127)
#define BLACK BGR(0,0,0)
#define WHITE BGR(255,255,255)
#define ORANGE BGR(0, 140, 255)

using namespace std;

class BGR {
public:
    uint8_t B;  // 使用 uint8_t 替代 byte
    uint8_t G;
    uint8_t R;

    BGR(uint8_t b=0, uint8_t g=0, uint8_t r=0) :B(b), G(g), R(r) {}
};

class Point {
public:
    double x;
    double y;

    Point(double x0 = 0, double y0 = 0) :x(x0), y(y0) {}
};

#pragma pack(1)
struct BitmapHead		// 位图文件头格式
{
    char h1 = 'B';
    char h2 = 'M';
    int fileSize = 54;
    int unknown = 0;
    int dataStart = 54;

    int headSize = 40;
    int width = 0;
    int height = 0;
    short colorPlane = 1;
    short bitsPerPixcel = 24;
    int pressureType = 0;
    int imageDataSize = 0;
    int xPixcelPerMeter = 0;
    int yPixcelPerMeter = 0;
    int colorCount = 0;
    int importentColorCount = 0;
};

class Bitmap {
    BitmapHead head;
    BGR* data;

public:
    Bitmap(int newWidth, int newHeight)
    {
        this->head.width = newWidth;
        this->head.height = newHeight;

        int lineSize = newWidth * 3;
        while (lineSize % 4)
            lineSize++;

        this->head.imageDataSize = lineSize * newHeight;
        this->head.fileSize = this->head.dataStart + this->head.imageDataSize;

        int dataCount = newWidth * newHeight;
        this->data = new BGR[dataCount];
        for (int i = 0; i < dataCount; ++i)
            data[i] = WHITE;
         
    }

    Point DrawLine(Point p, double d, double l, BGR color)
    {
        double a = d * PI / 180;
        double x2 = p.x + l * cos(a);
        double y2 = p.y + l * sin(a);

        Point p2 = { x2, y2 };

        DrawLine(p, p2, color);
        return p2;
    }

    void DrawLine(Point p1, Point p2, BGR color)
    {
        double x1 = p1.x;
        double x2 = p2.x;
        double y1 = p1.y;
        double y2 = p2.y;

        if (fabs(x1 - x2) > fabs(y1 - y2))
        {
            //x基准
            for (int x = (int)(x1 + 0.5);
                x1 < x2 ? x <= x2 + 0.5 : x >= x2 + 0.5;
                x1 < x2 ? ++x : --x)
            {
                int y = (int)((x - x1) * (y2 - y1) / (x2 - x1) + y1);
                SetPoint(x, y, color);
            }
        }
        else
        {
            //y基准
            for (int y = (int)(y1 + 0.5);
                y1 < y2 ? y <= y2 + 0.5 : y >= y2 + 0.5;
                y1 < y2 ? ++y : --y)
            {
                int x = (int)((y - y1) * (x2 - x1) / (y2 - y1) + x1);
                SetPoint(x, y, color);
            }
        }
    }

    void SetPoint(int x, int y, BGR color)
    {
        if (x < 0) return;
        if (y < 0) return;
        if (x >= head.width) return;
        if (y >= head.height) return;

        data[y * this->head.width + x] = color;
    }

    ~Bitmap()
    {
        delete[] this->data;
    }

    void SaveBitmap(const char* filename)
    {
        ofstream fout(filename, ios::binary);
        
        // 写入文件头
        fout.write((char*)&(this->head), sizeof(BitmapHead));

        // 计算每行需要的填充字节数
        int lineBytes = this->head.width * 3;
        int padding = (4 - (lineBytes % 4)) % 4;
        char paddingBytes[3] = {0, 0, 0};

        // BMP文件从底部开始存储，所以从最后一行开始写入
        for (int y = this->head.height - 1; y >= 0; --y)
        {
            for (int x = 0; x < this->head.width; ++x)
            {
                BGR pixel = this->data[y * this->head.width + x];
                // 写入BGR顺序
                fout.write((char*)&pixel.B, 1);
                fout.write((char*)&pixel.G, 1);
                fout.write((char*)&pixel.R, 1);
            }
            // 写入行填充字节
            if (padding > 0)
                fout.write(paddingBytes, padding);
        }
        fout.close();
    }	
};