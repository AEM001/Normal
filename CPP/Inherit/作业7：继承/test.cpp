//根据题目描述，以及以下提示和要求，直接在本文件中添加代码。将文件名修改为本人学号后提交。
//定义本科生类
#include<iostream>
#include<string>
using namespace std;
class UStudent
{
	//添加数据成员
	string stuId;
	string stuName;
    double credits;
    bool result;


public:
	
	//添加构造及接口函数
    UStudent(){};
    UStudent(string si,string st,double cred):stuId(si),stuName(st),credits(cred),result(false){};
    void check(){
        result=(credits>=160);//学籍审核
    }
    void print(){
        cout<<"本科生"<<' '<<stuId<<' '<<stuName<<' '<<credits<<' '<<(result?"可以毕业":"不能毕业")<<endl;
    }
    void setresult(bool res){result=res;}
    string getid(){return stuId;}
    string getname(){return stuName;}
    bool getresult(){return result;}
    double getcredits(){return credits;}      
	                               
}; 

//由本科生公有派生研究生类
class GStudent : public UStudent
{
	//按需添加数据成员
    bool teaching,research;

public:   
    GStudent(){};  // 已有默认构造函数
    GStudent(string si, string st, double cred, bool teach, bool res)
        : UStudent(si, st, cred), teaching(teach), research(res) {}
	//按需添加成员函数
void check()
{setresult(getcredits()>=36&&teaching&&research);}
void print()
{cout<<"研究生"<<' '<<getid()<<' '<<getname()<<' '<<getcredits()<<' '<<(getresult()?"可以毕业":"不能毕业")<<endl;}

	                       
};


int main()
{
	UStudent ustu[5];                 //本科生数组
	GStudent gstu[4];                 //研究生数组
	
	cout << "读入5名本科生学号、姓名及已修学分，并进行审核：" << endl;
	//添加代码
for(int i=0;i<5;i++){
    string si,st;double cred;
    cin>>si>>st>>cred;
    ustu[i]=UStudent(si,st,cred);}
	
	
	
	cout << "读入4名研究生学号、姓名已修学分、教学、科研考核结果，并进行审核：" << endl;
	//添加代码
    for(int i=0;i<4;i++){
        string si,st;double cred;bool teach,research;
        cin>>si>>st>>cred>>teach>>research;
        gstu[i]=GStudent(si,st,cred,teach,research);}
	
	//添加代码输出本科生和研究生的信息及学籍审核结果，每个学生信息占一行
    for(int i=0;i<5;i++){ustu[i].check();
        ustu[i].print();}
    for(int i=0;i<4;i++){gstu[i].check();
        gstu[i].print();}

	return 0;
}