#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

class Time {
    int seconds;

public:
    Time(int h = 0, int m = 0, int s = 0) {
        seconds = h * 3600 + m * 60 + s;
    }

    Time& operator++() {
        seconds++;
        return *this;
    }

    Time operator++(int) {
        Time temp = *this;
        seconds++;
        return temp;
    }

    bool operator==(const Time& t) const {
        return seconds == t.seconds;
    }

    bool operator<(const Time& t) const {
        return seconds < t.seconds;
    }

    void display() const {
        cout << seconds / 3600 << ":"
             << (seconds / 60) % 60 << ":"
             << seconds % 60 << endl;
    }
};

class Playlist {
    vector<string> tracks;

public:
    Playlist(vector<string> t) : tracks(t) {}

    string& operator[](int i) {
        if (i < 0 || i >= (int)tracks.size())
            throw out_of_range("Invalid playlist index");

        return tracks[i];
    }

    const string& operator[](int i) const {
        if (i < 0 || i >= (int)tracks.size())
            throw out_of_range("Invalid playlist index");

        return tracks[i];
    }
};

int main() {
    Time t(0, 0, 5);

    Time a = t++;
    Time b = ++t;

    cout << "Postfix: ";
    a.display();

    cout << "Prefix: ";
    b.display();

    cout << "Current Time: ";
    t.display();

    Playlist p({"Song 1", "Song 2", "Song 3"});

    try {
        cout << p[0] << endl;
        cout << p[1] << endl;
        cout << p[2] << endl;
    }
    catch (const out_of_range& e) {
        cout << e.what() << endl;
    }

    return 0;
}