#include <iostream>
#include<cmath>
using namespace std;

bool pairc(int a,int b)
{
    int x=(a<=b?a:b);
    int y=(a>=b?a:b);
    while(y%x!=0){
        int temp=y%x;
        y=x;
        x=temp;
    }
    return x==1;
}
void num_pair(int arr[5])
{
    int num=0;
    for(int i=0;i<4;++i)
    {for(int j=i+1;j<5;++j)
        {
        if(pairc(arr[i],arr[j]))
        {num++;}
        }
    }
    if(num==0){cout<<"No pairs of prime numbers";}
    else{cout<<num<<" pairs of prime numbers";}
    return;
}

int main() {

    int arr[5]={3, 18, 14, 17, 12};
    for(int i=0;i<5;++i)
    {cout<<arr[i]<<' ';}
    cout<<endl;
    num_pair(arr);
    
 
}