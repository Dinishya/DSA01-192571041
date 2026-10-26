#include <iostream>
#include <iomanip>
using namespace std;
int main() {
    int journeys = 1000;

    double fare = 0.10;
    double total1 = journeys * fare;

    int farePaise = 10;
    long long total2 = (long long)journeys * farePaise;

    cout << fixed << setprecision(2);

    cout << "Double total: Rs " << total1 << endl;
    cout << "Paise total: Rs "
         << total2 / 100.0 << endl;
    return 0;
}