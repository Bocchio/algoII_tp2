#ifndef COMPLEX_HPP__
#define COMPLEX_HPP__

#include <iostream>
#ifdef _WIN32
    #define _USE_MATH_DEFINES
    #include <math.h>
#else
    #include <cmath>
#endif

using std::cout;
using std::endl;
using std::ostream;
using std::istream;

#define COMPLEX_TOLERANCE 1E-4

class Complex{
    double real, imag;

 public:
    Complex(double a, double b) {
        real = a;
        imag = b;
    }

    Complex() {
        real = 0.0;
        imag = 0.0;
    }

    Complex(const Complex& c) {
        real = c.real;
        imag = c.imag;
    }

    ~Complex() {}

    void setReal(double new_value) {
        real = new_value;
    }

    void setImag(double new_value) {
        imag = new_value;
    }

    double getReal() const {
        return real;
    }

    double getImag() const {
        return imag;
    }

    void setMod(double new_value) {
        double factor = new_value/getMod();  // If modulus is 0, it'll raise an
        imag *= factor;                      // exception
        real *= factor;
    }

    void setArg(double new_value) {
        double mod = getMod();
        real = mod*cos(new_value);
        imag = mod*sin(new_value);
    }

    double getMod() const {
        return sqrt(imag*imag + real*real);
    }

    double getArg() const {
        if (real > 0)
            return atan(imag/real);
        if (real < 0 && imag >= 0)
            return atan(imag/real) + M_PI;
        if (real < 0 && imag < 0)
            return atan(imag/real) - M_PI;
        if (real == 0 && imag > 0)
            return M_PI_2;
        if (real == 0 && imag < 0)
            return -M_PI_2;
        return 0;  // actually undefined
    }

    Complex getConjugate() {
        return Complex(real, -imag);
    }

    Complex& operator=(const Complex& r) {
        real = r.real;
        imag = r.imag;
        return *this;
    }

    Complex operator+(const Complex& r) const {
        return Complex(real + r.real, imag + r.imag);
    }

    Complex operator-(const Complex& r) const {
        return Complex(real - r.real, imag - r.imag);
    }

    Complex operator/(double r) {
        return Complex(real/r, imag/r);
    }

    Complex& operator+=(const Complex& r) {
        real += r.real;
        imag += r.imag;
        return *this;
    }

    Complex& operator/=(double r) {
        real /= r;
        imag /= r;
        return *this;
    }

    Complex& operator*=(double r) {
        real *= r;
        imag *= r;
        return *this;
    }

    Complex& operator*=(const Complex& r) {
        Complex temp(real*r.real - imag*r.imag, real*r.imag + imag*r.real);
        real = temp.real;
        imag = temp.imag;
        return *this;
    }

    Complex operator*(const Complex& r) const {
        return Complex(real*r.real - imag*r.imag, real*r.imag + imag*r.real);
    }

    Complex operator*(double r) const {
        return Complex(r*real, r*imag);
    }

    Complex operator^(long exponent) const {
        double arg = getArg();
        double mod = getMod();
        mod = pow(mod, exponent);
        arg = arg*exponent;
        return Complex(mod*cos(arg), mod*sin(arg));
    }

    bool operator==(const Complex& r) const {
        Complex temp(real-r.real, imag-r.imag);
        if (temp.getMod() < COMPLEX_TOLERANCE)
            return true;
        return false;
    }

    bool operator!=(const Complex& r) const {
        return !(*this == r);
    }

    friend ostream& operator<<(ostream& os, const Complex& c) {
        os << "("
           << c.real
           << ","
           << c.imag
           << ")";
        return os;
    }

    friend istream& operator>>(istream& is, Complex& c) {
        bool good = true;
        double real = 0;
        double imag = 0;
        char ch = 0;

        if (is >> ch && ch == '(') {
            if (is >> real &&
                is >> ch && ch == ',' &&
                is >> imag &&
                is >> ch && ch == ')')
                good = true;
            else
                good = false;
        } else if (is.good()) {  // read single real
            is.putback(ch);
            good = (is >> real) ? true : false;
        }

        if (good) {
            c.real = real;
            c.imag = imag;
        } else {
            is.clear(std::ios::badbit);
        }

        return is;
    }
};

#endif  // COMPLEX_HPP__
