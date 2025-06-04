#include <iostream>
#include <stdexcept>
using namespace std;

// 通用链表节点模板类
template<typename T>
class Node {
public:
    T data;
    Node* next;
    
    Node(const T& value) : data(value), next(nullptr) {}
};

// 栈的链表实现
template<typename T>
class LinkedStack {
private:
    Node<T>* top;    // 栈顶指针
    int size;        // 栈大小

public:
    // 构造函数
    LinkedStack() : top(nullptr), size(0) {}
    
    // 析构函数
    ~LinkedStack() {
        clear();
    }
    
    // 拷贝构造函数
    LinkedStack(const LinkedStack& other) : top(nullptr), size(0) {
        *this = other;
    }
    
    // 赋值运算符
    LinkedStack& operator=(const LinkedStack& other) {
        if (this != &other) {
            clear();
            if (other.top != nullptr) {
                copyFrom(other);
            }
        }
        return *this;
    }
    
    // 入栈
    void push(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        newNode->next = top;
        top = newNode;
        size++;
    }
    
    // 出栈
    T pop() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty");
        }
        
        Node<T>* temp = top;
        T data = top->data;
        top = top->next;
        delete temp;
        size--;
        return data;
    }
    
    // 查看栈顶
    T peek() const {
        if (isEmpty()) {
            throw runtime_error("Stack is empty");
        }
        return top->data;
    }
    
    // 判断是否为空
    bool isEmpty() const {
        return top == nullptr;
    }
    
    // 获取大小
    int getSize() const {
        return size;
    }
    
    // 打印栈内容
    void print() const {
        if (isEmpty()) {
            cout << "Stack is empty" << endl;
            return;
        }
        
        cout << "Stack (top to bottom): ";
        Node<T>* current = top;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

private:
    // 清空栈
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    // 从另一个栈复制
    void copyFrom(const LinkedStack& other) {
        if (other.isEmpty()) return;
        
        // 使用递归或辅助栈来保持顺序
        Node<T>* otherCurrent = other.top;
        LinkedStack<T> tempStack;
        
        // 先将元素逆序压入临时栈
        while (otherCurrent != nullptr) {
            tempStack.push(otherCurrent->data);
            otherCurrent = otherCurrent->next;
        }
        
        // 再从临时栈弹出到当前栈，恢复原顺序
        while (!tempStack.isEmpty()) {
            push(tempStack.pop());
        }
    }
};

// 队列的链表实现
template<typename T>
class LinkedQueue {
private:
    Node<T>* front;  // 队头指针
    Node<T>* rear;   // 队尾指针
    int size;        // 队列大小

public:
    // 构造函数
    LinkedQueue() : front(nullptr), rear(nullptr), size(0) {}
    
    // 析构函数
    ~LinkedQueue() {
        clear();
    }
    
    // 拷贝构造函数
    LinkedQueue(const LinkedQueue& other) : front(nullptr), rear(nullptr), size(0) {
        *this = other;
    }
    
    // 赋值运算符
    LinkedQueue& operator=(const LinkedQueue& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }
    
    // 入队（在队尾插入）
    void enqueue(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        
        if (isEmpty()) {
            // 队列为空时，front和rear都指向新节点
            front = rear = newNode;
        } else {
            // 在队尾添加新节点
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }
    
    // 出队（从队头删除）
    T dequeue() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        
        Node<T>* temp = front;
        T data = front->data;
        front = front->next;
        
        // 如果队列变空，rear也要置空
        if (front == nullptr) {
            rear = nullptr;
        }
        
        delete temp;
        size--;
        return data;
    }
    
    // 查看队头元素
    T getFront() const {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        return front->data;
    }
    
    // 查看队尾元素
    T getRear() const {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        return rear->data;
    }
    
    // 判断是否为空
    bool isEmpty() const {
        return front == nullptr;
    }
    
    // 获取大小
    int getSize() const {
        return size;
    }
    
    // 打印队列内容
    void print() const {
        if (isEmpty()) {
            cout << "Queue is empty" << endl;
            return;
        }
        
        cout << "Queue (front to rear): ";
        Node<T>* current = front;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

private:
    // 清空队列
    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }
    
    // 从另一个队列复制
    void copyFrom(const LinkedQueue& other) {
        Node<T>* current = other.front;
        while (current != nullptr) {
            enqueue(current->data);
            current = current->next;
        }
    }
};

// 测试函数
int main() {
    cout << "=== 栈操作演示 ===" << endl;
    
    LinkedStack<int> stack;
    
    // 测试栈操作
    stack.push(10);
    stack.push(20);
    stack.push(30);
    
    cout << "栈大小: " << stack.getSize() << endl;
    stack.print();
    
    cout << "栈顶元素: " << stack.peek() << endl;
    cout << "出栈: " << stack.pop() << endl;
    cout << "出栈: " << stack.pop() << endl;
    
    stack.print();
    
    cout << "\n=== 队列操作演示 ===" << endl;
    
    LinkedQueue<int> queue;
    
    // 测试队列操作
    queue.enqueue(100);
    queue.enqueue(200);
    queue.enqueue(300);
    queue.enqueue(400);
    
    cout << "队列大小: " << queue.getSize() << endl;
    queue.print();
    
    cout << "队头元素: " << queue.getFront() << endl;
    cout << "队尾元素: " << queue.getRear() << endl;
    
    cout << "出队: " << queue.dequeue() << endl;
    cout << "出队: " << queue.dequeue() << endl;
    
    queue.print();
    
    // 继续入队测试
    queue.enqueue(500);
    cout << "入队500后: ";
    queue.print();
    
    cout << "\n=== 异常处理测试 ===" << endl;
    
    LinkedQueue<int> emptyQueue;
    LinkedStack<int> emptyStack;
    
    try {
        emptyQueue.dequeue();
    } catch (const exception& e) {
        cout << "队列异常: " << e.what() << endl;
    }
    
    try {
        emptyStack.pop();
    } catch (const exception& e) {
        cout << "栈异常: " << e.what() << endl;
    }
    
    cout << "\n=== 拷贝构造测试 ===" << endl;
    
    LinkedQueue<int> queue2 = queue;  // 拷贝构造
    cout << "原队列: ";
    queue.print();
    cout << "复制队列: ";
    queue2.print();
    
    return 0;
}