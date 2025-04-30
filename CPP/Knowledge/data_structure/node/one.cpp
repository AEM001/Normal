/*#include <iostream>
using namespace std;

class Node {
    public:
    int d;
    Node* ptr;
    Node(int val):d(val),ptr(nullptr){}
};

int main() {
    int a[3]={3,5,7};
    Node* tail=new Node(a[0]);
    Node* head=tail;
    for(int i=1;i<3;++i){
        tail->ptr = new Node(a[i]); // 创建新节点并链接到尾节点
        tail = tail->ptr;
    }         
   
    for(int i=0;i<3;++i){
        static Node* read=head;
        cout<<read->ptr;
        read=read->ptr;
    }
    int hea=0;
    Node* insertathead=new Node(hea);
    insertathead->ptr=head;
    int back=9;
    Node* insertatback=new Node(back);
    tail->ptr=insertatback;
    
    return 0;
}
*/
#include <iostream>
using namespace std;

// 定义链表节点结构
struct Node {
    int data;       // 数据域
    Node* next;     // 指针域，指向下一个节点

    // 构造函数
    Node(int val) : data(val), next(nullptr) {}
};

// 尾插法构建链表
Node* createListWithTailInsert(int values[], int n) {
    if (n == 0) return nullptr; // 如果数组为空，返回空链表

    Node* head = nullptr; // 链表头节点
    Node* tail = nullptr; // 链表尾节点

    for (int i = 0; i < n; i++) {
        Node* newNode = new Node(values[i]); // 创建新节点

        if (head == nullptr) {
            head = newNode; // 如果链表为空，新节点为头节点
        } else {
            tail->next = newNode; // 否则，将新节点链接到尾节点
        }
        tail = newNode; // 更新尾节点
    }

    return head; // 返回链表头节点
}

// 遍历链表
void printList(Node* head) {
    Node* current = head; // 从头节点开始
    while (current != nullptr) { // 遍历到链表末尾
        cout << current->data << " -> "; // 打印当前节点的数据
        current = current->next; // 移动到下一个节点
    }
    cout << "NULL" << endl;
}

// 在链表尾部插入新节点
void insertAtTail(Node* &head, int value) {
    Node* newNode = new Node(value); // 创建新节点

    if (head == nullptr) {
        head = newNode; // 如果链表为空，新节点为头节点
        return;
    }

    Node* tail = head; // 从头节点开始查找尾节点
    while (tail->next != nullptr) { // 遍历到尾节点
        tail = tail->next;
    }

    tail->next = newNode; // 将新节点链接到尾节点
}

// 删除链表中第一个值为 value 的节点
void deleteNode(Node* &head, int value) {
    if (head == nullptr) { // 如果链表为空，直接返回
        return;
    }

    if (head->data == value) { // 如果要删除的是头节点
        Node* temp = head;
        head = head->next; // 更新头节点为下一个节点
        delete temp; // 释放原来的头节点
        return;
    }

    Node* current = head; // 从头节点开始查找
    while (current->next != nullptr && current->next->data != value) {
        current = current->next; // 移动到下一个节点
    }

    if (current->next != nullptr) { // 如果找到目标节点
        Node* temp = current->next; // 保存目标节点
        current->next = current->next->next; // 删除目标节点
        delete temp; // 释放目标节点
    }
}

// 释放链表内存
void freeList(Node* head) {
    while (head != nullptr) {
        Node* temp = head; // 保存当前节点
        head = head->next; // 移动到下一个节点
        delete temp; // 释放当前节点
    }
}

int main() {
    // 初始化数组
    int values[] = {1, 2, 3};
    int n = sizeof(values) / sizeof(values[0]);

    // 使用尾插法构建链表
    Node* head = createListWithTailInsert(values, n);
    cout << "初始链表: ";
    printList(head); // 输出: 1 -> 2 -> 3 -> NULL

    // 在链表尾部插入新节点
    insertAtTail(head, 4);
    cout << "插入节点 4 后的链表: ";
    printList(head); // 输出: 1 -> 2 -> 3 -> 4 -> NULL

    // 删除节点
    deleteNode(head, 2);
    cout << "删除节点 2 后的链表: ";
    printList(head); // 输出: 1 -> 3 -> 4 -> NULL

    // 释放链表内存
    freeList(head);
    head = nullptr; // 置空头节点指针

    return 0;
}
