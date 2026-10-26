#include <iostream>
using namespace std;
class SmartCard {
public:
    int balance;
    SmartCard(int b) {
        balance = b;
    }    
void travel(int fare) {
    if (balance - fare >= 50) {
            balance = balance - fare;
            cout << "Journey Successful\n";
            cout << "Fare: Rs." << fare << endl;
            cout << "Balance: Rs." << balance;
        } else {
            cout << "Journey Rejected";
        }
    }
};
int main() {
    SmartCard card(120);
    card.travel(50);
    return 0;
}