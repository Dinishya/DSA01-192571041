#include <iostream>
#include <string>
using namespace std;

class Book {
    string title;
    int copies;

public:
    Book() : Book("", 0) {}

    Book(string t) : Book(t, 0) {}

    Book(string t, int c) : title(t), copies(c) {
        if (copies < 0)
            throw invalid_argument("Invalid number of copies");
    }

    void display() {
        cout << "Title: " << title << endl;
        cout << "Copies: " << copies << endl;
    }
};

int main() {
    Book b1;
    Book b2("C++ Programming");
    Book b3("Data Structures", 10);

    b1.display();
    b2.display();
    b3.display();

    return 0;
}