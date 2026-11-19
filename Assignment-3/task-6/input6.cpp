#include <iostream>
#include <cstring>
using namespace std;

class Text {
    char* data;

public:
    Text(const char* str = "") {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }

    Text(const Text& t) {
        data = new char[strlen(t.data) + 1];
        strcpy(data, t.data);
    }

    Text& operator=(const Text& t) {
        if (this != &t) {
            char* temp = new char[strlen(t.data) + 1];
            strcpy(temp, t.data);

            delete[] data;
            data = temp;
        }
        return *this;
    }

    Text operator+(const Text& t) const {
        char* temp = new char[strlen(data) + strlen(t.data) + 1];

        strcpy(temp, data);
        strcat(temp, t.data);

        Text result(temp);
        delete[] temp;

        return result;
    }

    bool operator==(const Text& t) const {
        return strcmp(data, t.data) == 0;
    }

    bool operator!=(const Text& t) const {
        return !(*this == t);
    }

    friend ostream& operator<<(ostream& out, const Text& t) {
        out << t.data;
        return out;
    }

    ~Text() {
        delete[] data;
    }
};

int main() {
    Text a("Hello ");
    Text b("World");

    Text c = a + b;

    cout << "Text 1: " << a << endl;
    cout << "Text 2: " << b << endl;
    cout << "Combined: " << c << endl;

    if (a == b)
        cout << "Equal" << endl;
    else
        cout << "Not Equal" << endl;

    a = a;

    Text d;
    d = c;

    cout << "Assigned: " << d << endl;

    return 0;
}