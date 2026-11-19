
#include <iostream>
#include <string>
using namespace std;

class Address
{
    string city;
    int pin;

public:
    Address(string c, int p) : city(c), pin(p) {}

    string str() const
    {
        return city + "-" + to_string(pin);
    }
};

class Person
{
protected:
    string name;
    int age;
    Address home;

public:
    Person(string n, int a, string c, int p)
        : name(n), age(a), home(c, p) {}
};

class Course
{
    string courseName;

public:
    Course(string c) : courseName(c) {}

    string getCourse() const
    {
        return courseName;
    }
};

class Student : public Person
{
    long long regNo;
    Course course;

public:
    Student(string n, int a, long long r, string c, int p,
            string co)
        : Person(n, a, c, p), regNo(r), course(co) {}

    void print() const
    {
        cout << regNo << " " << name << " (" << age
             << ") " << home.str() << " "
             << course.getCourse() << endl;
    }
};

int main()
{
    Student s("Kavya", 19, 192311001, "Salem",
              636001, "C++");

    s.print();

    return 0;
}
