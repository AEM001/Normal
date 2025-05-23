#include<iostream>
#include<fstream>
using namespace std;

int main(){
    ofstream outfile("2.dat",ios::binary);
    int num=12358;
    outfile.write((char*)(&num),sizeof(num));
    outfile.close();
    ifstream infile("2.dat",ios::binary);
    if(infile.is_open())
    {int n=0;
        infile.read((char*)(&n),sizeof(n));
        cout<<n<<endl;
        infile.close();
    }
    return 0;
};