#include<iostream>
#include<fstream>

int main(){
std::ofstream outfile("output.txt");
if(!outfile)
{std::cerr<<"can not open file"<<std::endl;
return 1;
}
outfile<<"first line"<<std::endl;
outfile<<"seconde line\n";
outfile<<"third line"<<std::endl;
outfile.close();
std::cout<<"file has been successfully written"<<std::endl;
return 0;
}