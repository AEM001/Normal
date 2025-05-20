// 要求：

// 1. 基类Character包含生命值属性和纯虚函数attack()

// 2. 派生类Warrior和Mage实现不同攻击方式

// 3. 实现治疗者类Healer（继承自Mage）

// 4. 创建战斗场景测试多态

//想要有一个名字的属性，同时不知道生命值的类型如何确定，protected？为什么在类的对象之间的protected的也不能访问
//为什么livevalue需要反复写


#include<iostream>
using namespace std;

class Character {
protected:
    int livevalue = 5;
    string name;

public:
    Character(string n):name(n) {}
    virtual void attack(Character &target) = 0;
    virtual ~Character() = default;
    string getname(){return name;}
    int getlivevalue(){return livevalue;}
    void setvalue(int a){
        livevalue-=a;
        if(livevalue<0)livevalue=0;
        if(a==5)livevalue=5;
    }
};

class Warrior : public Character {
public:
    Warrior(string w="fighter"):Character(w){} //为什么是这样？
    void attack(Character & a) override {
        a.setvalue(2);
        cout << "Warrior is shooting " << a.getname()<<' '<<"and the lifevalue of it is "<<a.getlivevalue()<<endl;
    }
};

class Mage : public Character {
public:
    Mage(string m="mager"):Character(m){}
    void attack(Character & a) override {
        a.setvalue(1);
        cout << "Mage is shooting " << a.getname()<<' '<<"and the lifevalue of it is "<<a.getlivevalue()<<endl;
    }
};

class Healer : public Mage {
public:
    Healer(string h="Healer"):Mage(h){}
    void heal(Character & a){
        a.setvalue(5);
        cout<<"healer is healing "<<a.getname()<<' '<<"the livevalue is "<<a.getlivevalue();
    }
};

int main() {
    Character* w1 = new Warrior();
    Character* m1 = new Mage();
    Healer* h1 = new Healer();
    w1->attack(*m1);
    h1->heal(*m1);
    delete w1;
    delete m1;
    delete h1;
}