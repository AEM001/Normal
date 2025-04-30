#include<iostream>
using namespace std;
class BankAccount {
private:
    double balance;
    
public:
    BankAccount(double b) : balance(b) {}
void deposit(double amount)
{balance+=amount;}
void withdraw(double amount){
balance-=amount;
}
void getprivate(){
cout<<"the current balance is:"<<balance;
}
friend class Bankmanager;
};

class Bankmanager {
    public:
    void displayBalance(const BankAccount& account)
    {
        cout<<"the rest of the balance is:"<<account.balance;
    }
    void adjustBalance(BankAccount& account, double adjustment)
    {
        account.balance+=adjustment;
    }
};
int main(){
    BankAccount ba(1000);
    Bankmanager bm;
    bm.displayBalance(ba);
    cout<<endl;
    bm.adjustBalance(ba,-100);
    cout<<endl;
    bm.displayBalance(ba);
    cout<<"the balance gotten from api" <<endl;
    ba.getprivate();
}