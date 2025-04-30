#include <iostream>
#include <string>
using namespace std;
// 1. 定义一个 Student 类
class Student {
private:    // 私有成员：外部无法直接访问
    std::string name;
    int age;

public:     // 公有成员：外部可访问
    // 2. 构造函数：初始化对象
    Student(std::string n, int a) : name(n), age(a) {}
    // 析构函数
    ~Student() {
        std::cout << name << " is destroyed." << std::endl;
    }
    // 3. 成员函数：定义行为
    void study() {
        std::cout << name << " is studying." << std::endl;
    }

    void setAge(int newAge) {
        if (newAge > 0) {
            age = newAge;  // 通过公有函数修改私有成员
        }
    }
    void printInfo() {
        cout<<name<<' '<<age;
    }
    // 4. 提供访问私有数据的接口
    std::string getName() const { 
        return name; 
    }
};

int main() {
    // 5. 创建对象（实例化）
    Student stu1("Alice", 20);  // 栈对象
    Student* stu2 = new Student("Bob", 22);  // 堆对象（需手动释放）

    stu1.study();                // 输出：Alice is studying.
    stu2->study();            // 输出：Bob is studying.

    // stu1.age = 21;           // 错误！age 是私有成员
    stu1.setAge(21);            // 通过公有函数修改

    std::cout << stu1.getName() << std::endl;  // 输出：Alice
    stu1.printInfo();
    delete stu2;  // 释放堆对象
    return 0;
}
/*由于delete，BOB的声明周期先结束，然后main函数结束，Alice调用析构函数*/