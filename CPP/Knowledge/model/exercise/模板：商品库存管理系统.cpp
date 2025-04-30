#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 请在此处定义模板类 Inventory
template <typename T>
class Inventory {
private:
    vector<T> items;
public:
    // 请在此处补充代码：实现 addItem 方法
    void addItem(T item) {
        // 请在此处补充代码
        items.push_back(item);

    }

    // 请在此处补充代码：实现 removeItem 方法
    void removeItem(T item) {
        // 请在此处补充代码
        for(int i=0;i<items.size();++i)
        {
            if(items[i]==item)
            {
                items.erase(items.begin()+i);
                break;
            }
        }

    }

    // 请在此处补充代码：实现 displayInventory 方法
    void displayInventory() {
        // 请在此处补充代码
        for(int i=0;i<items.size();++i)
        {
            cout<<items[i]<<" ";
        }
        cout<<endl;
        
    }
};

int main() {
    // 测试 Inventory<int>
    Inventory<int> intInventory;
    intInventory.addItem(101);
    intInventory.addItem(102);
    intInventory.addItem(103);
    intInventory.removeItem(102);
    intInventory.displayInventory();

    // 测试 Inventory<string>
    Inventory<string> stringInventory;
    stringInventory.addItem("Laptop");
    stringInventory.addItem("Mouse");
    stringInventory.addItem("Keyboard");
    stringInventory.removeItem("Mouse");
    stringInventory.displayInventory();

    return 0;
}