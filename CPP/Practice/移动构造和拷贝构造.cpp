#include <iostream>
#include <cstring> // for memcpy
using namespace std;

class Buffer {
private:
    int* data;
    size_t size;

public:
    // 构造函数
    Buffer(size_t n) : data(new int[n]), size(n) {
        std::cout << "普通构造函数" << std::endl;
        for(size_t i=0; i<n; ++i) data[i] = i;
    }

    // 拷贝构造函数
    Buffer(const Buffer& other) :data(new int[other.size]),size(other.size){
        memcpy(data,other.data,size);
        // 使用std::copy怎么写
        cout<<"拷贝构造"<<endl;
    }


    // TODO：在此处补全初始化列表及函数体
    //什么是初始化列表，刚刚似乎没有进行赋值，具体是什么情况

    // 移动构造函数
    Buffer(Buffer&& other) noexcept:data(other.data),size(other.size){
        other.data=nullptr;
        other.size=0;
        cout<<"移动构造"<<endl;
    }
    // TODO：在此处补全初始化列表及函数体
    

    // 拷贝赋值运算符
    Buffer& operator=(const Buffer& other)
    {for(int i=0;i<size;++i)
        {
            data[i]=other.data[i];
        }

    }
    // TODO：补全函数体

    // 移动赋值运算符
    Buffer& operator=(Buffer&& other) noexcept{
        for(int i=0;i<size;++i)
        {
            data[i]=other.data[i];
        }
        delete &other;
    }
    // TODO：补全函数体

    // 析构函数
    ~Buffer(){delete data;}
    // TODO：补全函数体

    void print() const {
        for(size_t i=0; i<size; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};

// 返回一个临时对象，触发移动构造
Buffer makeBuffer(size_t n) {
    Buffer tmp(n);
    return tmp;
}

int main() {
    std::cout << "--- 普通构造 ---" << std::endl;
    Buffer buf1(5);
    buf1.print();

    std::cout << "--- 拷贝构造 ---" << std::endl;
    Buffer buf2 = buf1; // 拷贝构造
    buf2.print();

    std::cout << "--- 移动构造 ---" << std::endl;
    Buffer buf3 = makeBuffer(3); // 移动构造
    buf3.print();

    std::cout << "--- 移动赋值 ---" << std::endl;
    buf3 = makeBuffer(4); // 移动赋值
    buf3.print();

    std::cout << "--- 拷贝赋值 ---" << std::endl;
    buf2 = buf1; // 拷贝赋值
    buf2.print();

    return 0;
}
