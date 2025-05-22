#include <iostream>
#include <stdexcept>
#include<cstring>
using namespace std;

class Array {
private:
    int* data;  // 使用普通指针管理内存
    size_t array_size;

public:
    // 构造函数
    Array(size_t size, int value = 0):data(new int[size])
    {for(int i=0;i<size;++i)
    {data[i]=value;}
cout<<"创建一个值全为"<<value<<"的新对象"<<endl;}  // 你需要补充

    // 默认构造函数
    Array():data(nullptr),array_size(0){
    cout<<"创建一个空的新对象"<<endl;

    }  // 你需要补充

    // 拷贝构造函数
    Array(const Array& other):data(new int[other.array_size])
    {
        array_size=other.array_size;
        memcpy(data,other.data,other.array_size * sizeof(int));
    }  // 你需要补充

    // 赋值运算符
    Array& operator=(const Array& other)
    {
        if(this!=&other)
        {   cout<<"进行了拷贝赋值"<<endl;
            delete[]data;
            data=new int[other.array_size];
            array_size=other.array_size;
            for(int i=0;i<array_size;++i)
            {data[i]=other.data[i];}
            
        }
        return *this;
    }  // 你需要补充

    // 析构函数
    ~Array(){delete[] data;
    cout<<"析构函数"<<endl;}  // 你需要补充

    // 获取数组大小
    size_t size() const {
        return array_size;
    }

    // 下标运算符
    int& operator[](size_t index) {
        if (index >= array_size) throw std::out_of_range("Index out of range");
        return data[index];
    }

    // 下标运算符（常量版本）
    const int& operator[](size_t index) const {
        if (index >= array_size) throw std::out_of_range("Index out of range");
        return data[index];
    }

    // 打印数组内容
    void print() const {
        for (size_t i = 0; i < array_size; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    try {
        // 创建并测试 Array 类的实例，补充测试代码
        Array a;
        Array b(5,1);
        a=b;

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
