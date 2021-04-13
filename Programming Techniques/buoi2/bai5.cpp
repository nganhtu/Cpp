#include <iostream>
#include <ostream>
#include <math.h>
#include <iomanip>

using namespace std;

struct Complex
{
    double real;
    double imag;
};

Complex operator+(Complex a, Complex b)
{
    return {a.real + b.real, a.imag + b.imag};
}

Complex operator-(Complex a, Complex b)
{
    return {a.real - b.real, a.imag - b.imag};
}

Complex operator*(Complex a, Complex b)
{
    return {a.real * b.real - a.imag * b.imag, a.real * b.imag + b.real * a.imag};
}

Complex operator/(Complex a, Complex b)
{
    if (b.real == 0 && b.imag == 0)
    {
        cout << "Divisor is 0" << endl;
        return {0, 0};
    }
    return {(a.real * b.real + a.imag * b.imag) / (pow(b.real, 2) + pow(b.imag, 2)), (b.real * a.imag - a.real * b.imag) / (pow(b.real, 2) + pow(b.imag, 2))};
}

ostream &operator<<(ostream &out, const Complex &i)
{
    out << '(' << std::setprecision(2) << i.real << (i.imag >= 0 ? '+' : '-') << std::setprecision(2) << fabs(i.imag) << 'i' << ')';
    return out;
}

int main()
{
    double real_a, real_b, img_a, img_b;
    cin >> real_a >> img_a;
    cin >> real_b >> img_b;

    Complex a{real_a, img_a};
    Complex b{real_b, img_b};

    cout << a << " + " << b << " = " << a + b << endl;
    cout << a << " - " << b << " = " << a - b << endl;
    cout << a << " * " << b << " = " << a * b << endl;
    cout << a << " / " << b << " = " << a / b << endl;

    return 0;
}
