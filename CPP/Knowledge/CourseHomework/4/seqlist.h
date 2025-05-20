#include<iostream>
#include<ostream>
#include<istream>
template<typename T, int size>
class SeqList {
    T list[size];
    int vol = 0;
public:
    bool isempty() const { return vol == 0; }
    
    void insert(T a, int loc) {
        if(loc < 0 || loc > vol || vol == size) {
            std::cout << "插入位置不合法或表满";
        } else {
            list[loc] = a;
        }
    }
    
    friend std::ostream& operator<<(std::ostream& os, const SeqList& a) {
        if(a.isempty()) {
            os << "空表";
            return os;
        }
        for(int i = 0; i < a.vol; ++i) {
            os << a.list[i] << ' ';
        }
        return os;
    }
    
    friend T& operator>>(std::istream& is, T a)
    {
       is>>a;
        return a;
    }
    bool del(T a)
    {
        for(int i=0;i<vol;i++)
        {
            if(list[i]==a)
            {
                for(int j=i;j<vol-1;j++)
                {
                    list[j]=list[j+1];
                }
                vol--;
                return true;
            }
        }
        return false;
    }
    void insert(T a)
    {
        if(vol==size)
        {std::cout<<"表满";}
        else
        {
            list[vol]=a;
            vol++;
            for(int i=vol;i>=0;i--)
            {
                if(list[i]<list[i-1])
                {T temp=list[i];list[i]=list[i-1];list[i-1]=temp;}

            }
        }

    }
    int biSearch(T a)
    {
        int low=0,high=vol-1,mid;
        while(low<=high)
        {
            mid=(low+high)/2;
            if(list[mid]==a)
            {return mid;}
            else if(list[mid]<a)
            {low=mid+1;}
            else
            {high=mid-1;}
        }
        return -1;
    }
    void sort()
    {for(int i=vol-1;i>0;i--)
        {
            for(int j=0;j<i;j++)
            {
                if(list[j]<list[j+1])
                {T temp=list[j];list[j]=list[j+1];list[j+1]=temp;}
            }
        }
    }
    T& operator[](int i)  // 修改返回类型为T&
    {
        if(i<0||i>=vol)
        {
            std::cout<<"下标不合法";
            static T dummy;  // 返回一个临时对象避免崩溃
            return dummy;
        }
        return list[i];
    }

};// SeqList<T,int size> std::ostream& operator<<(ostream& os,SeqList a)



