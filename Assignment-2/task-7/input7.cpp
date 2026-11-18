
#include <iostream>
using namespace std;

class SavingsAccount
{
private:
    int accountNumber;
    double balance;

    int transactions[10];
    int count;
    int start;

public:
    SavingsAccount(int acc, double bal)
    {
        accountNumber = acc;
        balance = bal;
        count = 0;
        start = 0;
    }

    void addTransaction(int amount)
    {
        int index;

        if (count < 10)
        {
            index = (start + count) % 10;
            count++;
        }
        else
        {
            index = start;
            start = (start + 1) % 10;
        }

        transactions[index] = amount;
    }

    void deposit(int amount)
    {
        balance += amount;
        addTransaction(amount);
    }

    void withdraw(int amount)
    {
        if (amount <= balance)
        {
            balance -= amount;
            addTransaction(-amount);
        }
        else
        {
            cout << "Insufficient Balance" << endl;
        }
    }

    void display()
    {
        cout << "Account Number: "
             << accountNumber << endl;

        cout << "Balance: "
             << balance << endl;

        cout << "Last Transactions:" << endl;

        for (int i = 0; i < count; i++)
        {
            int index = (start + i) % 10;
            cout << transactions[index] << endl;
        }
    }
};

int main()
{
    SavingsAccount acc(101, 5000);

    acc.deposit(1000);
    acc.withdraw(500);
    acc.deposit(2000);
    acc.withdraw(300);
    acc.deposit(700);
    acc.withdraw(200);
    acc.deposit(1500);
    acc.withdraw(400);
    acc.deposit(800);
    acc.withdraw(100);
    acc.deposit(1200);
    acc.withdraw(600);

    acc.display();

    return 0;
}
