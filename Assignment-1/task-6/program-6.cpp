#include <iostream>
using namespace std;
enum class Zone { Inner, Middle, Outer };

constexpr int BASE[] = {1000, 2000, 3000};
constexpr int SURCHARGE = 20;
constexpr long long MIN_BALANCE = 5000;

long long fare(Zone z, bool peak)
{
    int amount = 0;

    switch (z)
    {
        case Zone::Inner: amount = BASE[0]; break;
        case Zone::Middle: amount = BASE[1]; break;
        case Zone::Outer: amount = BASE[2]; break;
    }

    if (peak)
        amount += amount * SURCHARGE / 100;

    return amount;
}

bool canTravel(long long balance, long long f)
{
    return balance - f >= MIN_BALANCE;
}

int main()
{
    long long balance = 10000;

    Zone trips[] = {
        Zone::Inner, Zone::Outer, Zone::Middle
    };

    bool peak[] = {false, true, false};

    for (int i = 0; i < 3; i++)
    {
        long long f = fare(trips[i], peak[i]);

        if (canTravel(balance, f))
        {
            balance -= f;
            cout << "Fare in paise: " << f;
        }
        else
            cout << "Denied";

        cout << " Balance: " << balance << endl;
    }

    return 0;
}