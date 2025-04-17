#include<iostream>
using namespace std;
// 不要随意使用static变量，将逻辑梳理清楚
// 注意&&而不是，
// 尤其注意边界条件
void fill(int **a,int m)
{
   int count=1;
   int width=0;
    while(count<=m*m)
    {
        for(int i=width;i<m-width&&count<=m*m;++i,++count)
        {a[width][i]=count;
        }
        for(int i=1+width;i<m-width&&count<=m*m;++i,++count)
        {a[i][m-width-1]=count;}
        for(int i=m-2-width;i>=width&&count<=m*m;--i,++count)
        {a[m-width-1][i]=count;}
        for(int i=m-2-width;i>width&&count<=m*m;--i,++count)
        {a[i][width]=count;}
        width++; 
         }
    for(int i=0;i<m;++i)
    {
        for(int j=0;j<m;++j)
        {
            cout<<a[i][j]<<' ';
        }
        cout<<endl;
    }
}
int main()
{
    int m;
    cin>>m;
    int **a=new int*[m];
    for(int i=0;i<m;++i)
    {
        a[i]=new int[m];
  }
  fill(a,m);
  for(int i=0;i<m;++i)
  {delete []a[i];}
    delete []a;
}