#include<iostream>
#include<string>
#include<fstream>
#include<cstring>
using namespace std;

int main()
{
    string a;
    cout << "请输入字符串：";
    cin >> a;

    int len = a.size();  // 使用size()代替strlen()
    
    // 写入文件
    ofstream outfile("data.bin", ios::binary);
    if (!outfile) {
        cerr << "无法打开文件进行写入！" << endl;
        return 1;
    }
    outfile.write((char*)&len, sizeof(len));  // 写入字符串的长度
    outfile.write(a.c_str(), len);           // 写入字符串的内容
    outfile.close();                         // 关闭文件

    // 读取文件
    ifstream infile("data.bin", ios::binary);  // 读取相同的文件
    if (!infile) {
        cerr << "无法打开文件进行读取！" << endl;
        return 1;
    }
    // ******
    int length;
    infile.read((char*)&length, sizeof(length));  // 读取字符串的长度
    string temp(length, '\0');                    // 创建一个空字符串并准备读取内容
    infile.read(&temp[0], length);               // 读取字符串内容到temp
    infile.close();                              // 关闭文件

    cout << "读取的字符串为: " << temp << endl;
    
    return 0;
}
