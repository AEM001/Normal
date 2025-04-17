#include <iostream>   
#include <stdexcept>
using namespace std;

template<typename T, int size>
class SeqList
{
private:
    T list[size];
    int last;
    
public:
    // 构造函数，初始化顺序表为空表
    // 将last指针初始化为-1，表示表中无元素
    SeqList() : last(-1) {}
    
    // 判断是否为空
    bool empty() const
    {
        return (last == -1);
    }

    // 判断是否满
    bool full() const
    {
        return (last == size - 1);
    }
    
    // 降序插入元素
    bool insert(T a)
    {
        if (full())
            return false;
            
        int i;
        for (i = last; i >= 0 && list[i] < a; i--)
            list[i + 1] = list[i];
            
        list[i + 1] = a;
        last++;
        return true;
    }
    
    // 将元素插入特定的位置
    bool insert(T a, int i)
    {
        if (full() || i < 0 || i > last + 1)
            return false;
            
        for (int j = last; j >= i; j--)
            list[j + 1] = list[j];
            
        list[i] = a;
        last++;
        return true;
    }
    
    // 查询元素
    int search(T a) const
    {
        for (int i = 0; i <= last; i++)
        {
            if (list[i] == a)
                return i;
        }
        return -1;
    }
    
    // 删除元素
    bool del(T a)
    {
        int loc = search(a);
        if (loc == -1)
            return false;
            
        for (int i = loc; i < last; i++)
            list[i] = list[i + 1];
            
        last--;
        return true;
    }
    
    // 使用插入排序进行降序排序
    void sort()
    {
        for (int i = 1; i <= last; i++)
        {
            T temp = list[i];
            int j = i - 1;
            
            while (j >= 0 && list[j] < temp)
            {
                list[j + 1] = list[j];
                j--;
            }
            
            list[j + 1] = temp;
        }
    }
    
    // 重载运算符
    T& operator[](int n)
    {
        if (n > last || n < 0)
        {
            throw std::out_of_range("Invalid index"); 
        }
        return list[n];
    }
    
    //重载
    friend std::ostream& operator<<(std::ostream& os, const SeqList& a)
    {
        if (a.empty())
        {
            os << "空表";
            return os;
        }
        
        for (int i = 0; i <= a.last; i++)
        {
            os << a.list[i];
            if (i < a.last)
                os << " ";
        }
        return os;
    }


    int biSearch(T a) {
        return biSearchHelper(a, 0, last);
    }

    // 真正的递归实现
    int biSearchHelper(T a, int low, int high) {
        if (low > high) // 终止条件：区间为空
            return -1;

        int mid = low + (high - low) / 2; 

        if (a == list[mid]) { // 找到目标值
            return mid;
        } else if (a < list[mid]) { // 在左半部分继续查找
            return biSearchHelper(a, low, mid - 1);
        } else { // 在右半部分继续查找
            return biSearchHelper(a, mid + 1, high);
        }
    }

};