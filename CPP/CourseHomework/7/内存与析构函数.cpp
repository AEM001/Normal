#include <iostream>
using namespace std;

class A { public: int x; };
class B : public A { public: double y; };
class C : public B { public: char z; };

int main() {
    C obj;
    cout << "C�����ַ: " << &obj << endl;
    cout << "��ΪA*�ĵ�ַ: " << (A*)&obj << endl;
    cout << "�ȽϽ��: " << ( (void*)&obj == (void*)static_cast<A*>(&obj) );
}