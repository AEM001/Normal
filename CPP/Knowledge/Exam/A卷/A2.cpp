#include <iostream>
#include <cmath>
using namespace std;

class MyRGB {
private:
    int r, g, b;

public:
    // 完善代码1: 补充必要构造函数
    

    // 完善代码2: 设计成员函数grayInt，计算灰度值


    // 完善代码3: 根据测试需求，补充必要的代码设计
   

    void print() const {
        cout << "(" << r << ", " << g << ", " << b << ")";
    }
};

//添加代码4: 定义函数模板countDuplicates， 统计序列中去重后的不同数据个数
//           算法提示：遍历数组元素，对于每个元素，与之前的元素进行比较，如果找不到相同值的元素，这个数就是不重复的数。
//                     （可以自己设计其它算法，实现编程要求）


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
    
   
    if (count > 1)
        std::cout << "\nThere are " << count << " different grayscale colors.\n";
    else
        std::cout << "\nAll points have the same grayscale.\n";

    return 0;

}

/*简答题：函数模板和函数重载是 C++ 中常用的两种机制，
          从函数原型设计和应用场景角度，两者有何不同？(5分)


*/