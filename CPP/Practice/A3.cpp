#include <iostream>

using namespace std;

class Card
{
 public:  
     int left, top, bottom, right; // 左 上 下 右

    //  完善代码1： 完成构造函数(5分)
    Card(int a=0,int b=0,int c=0,int d=0):left(a),top(b),bottom(c),right(d){}
    

    friend ostream& operator<<(ostream& os, const Card& card) // 按左 上 下 右的次序显示
    {
        return os << '(' << card.left << ' ' << card.top << ' ' << card.bottom
            << ' ' << card.right << ')';
    }
};

class CardList
{
    int size;   // 卡牌数组容量
    int number; // 卡牌的实际个数
    Card* list; // 指向动态卡牌数组的指针
public:

    // 完善代码2：完成缺省构造函数，将size、number都置为0，list置为nullptr(5分)
    CardList():size(0),number(0),list(nullptr){}

    // 接受外部传来的长度为size的数组arr, 并将数组元素依次添加到list当中
    CardList(const Card* arr, int size_) : size(size_)        
    {
        number = 0;
        list = new Card[size];
        for (int i = 0; i < size; ++i)
            append(arr[i]);
    }

    // 完善代码3: 根据main函数测试代码需求，完善CardList类设计(注意动态内存管理，多个函数)(15分)
   CardList(const CardList& a):size(a.size),number(a.number),list(new Card[size])
   {for(int i=0;i<size;++i)
    {list[i]=a.list[i];}}
    CardList& operator=(const CardList b)
    {if(this!=&b)
        {size=b.size;
        number=b.number;
        for(int i=0;i<size;++i)
            {list[i]=b.list[i];}
        }
        return *this;
    }
    ~CardList(){for(int i=0;i<size;++i)
    {
     delete[] list;
     }
    }


    void append(const Card& card)
    {
        // 已有数组元素下标从0到number - 1，新卡片待插入的位置是从 number 到 0递减选择
        // 从第一个待插入位置number开始进行判断，如无法插入则更改至下一个待插入位置判断
        // 如果有任何位置可以插入，则调用insert函数进行插入，则提前返回，如果所有位置均无法插入则放弃
        // 特别提醒：注意判断数组空的问题，以及判断比较时的下标越界问题（最左侧插入时只需要和右侧比较，最右侧插入时只需要和左侧比较）
        if (number == 0)
        {
            list[number++] = card;  // 空表时插入
            return;
        }
        
        for (int i = number; i >= 0; --i) // i是待插入位置
        {
            if ((i == 0 && card.right == list[i].left) || (i == number && card.left == list[i - 1].right) || (card.right == list[i].left && card.left == list[i - 1].right))
            {
                insert(i, card);    // 将card，插入下标索引i处
                return;
            }
        }
    }

    // 在下标为index的位置插入card
    void insert(int index, const Card& card)        
    {
        // 完善代码4：完成insert设计，配合append函数实现卡牌插入列表功能
        //            在list所指向的动态数组的下标index处插入card，卡牌个数加1(5分)
        //            本题确保 list容量充足(number < size)，无需扩容操作
        //            本题确保 0 <= index <= number,可以不判断越界问题
        

    }

    // 完善代码5：完成CardList类的输出运算符重载，实现CardList数据输出功能(5分)
    

};

int main()
{
    Card cards[7] = { Card(4, 4, 7, 3), Card(3, 6, 3, 4), Card(4, 1, 3, 2), Card(2, 3, 5, 2),
                     Card(2, 5, 2, 9), Card(2, 4, 9, 2), Card(8, 4, 9, 2) };

    CardList cl(cards, 7);
    cout << cl ;

    CardList c2(cl);
    cout << c2;

    CardList c3;
    cout << c3;
    c2 = c3;
    cout << c2;
    c3 = cl;
    cout << c3;
    return 0;
}

/********************************************
思考题：如果给卡牌类加上旋转功能，允许在插入失败时进行旋转尝试，
        那么append函数的逻辑应该如何变化，简述设计思路。(5分)
答：


 ********************************************/

