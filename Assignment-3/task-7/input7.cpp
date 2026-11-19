#include <iostream>
#include <cmath>
using namespace std;

class Complex {
    double real, imag;

public:
    Complex() : real(0), imag(0) {}

    Complex(double r) : real(r), imag(0) {}

    Complex(double r, double i) : real(r), imag(i) {}

    Complex operator+(const Complex& c) const {
        return Complex(real + c.real, imag + c.imag);
    }

    Complex operator-(const Complex& c) const {
        return Complex(real - c.real, imag - c.imag);
    }

    Complex operator*(const Complex& c) const {
        return Complex(
            real * c.real - imag * c.imag,
            real * c.imag + imag * c.real
        );
    }

    Complex operator/(const Complex& c) const {
        double denominator = c.real * c.real + c.imag * c.imag;

        if (denominator == 0)
            throw runtime_error("Division by zero");

        return Complex(
            (real * c.real + imag * c.imag) / denominator,
            (imag * c.real - real * c.imag) / denominator
        );
    }

    bool operator==(const Complex& c) const {
        return real == c.real && imag == c.imag;
    }

    double magnitude() const {
        return sqrt(real * real + imag * imag);
    }

    friend ostream& operator<<(ostream& out, const Complex& c) {
        out << c.real;

        if (c.imag >= 0)
            out << " + " << c.imag << "j";
        else
            out << " - " << -c.imag << "j";

        return out;
    }
};

int main() {
    try {
        Complex R(10, 0);
        Complex L(0, 5);
        Complex C(0, -3);

        Complex series = R + L + C;

        cout << "Series Impedance: " << series << endl;
        cout << "Magnitude: " << series.magnitude() << endl;

        Complex Z1(10, 5);
        Complex Z2(5, -3);

        Complex parallel = (Z1 * Z2) / (Z1 + Z2);

        cout << "Parallel Impedance: " << parallel << endl;
        cout << "Magnitude: " << parallel.magnitude() << endl;

        Complex zero(0, 0);
        Complex result = Z1 / zero;

        cout << result << endl;
    }
    catch (const runtime_error& e) {
        cout << e.what() << endl;
    }

    return 0;
}