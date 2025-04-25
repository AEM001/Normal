class complex {
private:
    double real;
    double imag;
public:
    complex(double r = 0, double i = 0) : real(r), imag(i) {}
    
    friend std::ostream& operator<<(std::ostream& os, const complex& c) {
        os << c.real << "+" << c.imag << "i";
        return os;
    }
    
    friend std::istream& operator>>(std::istream& is, complex& c) {
        is >> c.real >> c.imag;
        return is;
    }
    
    bool operator<(const complex& c) const {
        return (real*real + imag*imag) < (c.real*c.real + c.imag*c.imag);
    }
    
    bool operator==(const complex& c) const {
        return real == c.real && imag == c.imag;
    }
};
