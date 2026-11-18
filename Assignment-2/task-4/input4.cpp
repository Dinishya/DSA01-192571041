
#include <iostream>
using namespace std;

class SavingsAccount
{
private:
    int accountNumber;
    double balance;
    int transactions[10];
    int count;

public:
    SavingsAccount(int acc, double bal)
    {
        accountNumber = acc;
        balance = bal;
        count = 0;
    }

    void addTransaction(int amount)
    {
        if (count == 10)
        {
            for (int i = 0; i < 9; i++)
                transactions[i] = transactions[i + 1];

            count = 9;
        }

        transactions[count++] = amount;
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

    void miniStatement();
};

void SavingsAccount::miniStatement()
{
    cout << "\nAccount Number: "
         << accountNumber << endl;

    cout << "Balance: " << balance << endl;

    cout << "Last Transactions:" << endl;

    for (int i = 0; i < count; i++)
        cout << transactions[i] << endl;
}

int main()
{
    SavingsAccount acc(101, 5000);

    acc.deposit(1000);
    acc.withdraw(500);
    acc.deposit(2000);
    acc.withdraw(300);

    acc.miniStatement();

    return 0;
}
