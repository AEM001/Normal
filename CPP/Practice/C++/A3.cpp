#include <iostream>
#include <cmath>
#include <iomanip>
#include <cassert>

using namespace std;

template<typename T>      
class MyLink;

//----------------结点类模板----------------
template<typename T>
class Node {
    T data;                     //数据域
    Node<T> *next;              //指针域
  public:
    Node(T _data) : data(_data), next(nullptr) {};
    T& GetData()                                 //获取结点数据
    {
        return data;
    }        
    friend ostream& operator<<(ostream& _out, const Node<T>& _node)
    {
        _out << _node.data ;
        return _out;
    }
  
    friend class MyLink<T>;                  
};

//-----------单向链表类模板------------------
template<typename T>
class MyLink {
    Node<T> *head, *tail;                           //链表头、尾指针
  public:
    MyLink();
    MyLink(const MyLink<T>&);                       //拷贝构造函数
    ~MyLink();                                      //析构
    void InsertNode(Node<T>* _node);                //在表尾插入结点
    int GetLength()const ;                          //获取链表长度
    T& operator[](int i) const;                     //获取第i个结点数据（从0开始）
    void OutputLink();                              //输出链表数据
};

template<typename T>
MyLink<T>::MyLink() : head(nullptr), tail(nullptr){}

//拷贝构造函数
template<typename T>
MyLink<T>::MyLink(const MyLink<T>& ls)
{
    Node<T>* TempP = ls.head, * P1;
    head = tail = nullptr;
    while (TempP != nullptr) {
        P1 = new Node<T>(TempP->data);
        P1->next = tail->next;				
        tail->next = P1;
        tail = P1;
        TempP = TempP->next;
    }
}

template<typename T>
MyLink<T>::~MyLink()
{
    Node<T>* curr = head;
    while(curr != nullptr){
        Node<T> *DeletedNode = curr;
        curr = curr->next;
        delete DeletedNode;
    }
}

template<typename T>
int MyLink<T>::GetLength() const
{
    int len = 0;
    Node<T>* curr = head;
    while (curr)
    {
        ++len;
        curr = curr->next;
    }
    return len;
}

template<typename T>
void MyLink<T>::InsertNode(Node<T>* _node)
{
    //（5分）添加代码1: 将将结点 _node插入链表尾
    if(head==nullptr)
    {
        head=_node;
        tail=_node;
    }
    else{tail->next=_node;
    tail=tail->next;}
}

template<typename T>
T& MyLink<T>::operator[](int i) const
{
    //(4分) 添加代码2: 获取第i个结点数据（i从0开始）
    Node<T>* current=head;
    while(i>0)
    {current=current->next;
    --i;}//OOOOOMyMyMyMyMyGodGodGodGod!!!!!!!
    return current->data;
}

template<typename T>
void MyLink<T>::OutputLink()
{
    if(head == nullptr) return;
    Node<T>* curr = head;
    while(curr)
    {
        cout << curr->GetData() << endl;
        curr = curr->next;
    }
}

// 类HealthyBF：健康体态
class HealthyBF {
    double height;              //身高
    double weight;              //体重    
    double BMI;                 //体重指数
public:
    //(8分)添加代码3：定义必要的构造函数 和 成员函数
    HealthyBF(double h=1,double w=0):height(h),weight(w){BMI=weight/(height*height);}

    friend ostream& operator<<(ostream& _out, const HealthyBF& _body)
    {
        _out << setw(6) << _body.height << setw(6) << _body.weight << setw(8) << setprecision(4) << _body.BMI;
        return _out;
    }
    friend istream& operator>>(istream& _in, HealthyBF& _body)
    {
        _in >> _body.height >> _body.weight;
        _body.BMI = _body.weight / (_body.height * _body.height);
        return _in;
    }
    friend bool operator>(const HealthyBF&a,const HealthyBF& b)
    {return a.BMI>b.BMI;}
};

// 函数模板OutputOverlimit: 输出单向链表中所有超过上限的结点数据信息
// list: 单向链表对象
// upperlimit: 上限对象
template <typename T>
void OutputOverlimit(const MyLink<T>& list, const T  &upperlimit)
{
    for (int i = 0; i < list.GetLength(); ++i)
    {
        if (list[i] > upperlimit)
            cout << list[i] << endl;
    }
}//首先需要得到一个存储了超过上限节点的数据的T类型的**链表**是否需要运算符重载

int main()
{
    double a[10] = { 1.69, 1.54, 1.70, 1.73, 1.60, 1.81, 1.57, 1.72, 1.71, 1.63 };  //一组身高数据
    double b[10] = { 65,50,55,79,57,74,48,60,64,63 };                               //一组体重数据
    
    MyLink<HealthyBF> hlist;            //通过HealthyBF类型实例化单向链表hlist
    //（5分）添加代码4：使用数组a，数组b中的身高、体重数据，初始化单向链表hlist         
    for(int i=0;i<10;++i)
    {HealthyBF temp(a[i],b[i]);
        Node<HealthyBF>* tempnode=new Node<HealthyBF>(temp);
        hlist.InsertNode(tempnode);//存疑，待检查
    }


    cout << "所有人员体态指标：" << endl;
    hlist.OutputLink();

    HealthyBF upperlimit(1.75, 73.2);     //身高1.75，体重73.2对应的BMI值约为23.9，为健康体态上限
    cout << "超重人员体态指标：" << endl;
    OutputOverlimit(hlist, upperlimit);
        
    return 0;
}

/********************** 问答题 ****************************
问题：在本题代码设计中，单向链表类MyLink的成员函数OutputLink 和 函数OutputOverlimit采用了两种不同的链表遍历方法，
      从算法效率角度，哪种方法更合适？简述理由。
答：
limit更为合理，只涉及到单个node节点，空间复杂度和时间复杂度都较低



**********************************************************/