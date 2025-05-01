#include<fstream>
#include<iostream>
std::ofstream outfile;

int main() {
	outfile.open("output.txt", std::ios::out);
    // 也可以使用构造函数打开
    if(outfile.is_open())
    {outfile<<"Hello,world"<<std::endl;
     outfile<<"second line"<<std::endl;
    
    outfile.close();
    std::cout << "数据已成功写入"<<std::endl;
	}
    else{
        std::cerr << "无法打开文件进行写入！" << std::endl;
        return 1; // 返回非零表示出错
    }
    return 0;
}
