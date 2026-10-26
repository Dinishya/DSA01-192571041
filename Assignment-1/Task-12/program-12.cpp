#include <iostream>
using namespace std;
int main() {
    int zone = 4;
    int fare;

    if (zone == 1) {
        fare = 1000;
    } else if (zone == 2) {
        fare = 2000;
    } else if (zone == 3) {
        fare = 3000;
    } else if (zone == 4) {
        fare = 6000;
    } else {
        cout << "Invalid zone";
        return 0;
    }

    cout << "Airport fare: Rs " << fare / 100 << endl;

    // Weekend discount of 10%
    fare = fare - fare * 10 / 100;
    cout << "Weekend fare: Rs " << fare / 100 << endl;

        int journeyZone = 2;

    if (journeyZone == 1 || journeyZone == 2) {
        cout << "Monthly pass: Journey allowed" << endl;
    } else {
        cout << "Monthly pass: Not covered" << endl;
    }

    return 0;
}