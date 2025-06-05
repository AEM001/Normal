/*
=== 二进制字符串读写练习题 ===

题目：学生信息管理系统
要求：创建一个简单的学生信息管理系统，能够将学生信息以二进制格式保存到文件并读取。

学生信息包括：
- 学号 (int)
- 姓名 (string) 
- 专业 (string)
- 成绩 (double)

练习任务：
1. 实现 saveStudent() 函数，将一个学生信息保存到二进制文件
2. 实现 loadStudent() 函数，从二进制文件读取学生信息
3. 实现 saveMultipleStudents() 函数，保存多个学生信息
4. 实现 loadMultipleStudents() 函数，读取多个学生信息

提示：
- 字符串需要先写入长度，再写入内容
- 多个学生信息时，先写入学生数量，再写入每个学生的信息
- 记得检查文件操作是否成功
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct Student {
    int id;
    string name;
    string major;
    double score;
    
    // 构造函数
    Student(int id = 0, string name = "", string major = "", double score = 0.0)
        : id(id), name(name), major(major), score(score) {}
    
    // 打印学生信息
    void print() const {
        cout << "学号: " << id << ", 姓名: " << name 
             << ", 专业: " << major << ", 成绩: " << score << endl;
    }
};

// TODO: 实现以下函数

// 任务1: 保存单个学生信息到二进制文件
bool saveStudent(const string& filename, const Student& student) {
    // 在这里实现你的代码
    // 提示：打开文件 -> 写入学号 -> 写入姓名(长度+内容) -> 写入专业(长度+内容) -> 写入成绩
    
    return false; // 临时返回，请修改
}

// 任务2: 从二进制文件读取单个学生信息
bool loadStudent(const string& filename, Student& student) {
    // 在这里实现你的代码
    // 提示：打开文件 -> 读取学号 -> 读取姓名(长度+内容) -> 读取专业(长度+内容) -> 读取成绩
    
    return false; // 临时返回，请修改
}

// 任务3: 保存多个学生信息
bool saveMultipleStudents(const string& filename, const vector<Student>& students) {
    // 在这里实现你的代码
    // 提示：先写入学生数量，然后循环写入每个学生的信息
    
    return false; // 临时返回，请修改
}

// 任务4: 读取多个学生信息
bool loadMultipleStudents(const string& filename, vector<Student>& students) {
    // 在这里实现你的代码
    // 提示：先读取学生数量，然后循环读取每个学生的信息
    
    return false; // 临时返回，请修改
}

// 测试函数
int main() {
    cout << "=== 二进制字符串读写练习 ===" << endl;
    
    // 测试数据
    Student student1(1001, "张三", "计算机科学", 85.5);
    Student student2(1002, "李四", "软件工程", 92.0);
    Student student3(1003, "王五", "数据科学", 78.5);
    
    vector<Student> students = {student1, student2, student3};
    
    cout << "\n--- 测试1: 单个学生信息 ---" << endl;
    cout << "原始数据: ";
    student1.print();
    
    // 保存单个学生
    if (saveStudent("student.dat", student1)) {
        cout << "保存成功!" << endl;
        
        // 读取单个学生
        Student loadedStudent;
        if (loadStudent("student.dat", loadedStudent)) {
            cout << "读取成功: ";
            loadedStudent.print();
        } else {
            cout << "读取失败!" << endl;
        }
    } else {
        cout << "保存失败!" << endl;
    }
    
    cout << "\n--- 测试2: 多个学生信息 ---" << endl;
    cout << "原始数据:" << endl;
    for (const auto& s : students) {
        s.print();
    }
    
    // 保存多个学生
    if (saveMultipleStudents("students.dat", students)) {
        cout << "保存成功!" << endl;
        
        // 读取多个学生
        vector<Student> loadedStudents;
        if (loadMultipleStudents("students.dat", loadedStudents)) {
            cout << "读取成功，共 " << loadedStudents.size() << " 个学生:" << endl;
            for (const auto& s : loadedStudents) {
                s.print();
            }
        } else {
            cout << "读取失败!" << endl;
        }
    } else {
        cout << "保存失败!" << endl;
    }
    
    return 0;
}

/*
参考答案提示：

saveStudent函数示例：
1. 打开文件 ofstream file(filename, ios::binary)
2. 写入学号 file.write((char*)&student.id, sizeof(student.id))
3. 写入姓名：先写长度，再写内容
4. 写入专业：先写长度，再写内容  
5. 写入成绩 file.write((char*)&student.score, sizeof(student.score))

loadStudent函数示例：
1. 打开文件 ifstream file(filename, ios::binary)
2. 读取学号 file.read((char*)&student.id, sizeof(student.id))
3. 读取姓名：先读长度，resize字符串，再读内容
4. 读取专业：先读长度，resize字符串，再读内容
5. 读取成绩 file.read((char*)&student.score, sizeof(student.score))

完成后可以运行测试，看看输出是否正确！
*/