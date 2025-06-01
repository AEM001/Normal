#include<iostream>
#include<fstream>
using namespace std;
int main()
{
    int a[250];
    for(int i=1;i<=250;++i)
    {a[i-1]=2*i;}
    ofstream outfile("data.dat",ios::binary);
    outfile.write(reinterpret_cast<char*>(a),250*sizeof(int));
    outfile.close();
    ifstream infile("data.dat",ios::binary);
    int* readin=new int[250];
    infile.read(reinterpret_cast<char*>(readin),250*sizeof(int));
    for(int i=0;i<250;++i)
    {cout<<readin[i]<<' ';}
}