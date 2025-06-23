//A2：卷积操作
#include<iostream>
#include<cstdlib>

using namespace std;

/*卷积函数cvn声明:
fs:        原始特征信号在内存空间中存放的首地址 
flt:       卷积核存放的首地址 
fm_w:      原始特征信号长度
fm_f:      卷积核长度
fm_s:      卷积步长 
max_index: 卷积后特征信号的最大值下标 
*/
int* cnv(int* fs, int* flt, int fm_w, int fm_f, int fm_s, int& max_index);

int main()
{
	//（10分）添加代码1：完成测试案例所需的数据定义及初始化
	
	//1.1  测试案例所需数据定义
	int filter[4]={-1,2,0,1};
	const int w=20;
	int featuresig[w];

	
	//1.2 利用rand函数，生成原始特征featuresig数据，取值范围：[-5，5]	
	srand(static_cast<unsigned int>(time(nullptr)));
	for(int i=0;i<w;++i)
	{
		featuresig[i]=-5 + rand() % 11; 
	} 

	
	//输出测试案例基础数据：卷积核，特征信号
	cout << "卷积核:" << endl;		  
	for(int i = 0 ; i < 4 ; i++)
		cout << filter[i] << '\t';
	cout << endl;
	cout << "特征信号:" << endl; 
	for(int i = 0 ; i < w ; i++)
	{
		cout << featuresig[i] << '\t';
		if((i+1) % 10 == 0)	cout << endl;
	}	

	int* cnv_fs;							//新特征动态数组首地址
	int index = -1;							//新特征最大特征值下标 
						
	//（3分）添加代码2: 调用卷积函数cnv，实现对案例原始信号featuresig的卷积操作
	// 卷积核为filter，滑动步长为2,获得的新特征存储在动态整型数组cvs_fs中，新特征最大值下标为index 
	cnv_fs=cnv(featuresig,filter,20,4,2,index);
	

	//输出卷积后的特征值 
	cout << "卷积后的特征值:" << endl;
	for(int i = 0 ; i < (w - 4) / 2 + 1 ; i++)
	{
		cout << cnv_fs[i] << '\t';
		if((i+1) % 10 == 0)	cout << endl;
	}
	cout << endl;
	//输出最大卷积特征值下标 
	cout << "最大卷积特征值下标:" << index << endl;

	//（2分）添加代码3: 完成必要的善后工作 
	delete[]cnv_fs;

	return 0;
}

int* cnv(int* fs, int* flt, int fm_w, int fm_f, int fm_s, int& max_index)
{
	int* new_fs;						// 存储卷积后的新特征动态数组的首地址 
	//（10分）添加代码4: 实现卷积操作，查找最大卷积特征值下标，返回存储卷积后新特征动态数组首地址
	int wl=1+(fm_w-fm_f)/fm_s;
	new_fs=new int[wl];
	    // 初始化并进行卷积操作
    for(int i = 0; i < wl; ++i) {
        new_fs[i] = 0;  // 初始化为0
		}
	// int* fsfs=new int[wl];
	for(int i=0;i<wl;++i)
	{
	for(int j=2*i;j<2*i+fm_f;++j)
	{new_fs[i]+=fs[j]*flt[j-2*i];}
	}
	int maxloc=0;
	for(int i=0;i<wl;++i)
	{if(new_fs[i]>new_fs[maxloc])
		{maxloc=i;}
	}
	max_index=maxloc;
	return new_fs;						//返回存放特征值动态数组的首地址 	
}
