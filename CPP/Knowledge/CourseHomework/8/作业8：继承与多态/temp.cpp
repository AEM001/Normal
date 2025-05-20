#include <vector>
#include<iostream>
using namespace std;

class Shape {
public:
    virtual void draw()= 0;
    virtual ~Shape() {}  // 添加虚析构函数
};

class Circle : public Shape {
public:
    void draw() override {
        cout << "Drawing a circle" << endl;
    }
};

class Square: public Shape {
public:
    void draw() override {
        cout << "Drawing a square" << endl;  // 修正输出内容
    }
};

int main() {
    vector<Shape*> shapes;
    shapes.push_back(new Circle());
    shapes.push_back(new Square());
    
    // 使用引用避免拷贝
    for(auto& x : shapes) {
        x->draw();
    }
    
    // 释放内存
    for(auto x : shapes) {
        delete x;
    }
    // 不需要 delete shapes; vector 会自动释放
}