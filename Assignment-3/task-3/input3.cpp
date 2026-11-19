#include <iostream>
using namespace std;

class Auditorium {
    int rows, seats;
    char** seatMap;

public:
    Auditorium(int r, int s) : rows(r), seats(s) {
        seatMap = new char*[rows];

        for (int i = 0; i < rows; i++) {
            seatMap[i] = new char[seats];

            for (int j = 0; j < seats; j++)
                seatMap[i][j] = 'O';
        }

        cout << "Auditorium created" << endl;
    }

    void book(int r, int s) {
        if (r >= 0 && r < rows && s >= 0 && s < seats) {
            seatMap[r][s] = 'B';
            cout << "Seat booked" << endl;
        } else {
            cout << "Invalid seat" << endl;
        }
    }

    void cancel(int r, int s) {
        if (r >= 0 && r < rows && s >= 0 && s < seats) {
            seatMap[r][s] = 'O';
            cout << "Booking cancelled" << endl;
        } else {
            cout << "Invalid seat" << endl;
        }
    }

    void display() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < seats; j++)
                cout << seatMap[i][j] << " ";
            cout << endl;
        }
    }

    ~Auditorium() {
        for (int i = 0; i < rows; i++)
            delete[] seatMap[i];

        delete[] seatMap;

        cout << "Auditorium destroyed" << endl;
    }
};

int main() {
    Auditorium a(3, 4);

    a.book(0, 1);
    a.book(1, 2);
    a.display();

    a.cancel(0, 1);
    a.display();

    Auditorium* p = new Auditorium(2, 3);
    p->book(0, 0);
    delete p;

    {
        Auditorium b(2, 2);
        b.book(1, 1);
    }

    return 0;
}