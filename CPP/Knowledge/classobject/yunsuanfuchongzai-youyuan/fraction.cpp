#include "fraction.h"
#include <cstdlib> // For abs

fraction::fraction(int a, int b) : fz(a), fm(b) {
    if (fm == 0) {
        cout << "Error: Denominator cannot be zero." << endl;
        fz = 0;
        fm = 1;
    } else {
        simplify();
    }
}

fraction::fraction() : fz(0), fm(1) {}

istream& operator>>(istream &in, fraction &f) {
    in >> f.fz >> f.fm;
    if (f.fm == 0) {
        cout << "Error: Denominator cannot be zero." << endl;
        f.fm = 1;
    }
    f.simplify();
    return in;
}

ostream& operator<<(ostream &out, const fraction &f) {
    if (f.fm == 1) {
        out << f.fz;
    } else if (f.fz == 0) {
        out << 0;
    } else {
        out << f.fz << "/" << f.fm;
    }
    return out;
}

fraction operator+(fraction a, fraction b) {
    fraction temp;
    temp.fz = a.fz * b.fm + b.fz * a.fm;
    temp.fm = a.fm * b.fm;
    temp.simplify();
    return temp;
}

fraction operator+(fraction a, int b) {
    fraction temp;
    temp.fz = a.fz + b * a.fm;
    temp.fm = a.fm;
    temp.simplify();
    return temp;
}

fraction fraction::operator++() {
    fz++;
    simplify();
    return *this;
}

fraction fraction::operator++(int) {
    fraction temp = *this;
    fz++;
    simplify();
    return temp;
}

fraction operator/(fraction a, fraction b) {
    if (b.fz == 0) {
        cout << "Error: Cannot divide by zero." << endl;
        return fraction(0, 1);
    }
    fraction temp;
    temp.fz = a.fz * b.fm;
    temp.fm = a.fm * b.fz;
    temp.simplify();
    return temp;
}

fraction::operator double() const {
    return (double)fz / fm;
}

int fraction::getmax(int a, int b) {
    while (b != 0) {
        int k = a % b;
        a = b;
        b = k;
    }
    return a;
}

void fraction::set(int a, int b) {
    if (b == 0) {
        cout << "Error: Denominator cannot be zero." << endl;
    } else {
        fz = a;
        fm = b;
        simplify();
    }
}

void fraction::simplify() {
    if (fm == 0) {
        cout << "Error: Denominator cannot be zero." << endl;
        return;
    }
    int k = getmax(abs(fz), abs(fm));
    fz /= k;
    fm /= k;
    if (fm < 0) {
        fz = -fz;
        fm = -fm;
    }
}