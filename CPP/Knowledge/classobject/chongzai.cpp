#include<iostream>
using namespace std;
class Bitch{
    public:
    int age;
    int height;
    Bitch() : age(0), height(0) {} // 默认构造函数
    Bitch(int a,int b):age(a),height(b){};
    Bitch operator+(const Bitch& one)const{
        return Bitch(age+one.age,height+one.height);
    }

};
int main(){
Bitch b1(19,180),b2(20,180),b3;
b3=b1+b2;
cout<<b3.age<<' '<<b3.height; 
}