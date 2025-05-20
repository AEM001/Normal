#include <iostream>
using namespace std;

// 基类
class Base {
public:
    virtual void display() {  // 提示：这个关键字用于实现多态
        cout << "这是基类的display()方法" << endl;
    }
    
    void show() {
        cout << "这是基类的show()方法" << endl;
    }
};

// 派生类
class Derived : public Base {  // 提示：这个类继承自Base
public:
    void display() override {  // 提示：这个关键字表示重写基类虚函数
        cout << "这是派生类的display()方法" << endl;
    }
    
    void show() {
        cout << "这是派生类的show()方法" << endl;
    }
    
    void derivedOnly() {
        cout << "这是派生类特有的方法" << endl;
    }
};

int main() {
    // 1. 基类指针指向派生类对象
    Base *basePtr = new Derived();  // 提示：创建派生类对象并用基类指针指向它
    basePtr->display();  // 多态调用派生类方法
    basePtr->show();   // 提示：调用基类的非虚函数
    
    // 2. 基类引用绑定派生类对象
    Derived derivedObj;
    Base &baseRef = derivedObj;  // 提示：使用基类引用绑定派生类对象
    baseRef.display();
    baseRef.show();
    
    // 3. 类型转换示例
    if (Derived* derivedPtr = dynamic_cast<Derived*>(basePtr)) {  // 提示：动态类型转换
        derivedPtr->derivedOnly();  // 提示：调用派生类特有方法
    }
    
    delete basePtr;
    return 0;
}
