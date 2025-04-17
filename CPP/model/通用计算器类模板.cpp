#include <iostream>
#include <string>
#include<cmath>
#include <stdexcept>

// 假设你的通用计算器类模板命名为 Calculator
template<typename T>
class Calculator {
    // 你需要在这里实现通用计算器的功能
    T a,b;
    public:
    T add(T a,T b)
    {
        return a+b;
    }
    T subtract(T a,T b)
    {
        return a-b;
    }
    T multiply(T a,T b)
    {
        return a*b;
    }
    T divide(T a,T b)
    {
        if(b==0)
        {
            throw std::runtime_error("Division by zero is not allowed.");
        }
        return a/b;
    }
    T power(T a,T b)
    {
        return pow(a,b);
    }
};

// 特化版本（针对 std::string）
template<>
class Calculator<std::string> {
    // 你需要在这里实现字符串特化的功能
    std::string a,b;
    public:
    std::string concatenate(std::string a,std::string b)
    {
        return a+b;
    }
};

int main() {
    try {
        // 测试 int 类型的基本运算
        Calculator<int> intCalc;
        std::cout << "Testing with int:\n";
        std::cout << "Add: " << intCalc.add(5, 3) << "\n";         // 输出 8
        std::cout << "Subtract: " << intCalc.subtract(5, 3) << "\n"; // 输出 2
        std::cout << "Multiply: " << intCalc.multiply(5, 3) << "\n"; // 输出 15
        std::cout << "Divide: " << intCalc.divide(6, 3) << "\n";   // 输出 2
        std::cout << "Power: " << intCalc.power(2, 3) << "\n";     // 输出 8

        // 测试 double 类型的基本运算
        Calculator<double> doubleCalc;
        std::cout << "\nTesting with double:\n";
        std::cout << "Add: " << doubleCalc.add(5.5, 3.2) << "\n";   // 输出 8.7
        std::cout << "Subtract: " << doubleCalc.subtract(5.5, 3.2) << "\n"; // 输出 2.3
        std::cout << "Multiply: " << doubleCalc.multiply(5.5, 3.2) << "\n"; // 输出 17.6
        std::cout << "Divide: " << doubleCalc.divide(6.0, 3.0) << "\n";   // 输出 2.0
        std::cout << "Power: " << doubleCalc.power(2.0, 3) << "\n";       // 输出 8.0

        // 测试 std::string 的特化版本
        Calculator<std::string> stringCalc;
        std::cout << "\nTesting with std::string:\n";
        std::cout << "Concatenate: " << stringCalc.concatenate("Hello, ", "World!") << "\n"; // 输出 "Hello, World!"

        // 测试异常处理
        std::cout << "\nTesting exception handling:\n";
        try {
            std::cout << "Divide by zero: " << intCalc.divide(5, 0) << "\n";
        } catch (const std::exception& e) {
            std::cout << "Exception caught: " << e.what() << "\n"; // 应该捕获异常
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}