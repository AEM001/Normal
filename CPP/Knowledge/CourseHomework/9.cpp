#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <stdexcept>
using namespace std;

template<typename T>
class Seqlist {
    T *list;
    int maxsize;
    int last;
    string filename;
public:
    Seqlist(int num = 0, const string &name = "")
        : maxsize(num > 0 ? num : 100), last(-1), list(nullptr), filename(name) {
        list = new T[maxsize];
        if (filename.empty()) return;
        ifstream infile(filename, ios::binary);
        if (!infile) {
            cout << "空表" << endl;
            return;
        }
        infile.seekg(0, ios::end);
        streamoff bytes = infile.tellg();
        if (bytes <= 0 || bytes % sizeof(T) != 0) {
            cout << "空表" << endl;
            infile.close();
            return;
        }
        int cnt = static_cast<int>(bytes / sizeof(T));
        infile.seekg(0, ios::beg);
        if (cnt > maxsize) {
            delete[] list;
            maxsize = cnt;
            list = new T[maxsize];
        }
        infile.read(reinterpret_cast<char*>(list), static_cast<size_t>(cnt) * sizeof(T));
        last = cnt - 1;
        infile.close();
    }

    ~Seqlist() {
        if (!filename.empty()) {
            ofstream outfile(filename, ios::binary | ios::trunc);
            if (outfile) {
                outfile.write(reinterpret_cast<const char*>(list), sizeof(T) * (last + 1));
                outfile.close();
            }
        }
        delete[] list;
    }

    void enlarge(int newmaxsize) {
        if (newmaxsize <= maxsize) return;
        T *newlist = new T[newmaxsize];
        for (int i = 0; i <= last; ++i) {
            newlist[i] = list[i];
        }
        delete[] list;
        list = newlist;
        maxsize = newmaxsize;
    }

    void insertOrder(const T &a) {
        if (last + 1 >= maxsize) {
            enlarge(maxsize + 100);
        }
        int i = last;
        while (i >= 0 && a < list[i]) {
            list[i + 1] = list[i];
            --i;
        }
        list[i + 1] = a;
        ++last;
    }

    const T& operator[](int idx) const {
        if (idx < 0 || idx > last) throw out_of_range("索引越界");
        return list[idx];
    }

    friend ostream& operator<<(ostream &os, const Seqlist<T> &s) {
        for (int i = 0; i <= s.last; ++i) {
            os << s.list[i] << ' ';
        }
        return os;
    }
};

class Complex {
    double re;
    double im;
public:
    Complex(double r = 0, double i = 0) : re(r), im(i) {}
    friend bool operator<(const Complex &a, const Complex &b) {
        return (a.re * a.re + a.im * a.im) < (b.re * b.re + b.im * b.im);
    }
    friend istream& operator>>(istream &is, Complex &c) {
        is >> c.re >> c.im;
        return is;
    }
    friend ostream& operator<<(ostream &os, const Complex &c) {
        os << "(" << c.re << "," << c.im << ")";
        return os;
    }
};

int main() {
    Seqlist<Complex> clist(5, "CData.txt");
    cout << clist << endl;
    double re, im;
    cout << "请输入 6 个复数的实部虚部：";
    for (int i = 0; i < 6; ++i) {
        cin >> re >> im;
        Complex c(re, im);
        clist.insertOrder(c);
    }
    cout << clist << endl;
    return 0;
}
