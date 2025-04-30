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
            cout<<"低于最低存款要求"<<endl;
            balance = 0; // 添加：不符合要求时余额设为0
        }
    }
    
    bool withdraw(double amount) override{
        if(withdraw_times<=0) {
            cout<<"超过免费取款次数，将要收取手续费"<<endl;
            balance--;
            return false; // 添加：超过次数后不允许取款
        }
        if(balance-amount<=100) {
            cout<<"低于最低存款要求"<<endl;
            return false;
        }
        balance-=amount;
        --withdraw_times;
        return true;
    }
    
    void display_details() const override{
        // 修正函数调用，添加括号
        cout<<"储蓄账户 "<<get_account_number()<<' '<<balance<<endl;
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
            cout<<"超过透支额度"<<endl;
            return false;
        }
        balance-=amount;
        return true;
    }
    void display_details()const override{
        // 修正函数调用，添加括号
        cout<<"支票账户 "<<get_account_number()<<' '<<balance<<endl;
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