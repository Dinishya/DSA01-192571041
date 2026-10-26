#include <iostream>
using namespace std;

long long balance = 10000;
const long long MIN_BALANCE = 5000;

bool canTravel(long long fare)
{
    return balance - fare >= MIN_BALANCE;
}

void deduct(long long fare)
{
    if (canTravel(fare))
    {
        balance -= fare;
        cout << "Travel allowed\n";
    }
    else
        cout << "Travel denied\n";
}

int main()
{
    deduct(2000);
    cout << "Balance: " << balance;

    return 0;
}