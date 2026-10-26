#include <iostream>
using namespace std;
int main() {
    int balance = 6000;
    int fare = 1000;
    cout << "Test 1: Normal fare" << endl;
    cout << "Fare: Rs " << fare / 100 << endl;
    cout << "Test 2: Minimum balance" << endl;
    if (balance - fare >= 5000) {
        cout << "Journey allowed" << endl;
    } else {
        cout << "Journey denied" << endl;
    }
    cout << "Test 3: Low balance" << endl;
    balance = 5900;
    if (balance - fare >= 5000) {
        cout << "Journey allowed" << endl;
    } else {
        cout << "Journey denied" << endl;
    }
    cout << "Test 4: Invalid recharge" << endl;
    int recharge = -100;
    if (recharge <= 0) {
        cout << "Invalid recharge" << endl;
    }
    cout << "Test 5: Peak fare" << endl;
    fare = 3000;
    fare = fare + fare * 20 / 100;
    cout << "Peak fare: Rs " << fare / 100 << endl;
    return 0;
}