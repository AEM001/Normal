class Complex
{
private:
	double real, imag;
public:
	Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    friend istream& operator>>(istream& is,Complex&c)
    {
        is>>c.real>>c.imag;
        return is;
    }
    friend ostream& operator<<(ostream& os,const Complex&c)
    {
        os<<c.real<<"+"<<c.imag<<"i";
        return os;
    }
    friend bool operator<(Complex&c1,Complex&c2)
    {
        return (c1.real*c1.real+c1.imag*c1.imag)<(c2.real*c2.real+c2.imag*c2.imag);
    }
    friend bool operator>(Complex&c1,Complex&c2)
    {
        return (c1.real*c1.real+c1.imag*c1.imag)>(c2.real*c2.real+c2.imag*c2.imag);
   
    }
    friend bool operator==(Complex&c1,Complex&c2)
    {return c1.real==c2.real&&c1.imag==c2.imag;}
    

};