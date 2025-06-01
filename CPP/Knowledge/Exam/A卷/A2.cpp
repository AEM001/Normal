#include <iostream>
#include <cmath>
using namespace std;

class MyRGB {
private:
    int r, g, b;

public:
    // 完善代码1: 补充必要构造函数
    MyRGB(int red, int green, int blue) : r(red), g(green), b(blue) {}
    
    // 完善代码2: 设计成员函数grayInt，计算灰度值
    int grayInt() const {
        return static_cast<int>(0.299 * r + 0.587 * g + 0.114 * b + 0.5);
    }

    // 完善代码3: 根据测试需求，补充必要的代码设计
    bool operator==(const MyRGB& other) const {
        return grayInt() == other.grayInt();
    }

    void print() const {
        cout << "(" << r << ", " << g << ", " << b << ")";
    }
};

//添加代码4: 定义函数模板countDuplicates，统计序列中去重后的不同数据个数
template <typename T>
int countDuplicates(const T* arr, int size) {
    int count = 0;
    for (int i = 0; i < size; ++i) {
        bool isDuplicate = false;
        for (int j = 0; j < i; ++j) {
            if (arr[i] == arr[j]) {
                isDuplicate = true;
                break;
            }
        }
        if (!isDuplicate) {
            ++count;
        }
    }
    return count;
}

int main() {
    // 定义一组MyRGB类型的颜色colors
    MyRGB colors[9] = {
        MyRGB(140, 60, 60),   MyRGB(153, 56, 51),  MyRGB(76, 150, 29),    
        MyRGB(255, 255, 255), MyRGB(0, 0, 0),      MyRGB(76, 150, 29), 
        MyRGB(200, 50, 50),   MyRGB(255, 0, 0),    MyRGB(76, 150, 29) 
    };

    // 输出colors中各颜色的r/g/b值及其对应的灰度值
    for (int i = 0; i < 9; ++i) {
        colors[i].print();
        std::cout << " -- Grayscale:" << colors[i].grayInt() << std::endl;
    }

    // 添加代码5：调用函数模板，获得colors中不重复灰度值的颜色个数count
    int count = countDuplicates(colors, 9);
   
    if (count > 1)
        std::cout << "\nThere are " << count << " different grayscale colors.\n";
    else
        std::cout << "\nAll points have the same grayscale.\n";

    return 0;
}

/*简答题：函数模板和函数重载是 C++ 中常用的两种机制，
          从函数原型设计和应用场景角度，两者有何不同？(5分)

函数模板和函数重载的主要区别：
1. 原型设计：
   - 函数模板：使用template关键字定义，可以处理多种数据类型
   - 函数重载：定义多个同名函数，参数列表不同

2. 应用场景：
   - 函数模板：适用于对不同数据类型执行相同操作的场景
   - 函数重载：适用于对不同参数类型或数量执行不同操作的场景

3. 代码生成：
   - 函数模板：编译器根据需要实例化具体版本
   - 函数重载：需要显式定义每个版本

4. 灵活性：
   - 函数模板更灵活，可以自动适应新类型
   - 函数重载需要手动添加新版本
*/