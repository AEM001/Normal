#include <iostream>
#include <fstream>
#include <string>
#include<cstring>
using namespace std;

//--- 商品类 inventory ——
// 你需要完成的内容：
// 1. 实现构造函数（包括默认值）
// 2. 完善输入输出重载运算符 >> <<
// 3. 完善比较运算符 < >，用于排序和插入
class inventory {
    char Name[20];       // 商品名称
             // 货号（关键字）
    int Quantity;      // 数量
    double Cost;       // 价格
    double TotalValue; // 总价（可选维护）

public:
    // 构造函数
    inventory(string name = "#",  int qty = 0, double cost = 0):Quantity(qty),Cost(cost){
        strncpy(Name,name.c_str(),strlen(name.c_str()));
        Name[strlen(name.c_str())] = '\0'; // 修改：确保字符串以null结尾
        TotalValue=Quantity*Cost;
    }

    // 运算符重载（友元函数）
    friend istream& operator>>(istream & is,inventory& inv)
    {
        is>>inv.Name>>inv.Quantity>>inv.Cost;
        inv.TotalValue = inv.Quantity * inv.Cost; // 修改：计算总价
        return is;
    }
    friend ostream& operator<<(ostream &os,const inventory& inv)
    {
        os<<inv.Name<<' '<<' '<<inv.Quantity<<' '<<inv.Cost<<endl;
        return os;
    }


    // 比较运算符，用于排序插入
    friend bool operator<(const inventory& inv1,const inventory& inv2)
    {return inv1.TotalValue<inv2.TotalValue;}


    // TODO: 如果需要，可以添加计算总价函数，或者自动维护总价
};


template <typename T>
class Array {
private:
    T* elements;      // 动态数组
    int last;         // 最后一个有效数据下标
    int maxSize;      // 容量
    char filename[20];  // 关联文件名

public:
    // 构造函数，读取文件初始化数据，容量缺省20
    Array(string fname, int maxs = 20):maxSize(maxs),last(-1){
        strncpy(filename,fname.c_str(),strlen(fname.c_str()));
        filename[strlen(fname.c_str())] = '\0'; // 修改：确保字符串以null结尾
        elements = new T[maxSize]; // 修改：必须分配内存
        ifstream infile(filename,ios::binary);
        if(infile.is_open())
        {infile.seekg(0,ios::end);
        streamsize filesize=infile.tellg();
        infile.seekg(0,ios::beg);
        int fileElements=filesize/sizeof(T); // 修改：去掉+1，这里计算错误
        if(fileElements>0)
        {if(fileElements>maxSize)
            {renew(fileElements);}
        
        infile.read(reinterpret_cast<char*>(elements),sizeof(T)*fileElements) ;  
        // T temp;
        // while(last<=maxSize-1&&infile>>temp)//amazing
        // {last++;
        // elements[last]=temp;}
        // infile.close();
        if (infile.good() || infile.eof()) {
                last = fileElements - 1;
            }
        }
        infile.close();
        }

    }

    // 构造函数，不读取文件，仅初始化容量
    Array(int maxs = 20):maxSize(maxs),last(-1) { // 修改：去掉对filename的错误初始化
        elements = new T[maxSize]; // 修改：必须分配内存
        filename[0] = '\0'; // 修改：正确初始化filename为空字符串
    }
// 析构时将数据写入文件，并释放内存
    ~Array() {
        if(filename[0] != '\0') { // 修改：只有当filename非空时才写文件
            ofstream outfile(filename,ios::binary|ios::trunc);
            outfile.write(reinterpret_cast<char*>(elements),(last+1)*sizeof(T));
            outfile.close();
        }
        delete[] elements;
    }
    
//好好好，第一次代码复用是吧！
    bool isFull() const { return last == maxSize - 1; }
// 扩容逻辑
    void renew(int newSize)
    {
        if (newSize <= maxSize) return;
        T *newlist = new T[newSize];
        for (int i = 0; i <= last; ++i) {
            newlist[i] = elements[i];
        }
        delete[] elements;
        elements = newlist;
        maxSize = newSize;
    }               

    void insert(const T& item, int pos)  // 位置插入
    {        // 检查位置合法性,当时只检查了满了的情况，并且也没有进行扩容……
        if (pos < 0 ) {
            return; // 或抛出异常
        }
        // 判断数组是否已满，调用renew扩容
        if (isFull()) {
            renew(maxSize+5);
        }
        if(pos<=last){
            for(int i=last;i>=pos;--i)
            {elements[i+1]=elements[i];}//或许应该考虑一下内存的问题
        }
        elements[pos]=item;
        last++;

    }
    void insertOrder(const T& item)      // 按升序插入
    {
        if(isFull()){renew(maxSize+100);}
        
        // 修改：重写insertOrder逻辑，修复原来的bug
        if(last==-1){ // 如果数组为空，直接插入
            last++;
            elements[last]=item;
            return;
        }
        
        // 找到插入位置
        int insertPos = 0;
        while(insertPos <= last && elements[insertPos] < item) {
            insertPos++;
        }
        
        // 移动元素
        for(int i = last; i >= insertPos; i--) {
            elements[i+1] = elements[i];
        }
        
        // 插入新元素
        elements[insertPos] = item;
        last++;
    }

    // 友元输出运算符，方便输出数组内容
    friend ostream& operator<<(ostream& os, const Array<T>& arr){
        for(int i=0;i<=arr.last;++i)
        {os<<arr.elements[i];}
        return os;
    }
};


// 主函数示例，建议你实现测试
int main()
{
    // 创建带文件名的商品数组对象，自动读取文件
    Array<inventory> mylist("mydata.dat", 50);

    cout << "请输入2个商品信息：" << endl;
    inventory temp;

    for (int i = 0; i < 2; i++) {
        cin >> temp;
        mylist.insertOrder(temp);  // 按顺序插入
    }

    cout << "当前商品列表：" << endl;
    cout << mylist;

    // 程序结束时析构函数自动保存数据

    return 0;
}