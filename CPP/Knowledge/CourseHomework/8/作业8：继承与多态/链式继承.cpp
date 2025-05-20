#include <iostream>
using namespace std;

class Person {
    string name;
    int age;
public:
    Person(string n = "", int a = 0) : name(n), age(a) {}
    virtual void display() { cout << "Person: " << name << ", " << age << endl; }
    virtual ~Person() {}  // 更明确的虚析构函数
};

class Employee : public Person {
    int employId;
    string department;
public:
    Employee(string n = "", int a = 0, int id = 0, string dept = "") 
        : Person(n, a), employId(id), department(dept) {}
    void display() override {
        cout << "Employee: " << employId << ", " << department << endl;
    }
};

class Manager : public Employee {
    int teamSize;
    int projectCount;
public:
    Manager(string n = "", int a = 0, int id = 0, string dept = "", 
           int size = 0, int count = 0)
        : Employee(n, a, id, dept), teamSize(size), projectCount(count) {}
    void display() override {
        cout << "Manager: team size=" << teamSize 
             << ", projects=" << projectCount << endl;
    }
};

int main() {
    Person p("Alice", 30);
    p.display();
    
    Employee e("Bob", 25, 1001, "IT");
    e.display();
    
    Manager m("Carol", 40, 2001, "Engineering", 10, 5);
    m.display();
    
    // 多态演示
    Person* poly = new Manager("Dave", 35, 3001, "Management", 8, 3);
    poly->display();
    delete poly;  // 记得释放内存
}