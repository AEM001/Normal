#include<iostream>
using namespace std;
template<typename T>
class Node{
public:
T data;
Node* head;

Node(T d=0,Node* n=nullptr):data(d),head(n){}
};

template<typename T>
class seq{
Node<T>* rear;
Node<T>* front;
int size;

public:


seq(Node<T>* f=NULL,Node<T>* r=NULL):front(f),rear(r),size(0){}
void ins(T d){
    
    Node<T>* back=new Node(d);
    if(front==nullptr){front=back;}
    else{rear->next=back;}
    rear=back;
    ++size;
}
void outs(){
    Node<T>* temp=head->next;
    delete[] front;
    front=temp;
    --size;
}
void displayh(){
    cout<<front->data<<endl;
}
void displayr(){
    cout<<rear->data<<endl;
}
};

int main(){
    seq<int> s;
    s.ins(7);
    s.displayh();
    s.displayr();


}