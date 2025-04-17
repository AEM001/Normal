#include <iostream>
using namespace std;

// 补充函数模板定义
template <typename T>
T maxValue(T a, T b) {
    // 在这里实现返回较大值的逻辑
    if(a>b){return a;}
    else{return b;}
}

int main() {
    cout << maxValue(10, 20) << endl;       // 输出：20
    cout << maxValue(3.5, 2.8) << endl;    // 输出：3.5
    cout << maxValue('a', 'z') << endl;    // 输出：z
    return 0;
}