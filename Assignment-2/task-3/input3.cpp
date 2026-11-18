
#include <iostream>
using namespace std;

struct Parcel
{
    double weight;
    bool express;
    bool insured;
};

double charge(int grams)
{
    return 30 + grams * 0.05;
}

double charge(double kg, bool express = false,
              bool insured = false)
{
    double amount = 30 + kg * 50;

    if (express)
        amount += 40;

    if (insured)
        amount += 20;

    return amount;
}

double charge(const Parcel& p)
{
    return charge(p.weight, p.express, p.insured);
}

int main()
{
    Parcel p = {3.0, true, true};

    cout << "Charge 1: " << charge(2) << endl;
    cout << "Charge 2: " << charge(2.0f) << endl;
    cout << "Charge 3: " << charge(500) << endl;
    cout << "Charge 4: " << charge(2.5) << endl;
    cout << "Charge 5: " << charge(2.5, true) << endl;
    cout << "Charge 6: " << charge(p) << endl;

    return 0;
}
