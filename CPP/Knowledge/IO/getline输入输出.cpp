#include <iostream>
#include<fstream>
using namespace std;
int main(){
ofstream outfile("1.txt");
outfile<<"hello C++ File IO"<<endl;
outfile<<"this is the second line"<<endl;
outfile.close();
ifstream infile("1.txt");
string line;
int i=1;
if(infile.is_open()){
    while(getline(infile,line))
    {cout<<line<<endl;
    cout<<i++<<endl;}
    infile.close();
}
return 0;
}