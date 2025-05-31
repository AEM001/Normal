#include <iostream>
#include <cstring>
#include <stdexcept>
using namespace std;

class Student {
private:
    char* name;
    int* scores;  // 存储三门课成绩
public:
    // 构造函数
    Student(const char* n = "", const int sc[3] = nullptr) {
        if (n == nullptr || strlen(n) == 0) {
            throw invalid_argument("Name cannot be empty");
        }
        name = new char[strlen(n) + 1];
        strcpy(name, n);
        scores = new int[3];
        if (sc != nullptr) {
            for (int i = 0; i < 3; ++i) scores[i] = sc[i];
        }
    }

    // 深拷贝构造函数
    Student(const Student& other) {
        // 请在此处补充代码
        int num=strlen(other.name);
        name=new char[num+1];
        strcpy(name,other.name);
        scores=new int[3];
        for(int i=0;i<3;++i)
        {
            scores[i]=other.scores[i];
        }
    }

    // 深拷贝赋值运算符
    Student& operator=(const Student& other) {
        if(this == &other) {
            return *this;
        }
        delete[] name;
        delete[] scores;
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
        scores = new int[3];  // 修正为固定3个元素
        for(int i = 0; i < 3; ++i) {
            scores[i] = other.scores[i];
        }
        return *this;
    }

    ~Student() {
        // 请在此处补充代码
        delete name;
        delete[]scores;
    }

    void Init(const char* n, const int sc[3]) {
        if (n == nullptr || strlen(n) == 0) {
            throw invalid_argument("Name cannot be empty");//注意不能为空
        }
        delete[] name;
        delete[] scores;
        name = new char[strlen(n) + 1];  // 修正为+1
        strcpy(name, n);
        scores = new int[3];
        for(int i = 0; i < 3; ++i) {
            scores[i] = sc[i];
        }
    }

    void Display() const {
        // 请在此处补充代码
        cout << name << ": ";  // 添加冒号和空格
        for(int i = 0; i < 3; ++i) {
            cout << scores[i];
            if (i < 2) cout << "/";  // 只在成绩间加斜线
        }
        cout << endl;
    }
};

// 测试用例
int main() {
    int scores1[3] = {90, 85, 78};
    Student s1;
    s1.Init("Alice", scores1);

    Student s2 = s1;  // 测试拷贝构造
    Student s3;
    s3 = s1;          // 测试赋值运算符

    s1.Display();
    s2.Display();
    s3.Display();

    // 测试异常
    try {
        Student s4;
        s4.Init("", scores1);
    } catch(const invalid_argument& e) {
        cout << "Error: " << e.what() << endl;
    }
    return 0;
}