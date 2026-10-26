#include <iostream>
using namespace std;
class SmartCard {
private:
    int balance;
public:
    SmartCard(int b) {
        balance = b;
    }
    void payFare(int fare) {
        if (fare < 0) {
            cout << "Invalid fare" << endl;
        } else if (balance - fare >= 5000) {
            balance = balance - fare;
            cout << "Payment successful" << endl;
        } else {
            cout << "Insufficient balance" << endl;
        }
    }
    void display() {
        cout << "Balance: Rs " << balance / 100 << endl;
    }
};
int main() {
    SmartCard card(10000);
    card.payFare(3000);
    card.display();
    return 0;
}