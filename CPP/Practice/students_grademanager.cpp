#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
#include<vector>
using namespace std;
// 问题：
// 如何进行读写，按照一定规则读写，并且是类似这样的按照对象读取

class StudentList{
vector<Student> list;
string s;
public:
    StudentList(string name):s(name){
        ifstream infile(name); 
    
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string name;
        int num;
        double grades;
        if (iss >> name >> num >> grades) {
            list.emplace_back(name, num, grades);
        }
    }    

    }
    friend ostream& operator<<(ostream& os,StudentList& s)
    {
        for(auto x:s.list){cout<<x<<endl;}
        return os;
    }
    void insertOrder(const Student& a)
    {
        for (int i=list.size()-2;i>=0;--i)
        {if(!(list[i]<a))
        {list[i+1]=list[i];}
        else{list[i]=a;}
    }
   
}
 vector<Student> getlist()
    {return list;}
};
class Student{
string name;
int num;
double grades;
public:
    Student(string a=" ",int b=0,double c=100):name(a),num(b),grades(c){}
    friend ostream& operator<<(ostream& os,const Student& s)
    {
        os<<left<<setw(7)<<s.name<<setw(7)<<s.num<<setw(7)<<s.grades<<endl;
    }
    friend bool operator<(const Student& a,const Student& b)
    {return a.grades<b.grades;}
    string getname(){return name;}
    int getnum(){return num;}
    double getgrades(){return grades;}

};

int main() {
    // 从文件恢复学生数据
    StudentList slist("students.txt");
    cout << "当前学生列表：" << endl;
    cout << slist << endl;

    // 读入新学生信息
    cout << "请输入 3 个学生的信息（姓名 学号 成绩）：" << endl;
    for (int i = 0; i < 3; ++i) {
        string name;
        int id;
        double score;
        cin >> name >> id >> score;
        Student s(name, id, score);
        slist.insertOrder(s); // 插入并保持成绩降序
    }
        std::ofstream outfile("students.txt");
    if (!outfile) {
        std::cerr << "无法打开文件用于写入: " <<"students.txt"<< std::endl;
        return;
    }
    for ( auto& stu : slist.getlist()) {
        outfile << stu.getname() << " " << stu.getnum() << " " << stu.getgrades() << std::endl;
    }
    outfile.close();

    // 打印更新后列表
    cout << "更新后学生列表：" << endl;
    cout << slist << endl;

    return 0;
}