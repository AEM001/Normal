#include<iostream>
#include<fstream>
using namespace std;
class Complex {
	double re, im;
public:
	Complex() { re = 0; im = 0; }
	Complex(double a, double b) { re = a; im = b; }
	friend istream& operator>>(istream& is, Complex& c) {
		is >> c.re >> c.im;
		return is;
	}
	friend ostream& operator<<(ostream& os, const Complex& c) {
		os << c.re;
		if (c.im < 0) {
			os << c.im << 'i';
		}
		else if (c.im > 0) {
			os << '+' << c.im << 'i';
		}
		return os;
	}
	bool operator>(Complex c) {
		return (re * re + im * im) > (c.re * c.re + c.im * c.im);
	}
	Complex& operator=(const Complex& c) {
		im = c.im;
		re = c.re;
		return *this;
	}
	double getre() {
		return this->re;
	}
	double getim() {
		return this->im;
	}
};
template<typename T>
class Seqlist {
	T* list;
	int maxsize;
	int last;
public:
	Seqlist() {
		maxsize = 6;
		last = -1;
		list = new T[maxsize]; 
	}
	void renew() {
		T* newList = new T[maxsize + 1];
		for (int i = 0; i <= last; i++) {
			newList[i] = list[i];
		}
		delete[] list;
		list = newList;
		maxsize++;
	}
	Seqlist(int m, const string name) {
		maxsize = m;
		last = -1;  // ��ȷ��ʼ��
		list = new T[maxsize];
		ifstream infile(name);
		T temp;
		while (infile >> temp) { // ���ڶ�ȡ�ɹ�ʱ����
			if (last >= maxsize - 1) renew();
			list[++last] = temp;
		}
		infile.close();
	}
	~Seqlist() {
		if (list) {
			string fname = "CData.txt";
			ofstream outfile(fname);
			for (int i = 0; i <= last; i++)
			{
				outfile << list[i].getre() << ' ' << list[i].getim() << endl;
			}
			outfile.close();
			
		}
		delete[]list;
	}
	
	void insertOrder(const T& c) {
		if (last == -1) {
			list[0] = c;
			last = 0;
			return;
		}
		for (int i = 0; i <= last; i++) {
			if (list[i] > c) {
				if (last >= maxsize - 1) renew();
				for (int j = last + 1; j > i; j--) {
					list[j] = list[j - 1];
				}
				list[i] = c;
				last++;
				return;
			}
		}
		if (last >= maxsize - 1) renew();
		list[++last] = c;
	}
	//friend istream& operator>>(istream& is, Seqlist clist) {}
	friend ostream& operator<<(ostream& os, const Seqlist<T>& clist) {
		if (!clist.list) {
			os << "�ñ�Ϊ��" << endl;
		}
		else {
			for (int i = 0; i <= clist.last; i++)
			{
				os << *(clist.list + i) << ' ';
			}
		}
		return os;
	}
};

int main() {
	Seqlist<Complex> clist(5, "CData.txt");
	cout << clist;
	cout << endl;
	double re, im;
	cout << "������6��������ʵ���鲿��" << endl;
	for (int i = 1; i <= 6; i++)
	{
		cin >> re >> im;
		Complex c(re, im);
		clist.insertOrder(c);
	}
	cout << clist;
	cout << endl;
	return 0;
}