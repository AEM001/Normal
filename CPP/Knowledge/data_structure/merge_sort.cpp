#include<iostream>
#include<vector>
using namespace std;
vector<int>merge(vector<int>&a,int l,int m,int r)
{
    vector<int>temp(r-l+1);
    int i=l,j=m+1,k=0;
    while(i<=m&&j<=r)
    {
        if(a[i]<=a[j]){
            temp[k++]=a[i++];}
            else{
                temp[k++]=a[j++];
            }
        
    }
    while(i<=m){
        temp[k++]=a[i++];
    }
    while(j<=r){
        temp[k++]=a[j++];
    }
    for(int p=0;p<k;p++)
    {
        a[l+p]=temp[p];
    }
    return a;
}
void mer(vector<int>& a, int l, int r){
    if(l<r){
        int m=l+(r-l)/2;
        
        mer(a,l,m);
        mer(a,m+1,r);
        merge(a,l,m,r);
    }
    else{return ;}
}
int main()
{
    vector<int>a={12,11,13,5,6,7};
    mer(a,0,a.size()-1);
    for(int i=0;i<a.size();i++)
    {
        cout<<a[i]<<" ";
    }
    return 0;
}