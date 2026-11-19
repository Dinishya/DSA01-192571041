
#include <iostream>
#include <string>
using namespace std;

class Vehicle
{
protected:
    string regNo;
    double dailyRate;
    bool rented = false;

public:
    Vehicle(string r, double d)
        : regNo(r), dailyRate(d) {}

    virtual double rentalCharge(int days) = 0;
    virtual void display() = 0;

    void rent()
    {
        if(!rented)
        {
            rented = true;
            cout << regNo << " rented successfully" << endl;
        }
        else
            cout << "Vehicle unavailable" << endl;
    }

    void returnVehicle(int days)
    {
        if(rented)
        {
            double charge = rentalCharge(days);
            double late = days > 3 ? (days - 3) * 100 : 0;

            cout << "Rental charge: " << charge << endl;
            cout << "Late charge: " << late << endl;
            cout << "Total: " << charge + late << endl;

            rented = false;
        }
    }

    virtual ~Vehicle() {}
};

class Bicycle : public Vehicle
{
    int gears;

public:
    Bicycle(string r, double d, int g)
        : Vehicle(r, d), gears(g) {}

    double rentalCharge(int days) override
    {
        return dailyRate * days;
    }

    void display() override
    {
        cout << "Bicycle: " << regNo
             << " Gears: " << gears << endl;
    }
};

class MotorVehicle : public Vehicle
{
protected:
    double fuelLevel;
    string serviceDate;

public:
    MotorVehicle(string r, double d, double f, string s)
        : Vehicle(r, d), fuelLevel(f), serviceDate(s) {}
};

class Car : public MotorVehicle
{
    int seats;

public:
    Car(string r, double d, double f, string s, int st)
        : MotorVehicle(r, d, f, s), seats(st) {}

    double rentalCharge(int days) override
    {
        return dailyRate * days;
    }

    void display() override
    {
        cout << "Car: " << regNo
             << " Seats: " << seats << endl;
    }
};

class ElectricCar : public MotorVehicle
{
protected:
    double batteryCapacity;
    double kwhRate;

public:
    ElectricCar(string r, double d, double f, string s,
                double b, double k)
        : MotorVehicle(r, d, f, s),
          batteryCapacity(b), kwhRate(k) {}

    double rentalCharge(int days) override
    {
        return dailyRate * days + batteryCapacity * kwhRate;
    }

    void display() override
    {
        cout << "Electric Car: " << regNo
             << " Battery: " << batteryCapacity << endl;
    }
};

class ElectricBus : public ElectricCar
{
    int capacity;

public:
    ElectricBus(string r, double d, double f, string s,
                double b, double k, int c)
        : ElectricCar(r, d, f, s, b, k), capacity(c) {}

    double rentalCharge(int days) override
    {
        return dailyRate * days + batteryCapacity * kwhRate;
    }

    void display() override
    {
        cout << "Electric Bus: " << regNo
             << " Capacity: " << capacity << endl;
    }
};

int main()
{
    Vehicle* fleet[5];

    fleet[0] = new Bicycle("B101", 100, 6);
    fleet[1] = new Car("C101", 1500, 50, "20-12-2026", 5);
    fleet[2] = new ElectricCar("E101", 2000, 80, "25-12-2026", 50, 10);
    fleet[3] = new ElectricBus("EB101", 3000, 100, "30-12-2026", 100, 10, 40);
    fleet[4] = new Bicycle("B102", 150, 5);

    for(int i = 0; i < 5; i++)
        fleet[i]->display();

    for(int i = 0; i < 4; i++)
    {
        fleet[i]->rent();
        fleet[i]->returnVehicle(4);
    }

    for(int i = 0; i < 5; i++)
        delete fleet[i];

    return 0;
}
