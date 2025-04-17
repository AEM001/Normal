#include <iostream>
#include <cstring>
using namespace std;

class Student {
private:
    char* name;
    int numCourses;
    char** courses;
    int* grades;
    
public:
    // 构造函数
    Student(const char* name) : numCourses(0), courses(nullptr), grades(nullptr) {
        // 请在此处补充代码
        this->name=new char[strlen(name)+1];
        strcpy(this->name,name);
    }
    
    // 拷贝构造函数（需要深拷贝）
    Student(const Student& other) {
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
        numCourses = other.numCourses;
        
        // 原始代码
        // courses = new char*[numCourses + 1];
        // 优化：不需要+1，因为grades数组大小也是numCourses
        courses = new char*[numCourses];
        
        for(int i = 0; i < numCourses; ++i) {
            courses[i] = new char[strlen(other.courses[i]) + 1];
            strcpy(courses[i], other.courses[i]);
        }
        
        grades = new int[numCourses];
        for(int i = 0; i < numCourses; ++i) {
            grades[i] = other.grades[i];
        }
    }
    
    // 赋值运算符重载
    Student& operator=(const Student& other) {
        if(this != &other) {
            // 先释放原有资源
            delete[] name;
            delete[] grades;
            
            // 原始代码
            // for(int i = 0; i < numCourses; ++i) { delete courses[i]; }
            // delete[]* courses;  // 错误语法

            //细节，注意检查是否为空指针
            // 修正后的释放方式
            if(courses) {
                for(int i = 0; i < numCourses; ++i) {
                    delete[] courses[i];
                }
                delete[] courses;
            }

            // 深拷贝
            name = new char[strlen(other.name) + 1];
            strcpy(name, other.name);
            numCourses = other.numCourses;
            
            courses = new char*[numCourses];
            for(int i = 0; i < numCourses; ++i) {
                courses[i] = new char[strlen(other.courses[i]) + 1];
                strcpy(courses[i], other.courses[i]);
            }
            
            grades = new int[numCourses];
            for(int i = 0; i < numCourses; ++i) {
                grades[i] = other.grades[i];
            }
        }
        return *this;
    }
    
    // 添加课程
    void addCourse(const char* courseName, int grade) {
        // 处理初始添加课程的情况
        if(numCourses == 0) {
            numCourses = 1;
            courses = new char*[1];
            courses[0] = new char[strlen(courseName) + 1];
            strcpy(courses[0], courseName);
            
            grades = new int[1];
            grades[0] = grade;
            return;
        }

        // 原始代码
        numCourses += 1;
        char **newcourses = new char*[numCourses];
        for(int i = 0; i < numCourses - 1; i++) {
            newcourses[i] = new char[strlen(courses[i]) + 1];
            strcpy(newcourses[i], courses[i]);
            delete[] courses[i];  // 释放旧内存
        }
        newcourses[numCourses - 1] = new char[strlen(courseName) + 1];
        strcpy(newcourses[numCourses - 1], courseName);
        
        int *newgrades = new int[numCourses];
        for(int i = 0; i < numCourses - 1; ++i) {
            newgrades[i] = grades[i];
        }
        newgrades[numCourses - 1] = grade;
        
        delete[] courses;
        delete[] grades;
        
        courses = newcourses;
        grades = newgrades;
    }
    
    // 打印学生信息
    void print() const {
        cout << "Student: " << name << endl;
        for (int i = 0; i < numCourses; ++i) {
            cout << courses[i] << ": " << grades[i] << endl;
        }
    }
    
    // 析构函数
    ~Student() {
        delete[] name;
        delete[] grades;
        for(int i = 0; i < numCourses; ++i) {
            delete[] courses[i];  // 释放每个课程字符串
        }
        delete[] courses;  // 修正：删除指针数组本身
        // 原始错误代码：
        // delete[]* courses;  // 这是错误的语法
    }
};

int main() {
    Student s1("Alice");
    s1.addCourse("Math", 90);
    s1.addCourse("Physics", 85);
    
    // 测试拷贝构造
    Student s2 = s1;
    s2.addCourse("Chemistry", 88);
    
    // 测试赋值运算符
    Student s3("Bob");
    s3 = s1;
    s3.addCourse("Biology", 92);
    
    s1.print();
    s2.print();
    s3.print();
    
    return 0;
}