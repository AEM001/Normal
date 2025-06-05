#include <iostream>
using namespace std;

// 链表节点结构
struct Node {
    int data;
    Node* next;
    Node(int d=0,Node*n=nullptr):data(d),next(n){}
    
    // 构造函数：初始化节点数据和指针
};

// 栈类
class Stack {
private:
    Node* top;  // 栈顶指针
    
public:
    // 构造函数：初始化空栈
    Stack() {
        // TODO: 初始化栈顶指针
        Node* top=nullptr;
    }

    
    // 析构函数：释放所有节点内存
    ~Stack() {
        while(top->next!=nullptr)
        {Node* newnode=top->next;
        delete[]top;
        top=newnode;
    }
        delete[]top;
        // TODO: 遍历链表，释放每个节点的内存
    }
    
    // 入栈操作
    void push(int value) {
        // TODO: 创建新节点，设置数据
        // TODO: 将新节点插入到栈顶
        // TODO: 更新栈顶指针
        Node newnode(value) ;
        Node* node=&newnode;
        if(top==nullptr){top=node;}
        else{top->next=node;
        top=node;}
    }
    
    // 出栈操作
    void pop() {
        // TODO: 检查栈是否为空
        // TODO: 保存要删除的节点
        // TODO: 更新栈顶指针
        // TODO: 释放节点内存
        if(isEmpty()){return ;}
        else{}

    }
    
    // 获取栈顶元素
    int peek() {
        // TODO: 检查栈是否为空
        // TODO: 返回栈顶元素的数据
    }
    
    // 检查栈是否为空
    bool isEmpty() {
        // TODO: 判断栈顶指针是否为空
        return top==nullptr;
    }
    
    // 显示栈中所有元素
    void display() {
        // TODO: 从栈顶开始遍历链表
        // TODO: 打印每个节点的数据
        Node* current=top;
        while(current!=nullptr)
        {
            cout<<current->data<<' ';
        }
    }
};

// 测试函数
int main() {
    Stack s;
    
    // TODO: 测试入栈、出栈、查看栈顶等操作
    
    return 0;
}