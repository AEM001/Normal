#include<iostream>
#include<string>
#include<vector>
using namespace std;
// 还是没有深刻理解虚函数的作用和意义
// 析构函数究竟怎么搞
class animal{
protected:
    string name;
    int age;
    string gender;
public:
    animal(string n=NULL,int a=0,string g=NULL):name(n),age(a),gender(g){}
    virtual void makesound(){cout<<"animal speak;";}
    virtual string specialmove()=0;
    virtual void eat()
    {
        cout<<"eating"<<endl;
    }
   
    string getname(){return name;}
    int getage(){return age;}
    string getgender(){return gender;}
    // void display 如何实现特殊行为的展示输出，如果是定义为纯虚函数的话？maybe 也可以定义一个展示特殊行为的纯虚函数
};
class zoo{
private:
    vector<animal*> aset;
    // 需要存储指针
    int n;
public:
    zoo(vector<animal*> anset=vector<animal*>(),int num=0):aset(anset),n(num){}
    void display(){
        // 需要定义动物类的接口函数之类的
        for(auto& ani:aset){
            cout<<ani->getname()<<' '<<ani->getage()<<' '<<ani->getgender ()<<"    "<<ani->specialmove()<<" ";
        }
    }

};
class dog:public animal{
public:
    dog(string n=NULL,int a=0,string g=NULL):animal(n,a,g){}
    void makesound() override{
        cout<<"dog bake"<<endl;
    }
    string specialmove(){
        return "jump";
    }
    void eat()override{
        cout<<"eating dog food"<<endl;
    }

};
class cat:public animal{
    public:
    cat(string n=NULL,int a=0,string g=NULL):animal(n,a,g){}
        void makesound() override{
        cout<<"cat bake"<<endl;
    }
    string specialmove(){
        return "fly";
    }
    void eat()override{
        cout<<"eating cat food"<<endl;
    }
};
class lcat:public cat{
    public:
    lcat(string n=NULL,int a=0,string g=NULL):cat(n,a,g){}
            void makesound() override{
        cout<<"cat bake"<<endl;
    }
    string specialmove(){
        return "lllfly";
    }
    void eat()override{
        cout<<"eating cat food"<<endl;
    }
};
int main(){
    dog d1("曾礼",1,"male") ;
    dog d2("zl",2,"male");
    cat c1("zengli",3,"female");
    cat c2("zengl",4,"male");
    lcat l1("曾l",5,"medium");
    vector<animal*> ani={&d1,&d2,&c1,&c2,&l1};
    zoo z(ani,5);
    z.display();
    return 0;
}