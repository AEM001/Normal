#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// 基类：图形
class Shape {
protected:
    string name;
    
public:
    // 构造函数，接受图形名称参数
    Shape(string n) : name(n) {}
    
    // 虚析构函数
    virtual ~Shape() {
        cout << "Shape 析构函数被调用" << endl;
    }
    
    // 纯虚函数 calculateArea()，返回double类型
    virtual double calculateArea() = 0;
    
    // 纯虚函数 calculatePerimeter()，返回double类型
    virtual double calculatePerimeter() = 0;
    
    // 虚函数 displayInfo()，显示图形基本信息
    virtual void displayInfo() {
        cout << "图形名称: " << name << endl;
    }
    
    // 获取图形名称
    string getName() const { return name; }
};

// 派生类：矩形
class Rectangle : public Shape {
private:
    double width, height;
    
public:
    // 构造函数，接受宽度和高度参数
    Rectangle(double w, double h) : Shape("矩形"), width(w), height(h) {}
    
    // 重写 calculateArea() 函数
    double calculateArea() override {
        return width * height;
    }
    
    // 重写 calculatePerimeter() 函数
    double calculatePerimeter() override {
        return 2 * (width + height);
    }
    
    // 重写 displayInfo() 函数，显示矩形特有信息
    void displayInfo() override {
        Shape::displayInfo();
        cout << "宽度: " << width << endl;
        cout << "高度: " << height << endl;
        cout << "面积: " << calculateArea() << endl;
        cout << "周长: " << calculatePerimeter() << endl;
    }
};

// 派生类：圆形
class Circle : public Shape {
private:
    double radius;
    const double PI = 3.14159;
    
public:
    // 构造函数，接受半径参数
    Circle(double r) : Shape("圆形"), radius(r) {}
    
    // 重写 calculateArea() 函数
    double calculateArea() override {
        return PI * radius * radius;
    }
    
    // 重写 calculatePerimeter() 函数
    double calculatePerimeter() override {
        return 2 * PI * radius;
    }
    
    // 重写 displayInfo() 函数，显示圆形特有信息
    void displayInfo() override {
        Shape::displayInfo();
        cout << "半径: " << radius << endl;
        cout << "面积: " << calculateArea() << endl;
        cout << "周长: " << calculatePerimeter() << endl;
    }
};

// 派生类：三角形
class Triangle : public Shape {
private:
    double side1, side2, side3;
    
public:
    // 构造函数，接受三边长参数
    Triangle(double a, double b, double c) : Shape("三角形"), side1(a), side2(b), side3(c) {}
    
    // 重写 calculateArea() 函数 (使用海伦公式)
    double calculateArea() override {
        double s = (side1 + side2 + side3) / 2;
        return sqrt(s * (s - side1) * (s - side2) * (s - side3));
    }
    
    // 重写 calculatePerimeter() 函数
    double calculatePerimeter() override {
        return side1 + side2 + side3;
    }
    
    // 重写 displayInfo() 函数，显示三角形特有信息
    void displayInfo() override {
        Shape::displayInfo();
        cout << "边长1: " << side1 << endl;
        cout << "边长2: " << side2 << endl;
        cout << "边长3: " << side3 << endl;
        cout << "面积: " << calculateArea() << endl;
        cout << "周长: " << calculatePerimeter() << endl;
    }
};

// 图形管理器类
class ShapeManager {
private:
    vector<Shape*> shapes;  // 使用原始指针代替智能指针
    
public:
    // TODO 18: 补充 addShape 函数，接受 Shape* 类型参数
    void addShape(Shape* shape) {
        // 实现代码
        shapes.push_back(shape);
    }
    
    // TODO 19: 补充 displayAllShapes 函数，遍历并显示所有图形信息
    void displayAllShapes() {
        // 实现代码
        for(auto x:shapes)
        {x->displayInfo();}
    }
    
    // TODO 20: 补充 calculateTotalArea 函数，计算所有图形的总面积
    double calculateTotalArea() {
        // 实现代码
        double sum=0;
        for(auto x:shapes){
            sum+=x->calculateArea();
        }
        return sum;
    }
    
    // TODO 21: 补充 findLargestShape 函数，返回面积最大的图形指针
    Shape* findLargestShape() {
        // 实现代码
        Shape* temp=shapes[0];
        for(auto x:shapes){
            if((x->calculateArea())>(temp->calculateArea()))
            {temp=x;}
        }
        return temp;
    }
    
    // TODO 22: 补充析构函数，释放所有动态分配的图形对象
    ~ShapeManager() {
        // 实现代码
         for(auto x:shapes){
           delete[] x;
        }
    }
};

int main() {
    ShapeManager manager;
    
    // TODO 22: 创建不同类型的图形对象并添加到管理器中
    // 创建一个矩形 (宽5, 高3)
    Rectangle re(5,3); 
    manager.addShape(&re);   
    
    // 创建一个圆形 (半径4)
    Circle ci(4);
    manager.addShape(&ci);
    
    // 创建一个三角形 (边长3, 4, 5)
    Triangle tr(3,4,5);
    manager.addShape(&tr);
    // TODO 23: 调用管理器的各种功能
    cout << "=== 所有图形信息 ===" << endl;
    manager.displayAllShapes();
    
    cout << "\n=== 总面积 ===" << endl;
    cout<<manager.calculateTotalArea();
    
    cout << "\n=== 面积最大的图形 ===" << endl;
    Shape*x=manager.findLargestShape();
    cout<<x->getName();
    cout<<x->calculateArea();
    
    return 0;
}