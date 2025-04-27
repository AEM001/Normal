#include <iostream>
using namespace std;

class A { public: int x; };
class B : public A { public: double y; };
class C : public B { public: char z; };

int main() {
    C obj;
    cout << "C对象地址: " << &obj << endl;
    cout << "作为A*的地址: " << (A*)&obj << endl;
    cout << "比较结果: " << ( (void*)&obj == (void*)static_cast<A*>(&obj) );
}