#include<iostream>
#include "seqlist.h"
#include "Complex.h"
using namespace std;

int main()
{
	// //��1��ʵ����Ϊʵ������
	// SeqList<double, 10> dArr;            //����ձ�

	// cout << "dArr:  " << dArr << endl;    //��������dArr:  �ձ�	

	// cout << endl;

	// cout << "...�������ݱ�..." << endl;
	// double x;
	// cout << "����5��ʵ����";
	// for (int i = 0; i < 5; i++)          //��5��ʵ���ֱ𰴽���Ͷ���˳��������
	// {
	// 	cin >> x;                        //����һ��ʵ��
	// 	dArr.insert(x, i);               //ʵ���������±�i��
	// }

	// cout << "...�������..." << endl;
	// cout << "dArr��" << dArr << endl;

	// cout << endl;

	// cout << "...���Һ�ɾ��Ԫ��..." << endl;
	// double dkey;                         //����������
	// cout << "���������ʵ����";
	// cin >> dkey;

	// if (dArr.del(dkey) == false)
	// 	cout << "�����޴�Ԫ�أ�" << endl;
	// cout << dArr << endl;

	// cout << endl << endl;
	

	//��2��ʵ����Ϊ��������
	SeqList<Complex, 10> cDesArr;          //���콵��ձ�

	cout << "cDesArr:  " << cDesArr << endl;     //��������dDesArr:  �ձ�

	cout << endl;

	cout << "...���������..." << endl; 
	Complex c;
	cout << "����5��ʵ����";
	for (int i = 0; i < 5; i++)          //��5�������ֱ𰴽���Ͷ���˳��������
	{
		cin >> c;                        //����һ������
		cDesArr.insert(c);               //������������뵽������
	}

	cout << "...��������..." << endl;
	cout << "cDesArr��" << cDesArr << endl;

	cout << endl;

	cout << "...����Ԫ�ز��޸�..." << endl;
	Complex ckey;                        //����������
	cout << "��������Ҹ���";
	cin >> ckey;
	int pos;

	pos = cDesArr.biSearch(ckey);       //���ң������±�
	if (pos == -1) cout << "�����޴�Ԫ�أ�" << endl;
	else
	{
		cout << "����һ��������Ϊ�޸ĸ�Ԫ�ص�ֵ��";
		cin >> cDesArr[pos];
		cout << "��������" << endl;
		cDesArr.sort();
		cout << cDesArr << endl;
	}

	system("pause");
	return 0;
}