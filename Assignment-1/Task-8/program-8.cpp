#include <iostream>
using namespace std;
int main() {
    int amount;

    cout << "Enter recharge amount in rupees: ";
    cin >> amount;

    if (cin.fail()) {
        cout << "Invalid input";
    } else if (amount <= 0) {
        cout << "Enter a positive amount";
    } else {
        cout << "Recharge successful: Rs " << amount;
    }
    return 0;
}