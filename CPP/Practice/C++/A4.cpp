//应用面向对象的派生和多态特性，根据收入的分布规律进行税率及起征点分析

#include<iostream>
#include<cmath>
using namespace std;

//========= SalaryData 基类定义 ===========
class SalaryData {
protected:				  // A 行	
	string dataName;      // 样本数据名称
	double* dataSet;      // 样本收入数据向量
	int subscript;	      // 数据已用最大下标值，即从0开始的有效数据位置
	int maxSize;	      // 最大可存储数据个数

	double mean;          // 样本数据平均值（平均收入）
	double medianSalary;  // 样本数据中位数（中位数收入）
	double sum;			  // 样本数据总和（总收入）	

	double taxRate;       // 税率（若10% 则为0.1）
	double taxSalary;     // 税率起点收入

public:
	SalaryData(string name = "", int maxs = 150);   //构造函数
	SalaryData(const SalaryData& vecs);				//拷贝（复制）构造函数
	virtual ~SalaryData();							//析构函数
	void SetData(double dataarray[], int len);	    //从外部数组初始化数据成员
	void Sort();									//数据排序
	void CalMeanMedianSalary();						//计算数据平均值 和 中位数
	virtual void CalTaxRate();						//确定税率和起点收入的虚函数
	void Print()									//输出样本数据、量化分析、税率信息
	{
		cout << " 收入样本数据：" << dataName << endl;
		for (int i = 0; i <= subscript; i++)
		{
			cout << dataSet[i] << "  ";
		}
		cout << endl;
		cout << "样本均值：" << mean << "  样本中位数：" << medianSalary << endl;
		cout << "税率：" << taxRate << "  起征点： " << taxSalary << endl;
		cout << "------------------------------\n";
	}
};

SalaryData::SalaryData(string name, int maxs)
{
	dataName = name;
	maxSize = maxs;
	dataSet = new double[maxSize];
	subscript = -1;

	sum = mean = medianSalary = 0;
	taxRate = 0;
	taxSalary = 0;
}

SalaryData::SalaryData(const SalaryData& vecs)
{
	dataName = vecs.dataName;
	maxSize = vecs.maxSize;
	dataSet = new double[maxSize];
	subscript = vecs.subscript;

	for (int i = 0; i <= subscript; i++)
	{
		dataSet[i] = vecs.dataSet[i];
	}

	mean = vecs.mean;
	sum = vecs.sum;
	medianSalary = vecs.medianSalary;
	taxRate = vecs.taxRate;
	taxSalary = vecs.taxSalary;
}

//计算平均值 和 中位数数据
void SalaryData::CalMeanMedianSalary()
{
	if (subscript == -1)
		return;
	
	//统计均值mean
	sum = 0;
	for (int i = 0; i <= subscript; i++)
	{
		sum += dataSet[i];
	}
	mean = sum / (subscript + 1);

	//（6分）补充程序1：实现求 中位数 功能
	Sort();
if ((subscript + 1) % 2 == 1)  // 奇数个元素
{
    medianSalary = dataSet[subscript / 2];
}
else {  // 偶数个元素
    medianSalary = (dataSet[subscript / 2] + dataSet[subscript / 2 + 1]) / 2;
}


}

void SalaryData::Sort()
{
	int i, j;
	double temp;
	for (i = 0; i < subscript; i++)
	{
		for (j = i + 1; j < subscript + 1; j++)
		{
			if (dataSet[i] < dataSet[j])
			{
				temp = dataSet[i];
				dataSet[i] = dataSet[j];
				dataSet[j] = temp;
			}
		}
	}
}

SalaryData::~SalaryData()
{
	delete[]dataSet;
}

//从外部数组初始化成员变量数据，并进行量化分析及税率计算
void SalaryData::SetData(double dataArray[], int len)
{
	if (maxSize < len)
	{
		delete[]dataSet;

		maxSize = len;
		dataSet = new double[maxSize];
	}
	subscript = len - 1;

	for (int i = 0; i < len; i++)
	{
		dataSet[i] = dataArray[i];
	}
	//统计分析数据均值、中位数,设置税率及其起征点
	CalMeanMedianSalary();
	CalTaxRate();
}

//设置税率起点为中位数，税率为5%
void SalaryData::CalTaxRate()
{
	CalMeanMedianSalary();
	taxSalary = medianSalary;
	taxRate = 0.05;
}


//============= NewTaxModel类定义 =============
class NewTaxModel : public SalaryData
{
public:
	NewTaxModel(string name = "", int maxs = 150) : SalaryData(name, maxs) { };
	~NewTaxModel() {};
	void CalTaxRate();
};

void NewTaxModel::CalTaxRate()
{
	// （6分）补充程序2： 实现 根据二八定律检验，重新计算税率 和 税率起点
	int k=0.2*(subscript+1);
	int sum20=0;
	for(int i=0;i<k;++i)
	{sum20+=dataSet[i];}
	if(sum20>=0.8*sum)
	{taxRate=0.4;
	taxSalary=dataSet[k-1];}



}


// (6分)补充程序3：设计并实现全局函数Calfun
// 原型：void Calfun(参数类型  City, double arr[], int len)
// Calfun功能:根据一组长度为len的外部数据arr，更新类对象City数据，
//            进行量化分析及税率计算,输出结果
void Calfun(SalaryData City,double arr[],int len)
{City.SetData(arr,len);
City.Print();}




int main()
{
	SalaryData  cityA("Jing Jiang");
	NewTaxModel cityB("Jing Hai");

	//两组收入数据
	double dataA[10] = { 5690, 6154, 5170, 6173, 7160, 4181, 3157, 2172, 6171, 5163 };
	double dataB[10] = { 56910, 5170, 6173, 4160, 3181, 1157, 6172, 3171, 75410, 1163 };

	//（4分）补充程序4：调用Calfun函数，给出对cityA 和 cityB 量化分析结果及税率建议.其中：
	//                  cityA使用数组dataA初始化，cityB使用数组dataB初始化
	Calfun(cityA,dataA,10);
	Calfun(cityB,dataB,10);
	
	return 0;
}


/**************** 问答题 ******************
问题(3分)在本题的类框架设计中，将基类SalaryData的A行代码“protected：”删除，将会产生什么问题？简述解决方案。
答：







**********************************************/