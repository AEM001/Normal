#include <iostream>
#include <cassert>

using namespace std;

class Score
{
private:
    int math;
    int physical;
public:
    Score(int math = 0, int physical = 0) 
    {
        this->math = math;
        this->physical = physical;
    }
    
    void setMath(int m)  {  math = m;  }
    void setPhy(int p)  { physical = p;}

    //  完善代码1： 实现访问接口（5分）

    
    //  完善代码2： 实现比较运算符重载,可满足排序算法需求（5分）
    //              比较原则：先用math进行比较，如果math相等，则用physical进行比较


};

class ScoreManagement
{
    static const int MAXSIZE = 10;  // 数组默认容量
    int size;                       // 实际学生成绩个数
    Score score[MAXSIZE];           // 存储学生成绩数组

public:
    ScoreManagement() : size(0)
    {        
        for (int i = 0; i < MAXSIZE; ++i)
            score[i] = {};
    }
    ScoreManagement(const Score stuarray[], int count) 
    {
        // 利用首地址为stuarray，长度为count数据，初始化学生成绩
        size = std::min(MAXSIZE, count);
        for (int i = 0; i < size; i++)
        {
            score[i] = stuarray[i];
        }
    }
    int getsize() const { return size; }
       
    Score& operator[](int i)
    {
        if (i < 0 || i >= size)     // 下标i有效性检查
        {
            std::cout << "下标越界\n";
            exit(1);
        }
        return score[i];
    }

    // 完善代码3: 实现必要的下标运算符[]重载(4分)

    
    void sort()
    {
        // 完善代码4： 按降序实现学生成绩排序，算法任选（10分）
        
    }
};

// 输出一个学生成绩
ostream& operator<<(ostream& os, const Score& s)
{
    os << "Math:" << s.getMath() << '\t' << "Physics:" << s.getPhy();
    return os;
}

// 输出一组学生成绩
void output(const ScoreManagement& sm)
{
    for (int i = 0; i < sm.getsize(); i++)
    {
        cout << sm[i] << endl;
    }
}

int main()
{
    Score sarray[5] = { Score(90, 90), Score(90, 88), Score(80, 85), Score(85, 80), Score(92, 89) };

    // 完善代码5：使用sarray数组数据，定义ScoreManagement对象sm(2分)
    
   
    sm[2] = Score(85, 90);      // 修改下标为2的学生成绩
    cout << "sm[2]: " << sm[2] << endl;

    // 完善代码6：调用sort，将学生成绩按降序排序(2分)
    

    cout << "\nThe sorted student scores are as follows:\n";
    // 完善代码7：调用output，输出排序后所有学生成绩(2分)
    
 
   return 0;
}
