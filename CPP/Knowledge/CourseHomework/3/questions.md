### 一、函数与作用域
- 区分成员函数和全局函数（友元），全局函数就不用添加作用域；而在头文件中，只有成员函数才需要添加作用域
- 为什么友元可以直接访问类内部的东西
- 静态成员如何定义初始化、使用
- 静态成员避免多次定义
  - 头文件里面仅仅进行声明，定义在cpp文件里面

### 二、运算符重载

#### 基本概念
- **运算符重载调用机制**：编译器遇到运算符时，优先检查**左操作数的类**是否有**成员函数重载**，若无则查找**全局重载函数**，最终选择参数最匹配的版本
- **成员函数vs全局函数**：
  - 必须成员函数重载：`=`, `[]`, `()`, `->`, `new`, `delete`等
  - 必须全局函数重载：`<<`, `>>`等（因无法修改流对象）

#### 前置++和后置++
- **前置++**：`T& operator++()` - 先增加再返回，返回引用
- **后置++**：`T operator++(int)` - 先返回副本再增加，返回值

#### 引用(&)使用规则
- **必须使用引用的情况**：
  1. 赋值类运算符：`=`, `+=`, `-=`等（支持链式调用）
     - 例：`myString& operator=(const myString&)`
  2. 流运算符：`<<`, `>>`（避免复制流对象）
     - 例：`ostream& operator<<(ostream&, const myString&)`
  3. 修改自身的运算符：前置`++`, `--`
     - 例：`myString& operator++()`
  4. 大对象参数传递（避免拷贝开销）
     - 例：`bool operator==(const myString&)`

- **不应使用引用的情况**：
  1. 算术运算符：`+`, `-`, `*`, `/`（返回新对象）
     - 例：`myString operator+(const myString&)`
  2. 关系运算符：`==`, `!=`, `<`, `>`（返回基本类型）
     - 例：`bool operator==(const myString&)`
  3. 后置自增/自减：`++`, `--`（返回修改前副本）
     - 例：`myString operator++(int)`

#### 特殊注意事项
- **const修饰**：不修改参数时用`const &`
- **移动语义**：右值引用用`&&`
- **链式调用**：修改自身的操作应返回引用
- **对象生命周期**：返回引用时必须确保对象有效
- **const成员函数**：不能修改成员变量
  ```cpp
  // const版本的下标访问
  const char& operator[](size_t pos) const {  // const成员函数
      return data[pos];  // 返回const引用防止修改
  }

### 三、模板与友元
- 模板与友元，使用细节与注意事项
- 在使用模板时，不能再次写int size，只需要在模板最开始的声明处说清楚就行
```cpp
template<typename T, int size>
class SeqList {
    friend std::ostream& operator<<(std::ostream&, const SeqList&);
};

特化版本：适用于需要为不同模板参数提供不同实现的场景
非特化版本：适用于所有实例化类型都使用相同实现的通用情况
四、构造与拷贝
fraction::fraction(int a=0,int b=1):fz(a),fm(b){};在声明中，似乎需要移除默认参数，否则会报错，为什么？
拷贝构造函数需要考虑空指针和自赋值的情况
拷贝构造函数的参数是const引用，为什么？
因为拷贝构造函数是用来初始化一个新对象的，而不是用来赋值的，所以需要const引用，避免修改原对象
五、语法细节
cout的问题究竟是什么：注意细节：include<iostream>
头文件保护
#ifndef FRACTION_H
#define FRACTION_H
#endif
其他地方只要不需要修改，尽量都是用const保护
bool isempty() const { return size == 0; } // 注意这里的const
一定注意：预算符重载如果不是友元函数，则是一个类的成员函数，此时this指针指向的是左操作数，而不是右操作数，所以需要将左操作数作为参数传递给函数，而右操作数作为函数的参数；同时注意在模板中，返回的是什么尤其是返回T的时候
六、友元示例
友元 os<<a.list[i]<<' '; // 通过对象a访问私有成员list

七、调试技巧
如何阅读报错信息
• 报错格式：

• 文件名:行号:列号: error: 描述

• 行号 | 代码

• note: 候选方案
```
// 1. 基本函数模板定义
template <typename T>  // 或者 template <class T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

// 2. 多参数模板
template <typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}

// 3. 显式实例化
template double max<double>(double, double);  // 显式实例化特定类型

// 4. 显式特化
template <>
const char* max<const char*>(const char* a, const char* b) {
    return (strcmp(a, b) > 0) ? a : b;
}

// 5. 非类型模板参数
template <typename T, int Size>
void printArray(T (&arr)[Size]) {
    // 使用Size作为非类型参数
}

// 6. 可变参数模板
template <typename... Args>
void print(Args... args) {
    // 处理可变数量参数
}

// 7. 默认模板参数
template <typename T = int>
T getValue() {
    return T();
}

// 1. 基本类模板定义
template <typename T>
class Stack {
private:
    T* elements;
    int size;
    // ... 其他成员
};

// 2. 类模板成员函数定义
template <typename T>
void Stack<T>::push(const T& elem) {
    // 实现
}

// 3. 类模板特化
template <>
class Stack<bool> {
    // 针对bool类型的特殊实现
};

// 4. 类模板部分特化
template <typename T>
class Container<T*> {
    // 针对指针类型的特殊实现
};

// 5. 类模板中的静态成员
template <typename T>
class Example {
    static T staticVar;  // 声明
};
// 定义静态成员（在类外）
template <typename T>
T Example<T>::staticVar = T();

// 6. 类模板作为模板参数
template <typename T, template <typename> class Container>
class Adapter {
    Container<T> container;
};

// 7. 类模板友元
template <typename T>
class MyClass {
    // 友元函数
    template <typename U>
    friend void friendFunc(MyClass<U>&);
    
    // 友元类
    template <typename U>
    friend class FriendClass;
};

// 8. 类模板别名（C++11）
template <typename T>
using Vec = std::vector<T>;
易错点
// 1. 模板定义通常放在头文件中
// 错误：将模板定义分离到.cpp文件中
// 正确：整个模板定义应在头文件中

// 2. 模板参数推导失败
template <typename T>
T max(T a, T b);
// 错误：max(5, 5.5) - 参数类型不同，无法推导T
// 正确：max<double>(5, 5.5) 或使用auto返回类型

// 3. 依赖类型前缺少typename关键字
template <typename T>
void func() {
    // 错误：T::iterator it;
    // 正确：typename T::iterator it;
}

// 4. 模板特化必须在使用前声明
// 错误：先使用特化版本，后声明
// 正确：先声明特化，再使用

// 5. 类模板中的this指针
template <typename T>
class Example {
    void func() {
        // 错误：T::method()
        // 正确：this->method() 或 Example<T>::method()
    }
};