
#include <iostream>
using namespace std;

constexpr int FARE_A = 50;
constexpr int FARE_B = 80;
constexpr int SURCHARGE = 10;

enum class Zone { A, B };

int main()
{
    Zone z = Zone::A;
    int fare;

    if (z == Zone::A)
        fare = FARE_A;
    else
        fare = FARE_B;

    cout << "Fare: " << fare << endl;
    cout << "Surcharge: " << SURCHARGE << endl;
    cout << "Total: " << fare + SURCHARGE;

    return 0;
}
