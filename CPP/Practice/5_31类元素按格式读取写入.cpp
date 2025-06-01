#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class inventory {
    std::string Name, IdNo;  // 货物名称与货号
    int Quantity;            // 数量
    double Cost, TotalValue; // 单价与总价
public:
        inventory(std::string name, std::string id, int qty, double cost) 
        : Name(name), IdNo(id), Quantity(qty), Cost(cost), TotalValue(qty * cost) {}
    // 默认构造函数
    inventory() : Name("#"), IdNo("0"), Quantity(0), Cost(0), TotalValue(0) {}
    // 声明输出运算符重载函数
    friend ostream& operator <<(ostream& os,const inventory& inv)
    {os<<inv.Name<<"    "<<inv.IdNo<<"  "<<inv.Quantity<<"  "<<inv.Cost<<"  "<<inv.TotalValue<<endl;
    return os;}
    // 声明输入运算符重载函数
    friend istream& operator >>(istream& is,inventory& inv)
    {
        is>>inv.Name>>inv.IdNo>>inv.Quantity>>inv.Cost>>inv.TotalValue;
        return is;
    }
};

int main() {
    inventory car1("夏利2000", "805637928", 156, 80000);
    inventory motor1("金城125", "93612575", 302, 10000);
    inventory car2, motor2;

    std::ofstream destfile("f1.txt");
    // 补全将 car1 和 motor1 对象数据写入文件的代码
    destfile<<car1<<motor1;

    destfile.close();
    cout<<car1<<endl;
    cout<<motor1<<endl;
    std::ifstream sourfile("f1.txt");
    // 补全从文件读取数据到 car2 和 motor2 对象的代码
    sourfile>>car2>>motor2;

    sourfile.close();
    cout<<car2<<endl;
    cout<<motor2<<endl;
    return 0;
}