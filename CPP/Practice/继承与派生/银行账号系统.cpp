#include<iostream>
#include<string>
using namespace std;
class Account{
private:
    string account_number;
protected:
    double balance;
public:
    Account(string num,double a):account_number(num),balance(a){}
    virtual bool deposit(double amount)
    {if(amount>=0)
        {
            balance+=amount;
            return true;
        }
        return false;

    };
    virtual bool withdraw(double amount){
        if(balance>=amount)
        {balance-=amount;
        return true;}
        else{
            return false;
        }
    }
    double get_balance() const{return balance;}
    string get_account_number() const{return account_number;}
    virtual void display_details() const{}
    virtual ~Account(){}
    
};

class SavingsAccount: public Account{
    int withdraw_times=3;
    public:
    SavingsAccount(string num,double a):Account(num,a){
        if(a<200){
            cout<<"������ʹ��Ҫ��"<<endl;
            balance = 0; // ��ӣ�������Ҫ��ʱ�����Ϊ0
        }
    }
    
    bool withdraw(double amount) override{
        if(withdraw_times<=0) {
            cout<<"�������ȡ���������Ҫ��ȡ������"<<endl;
            balance--;
            return false; // ��ӣ���������������ȡ��
        }
        if(balance-amount<=100) {
            cout<<"������ʹ��Ҫ��"<<endl;
            return false;
        }
        balance-=amount;
        --withdraw_times;
        return true;
    }
    
    void display_details() const override{
        // �����������ã��������
        cout<<"�����˻� "<<get_account_number()<<' '<<balance<<endl;
    }
};
class CheckingAccount :public Account{
    public:
    CheckingAccount(string num,double a):Account(num,a){}
    bool deposit(double amount)override{
        if(amount>=0)
        {
            balance+=amount;
            return true;
        }
        return false;
    }
    bool withdraw(double amount){
        if(balance -amount<=-100)
        {
            cout<<"����͸֧���"<<endl;
            return false;
        }
        balance-=amount;
        return true;
    }
    void display_details()const override{
        // �����������ã��������
        cout<<"֧Ʊ�˻� "<<get_account_number()<<' '<<balance<<endl;
    }
};
int main()
{
    SavingsAccount s("01",300);
    s.deposit(100);
    s.display_details();
    s.withdraw(50);
    s.display_details();
    s.withdraw(50);
    s.withdraw(50);
    s.withdraw(50);
    CheckingAccount c("02",300);
    c.deposit(100);
    c.display_details();
    c.withdraw(500);
}