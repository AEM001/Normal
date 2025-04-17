#include<iostream>
#include<string>
using namespace std;
class Base{
public:
string a="base";
};
class d1 : public Base{
    public:
    string b="d1";
};

int main(){
    d1 depri1;
    cout<<depri1.a<<endl;
    cout<<depri1.b;
}