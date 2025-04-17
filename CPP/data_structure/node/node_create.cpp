#include <iostream>
#include <string>
using namespace std;

// 模板化的链表节点类
template <typename T>
class Node {
public:
    T data;
    Node<T>* next;
    
    Node(T val) : data(val), next(nullptr) {}
};

// 模板化的链表类
template <typename T>
class LinkedList {
private:
    Node<T>* head;
    
public:
    LinkedList() : head(nullptr) {}
    
    ~LinkedList() {
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
    }
    
    void insertAtHead(T val) {
        Node<T>* newNode = new Node<T>(val);
        newNode->next = head;
        head = newNode;
    }
    
    void insertAtTail(T val) {
        Node<T>* newNode = new Node<T>(val);
        
        if (head == nullptr) {
            head = newNode;
            return;
        }
        
        Node<T>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
    
    void deleteNode(T val) {
        if (head == nullptr) return;
        
        if (head->data == val) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        
        Node<T>* current = head;
        while (current->next != nullptr && current->next->data != val) {
            current = current->next;
        }
        
        if (current->next != nullptr) {
            Node<T>* temp = current->next;
            current->next = current->next->next;
            delete temp;
        }
    }
    
    void printList() {
        Node<T>* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
};

int main() {
    // 测试整型链表
    cout << "整型链表测试:" << endl;
    LinkedList<int> intList;
    intList.insertAtHead(3);
    intList.insertAtHead(2);
    intList.insertAtHead(1);
    intList.deleteNode(2);
    intList.printList();
    
    // 测试字符串链表
    cout << "\n字符串链表测试:" << endl;
    LinkedList<string> strList;
    strList.insertAtTail("hello");
    strList.insertAtTail("world");
    strList.insertAtTail("cpp");
    strList.deleteNode("world");
    strList.printList();
    
    return 0;
}