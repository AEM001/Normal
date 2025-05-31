// #include<iostream>
// #include<string>
// #include<ostream>
// using namespace std;

// class Student{
//     char name[21];
//     int id;
//     double score;
// public:
//     //学生的构造函数
//     Student(char* n,int i,double s):name(n),id(i),score(s){}
//     friend ostream& operator<<(ostream& os,Student& stu)
//     {
//         os<<stu.name<<'('<<stu.id<<')'<<':'<<stu.score<<endl;

//     }
//     double getgrades(){return score;}
// };
// class StudentList{
//    Student* s;
//    int size=3;//读入进来的大小不确定呀
//    int last=-1;
//    string fname;
//     //构造函数
// public:
//     StudentList (string f):fname(f){
//     ifstream infile(f,ios::binary);

//     }
//     friend ostream& operator<<(ostream& os,StudentList& slist)
//     {
//         for(int i=0;i<slist.size;++i)
//         {
//             cout<<slist[i]<<endl;
//             return os;
//         }
//     }
//     Student& operator[](int i)
//     {
//         return s[i];
//     }
//     void insertOrder(Student& stu)
//     {
//         if(last==-1){s[++last]=stu;}
//         else{
//             for(int i=last;i>=0;--i)
//             {
//                 if(s[i].getgrades()<stu.getgrades())
//                 {s[i+1]=s[i];}
//                 else{s[i+1]=stu;
//                 break;}
//             }
//         }
//     }


// };
// int main() {
//     // 从文件恢复学生数据
//     StudentList slist("students.dat");
//     cout << "当前学生列表：" << endl;
//     cout << slist << endl;

//     // 读入新学生信息
//     cout << "请输入 3 个学生的信息（姓名 学号 成绩）：" << endl;
//     for (int i = 0; i < 3; ++i) {
//         string name;
//         int id;
//         double score;
//         cin >> name >> id >> score;
//         Student s(name, id, score);
//         slist.insertOrder(s); // 插入并保持成绩降序
//     }

//     // 打印更新后列表
//     cout << "更新后学生列表：" << endl;
//     cout << slist << endl;

//     return 0;
// }


#include<iostream> 
#include<string> 
#include<ostream>
#include<fstream>  // 【修改1】添加文件流头文件
#include<cstring>  // 【修改2】添加字符串操作头文件
using namespace std;  

class Student{     
    char name[21];     
    int id;     
    double score; 
public:     
    // 【修改3】添加默认构造函数，用于数组初始化
    Student() : id(0), score(0.0) {
        name[0] = '\0';
    }
    
    // 【修改4】修改构造函数参数类型，支持string转换为char数组
    Student(const string& n, int i, double s) : id(i), score(s) {
        strncpy(name, n.c_str(), 20);
        name[20] = '\0';  // 确保字符串结束
    }
    
    // 【修改5】修改友元函数参数为const引用，添加return语句
    friend ostream& operator<<(ostream& os, const Student& stu)     
    {         
        os << stu.name << '(' << stu.id << ')' << ':' << stu.score;
        return os;  // 添加return语句
    }     
    
    double getgrades() const { return score; }  // 【修改6】添加const修饰符
}; 

class StudentList{    
    Student* s;    
    int capacity;  // 【修改7】重命名size为capacity，表示容量
    int count;     // 【修改8】用count表示实际学生数量，替代last
    string fname;     

public:     
    StudentList(string f) : fname(f), capacity(10), count(0) {  // 【修改9】初始化成员变量
        s = new Student[capacity];  // 【修改10】动态分配内存
        
        // 【修改11】完善文件读取逻辑
        ifstream infile(f, ios::binary);      
        if (infile.is_open()) {
            Student temp;
            while (infile.read(reinterpret_cast<char*>(&temp), sizeof(Student))) {
                if (count >= capacity) {
                    // 扩容
                    capacity *= 2;
                    Student* newS = new Student[capacity];
                    for (int i = 0; i < count; ++i) {
                        newS[i] = s[i];
                    }
                    delete[] s;
                    s = newS;
                }
                s[count++] = temp;
            }
            infile.close();
        }
    }
    
    // 【修改12】添加析构函数，保存文件并释放内存
    ~StudentList() {
        ofstream outfile(fname, ios::binary);
        if (outfile.is_open()) {
            for (int i = 0; i < count; ++i) {
                outfile.write(reinterpret_cast<const char*>(&s[i]), sizeof(Student));
            }
            outfile.close();
        }
        delete[] s;
    }
         
    // 【修改13】修复输出运算符重载的逻辑错误
    friend ostream& operator<<(ostream& os, const StudentList& slist)     
    {         
        if (slist.count == 0) {
            os << "暂无学生信息";
            return os;
        }
        for (int i = 0; i < slist.count; ++i)         
        {             
            os << slist.s[i] << endl;  // 修改：使用slist.s[i]而不是slist[i]
        }
        return os;
    }     
    
    // 【修改14】修改下标运算符为const版本
    const Student& operator[](int i) const     
    {         
        return s[i];     
    }     
    
    // 【修改15】完善insertOrder函数，修复插入逻辑
    void insertOrder(const Student& stu)     
    {         
        // 检查是否需要扩容
        if (count >= capacity) {
            capacity *= 2;
            Student* newS = new Student[capacity];
            for (int i = 0; i < count; ++i) {
                newS[i] = s[i];
            }
            delete[] s;
            s = newS;
        }
        
        // 找到插入位置（降序排列）
        int pos = count;
        for (int i = 0; i < count; ++i) {
            if (s[i].getgrades() < stu.getgrades()) {
                pos = i;
                break;
            }
        }
        
        // 移动元素为新学生腾出位置
        for (int i = count; i > pos; --i) {
            s[i] = s[i-1];
        }
        
        // 插入新学生
        s[pos] = stu;
        count++;
    }   
}; 

int main() {     
    // 从文件恢复学生数据     
    StudentList slist("students.dat");     
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

    // 打印更新后列表     
    cout << "更新后学生列表：" << endl;     
    cout << slist << endl;      

    return 0; 
}