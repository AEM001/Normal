#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 表演接口（纯虚类）
class Performable {
public:
    virtual void perform() = 0;
    virtual ~Performable() { cout << "[Performable析构]\n"; }
};

// 动物基类
class Animal {
protected:
    string name;
    int age;
    string gender;
public:
    Animal(string n = "", int a = 0, string g = "") : name(n), age(a), gender(g) {
        cout << "[Animal构造] " << name << endl;
    }

    // 虚析构，支持多态delete
    virtual ~Animal() {
        cout << "[Animal析构] " << name << endl;
    }

    // 拷贝构造
    Animal(const Animal& other) : name(other.name), age(other.age), gender(other.gender) {
        cout << "[Animal拷贝构造] " << name << endl;
    }

    virtual void makeSound() { cout << name << ": Animal sound" << endl; }
    virtual string specialMove() = 0;
    virtual void eat() { cout << name << ": Animal eats" << endl; }

    virtual void display() {
        cout << "Animal: " << name << " Age: " << age << " Gender: " << gender << endl;
    }

    string getName() const { return name; }
    int getAge() const { return age; }
    string getGender() const { return gender; }
};

// 狗
class Dog : public Animal {
public:
    Dog(string n = "", int a = 0, string g = "") : Animal(n, a, g) {
        cout << "[Dog构造] " << name << endl;
    }
    ~Dog() override { cout << "[Dog析构] " << name << endl; }

    Dog(const Dog& other) : Animal(other) {
        cout << "[Dog拷贝构造] " << name << endl;
    }

    void makeSound() override { cout << name << " says: Woof!" << endl; }
    string specialMove() override { return "Jump"; }
    void eat() override { cout << name << " eats dog food" << endl; }
    void display() override {
        cout << "[Dog] " << name << ", " << age << ", " << gender << ", 特技: " << specialMove() << endl;
    }
};

// 鸟
class Bird : public Animal {
public:
    Bird(string n = "", int a = 0, string g = "") : Animal(n, a, g) {
        cout << "[Bird构造] " << name << endl;
    }
    ~Bird() override { cout << "[Bird析构] " << name << endl; }

    Bird(const Bird& other) : Animal(other) {
        cout << "[Bird拷贝构造] " << name << endl;
    }

    void makeSound() override { cout << name << " says: Tweet!" << endl; }
    string specialMove() override { return "Fly"; }
    void eat() override { cout << name << " eats seeds" << endl; }
    void display() override {
        cout << "[Bird] " << name << ", " << age << ", " << gender << ", 特技: " << specialMove() << endl;
    }
};

// 鹦鹉（多重继承：既是鸟，又能表演）
class Parrot : public Bird, public Performable {
public:
    Parrot(string n = "", int a = 0, string g = "") : Bird(n, a, g) {
        cout << "[Parrot构造] " << name << endl;
    }
    ~Parrot() override { cout << "[Parrot析构] " << name << endl; }

    Parrot(const Parrot& other) : Bird(other) {
        cout << "[Parrot拷贝构造] " << name << endl;
    }

    void makeSound() override { cout << name << " says: Hello!" << endl; }
    string specialMove() override { return "Talk"; }
    void eat() override { cout << name << " eats nuts" << endl; }
    void perform() override { cout << name << " performs: I can mimic!" << endl; }
    void display() override {
        cout << "[Parrot] " << name << ", " << age << ", " << gender << ", 特技: " << specialMove() << endl;
        perform();
    }
};

// 动物园
class Zoo {
private:
    vector<Animal*> animals;
public:
    Zoo() = default;
    // 拷贝构造（深拷贝：依次拷贝各动物）
    Zoo(const Zoo& other) {
        cout << "[Zoo拷贝构造]\n";
        for(auto ptr : other.animals) {
            // 类型识别，拷贝派生类
            if (auto d = dynamic_cast<Dog*>(ptr)) animals.push_back(new Dog(*d));
            else if (auto p = dynamic_cast<Parrot*>(ptr)) animals.push_back(new Parrot(*p));
            else if (auto b = dynamic_cast<Bird*>(ptr)) animals.push_back(new Bird(*b));
            // 以后新增种类时，这里也要增加判断
        }
    }
    // 析构：释放全部动物
    ~Zoo() {
        cout << "[Zoo析构]\n";
        for(auto ptr : animals) delete ptr;
        animals.clear();
    }
    void addAnimal(Animal* a) { animals.push_back(a); }
    void showAll() {
        cout << "Zoo内所有动物:\n";
        for(auto ptr : animals) {
            ptr->display();
            ptr->makeSound();
            ptr->eat();
            cout << "---------------------\n";
        }
    }
};

int main() {
    Zoo z;
    z.addAnimal(new Dog("旺财", 3, "M"));
    z.addAnimal(new Parrot("波儿", 2, "F"));
    z.addAnimal(new Bird("雁儿", 1, "F"));

    cout << "\n--- 原始动物园 ---\n";
    z.showAll();

    cout << "\n--- 复制动物园 ---\n";
    Zoo z2 = z; // 拷贝构造
    z2.showAll();

    cout << "\n--- 程序结束时自动析构 ---\n";
    return 0;
}
