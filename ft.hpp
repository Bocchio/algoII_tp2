#ifndef FT_HPP__
#define FT_HPP__

#include <cfloat>
#include <cmath>
#include <iostream>
#include "vector.hpp"
#include "complex.hpp"

// Helper function, a generic implementation of the Cooley-Tukey fft algorithm
static Vector<Complex> fft(const Vector<Complex>& x, double w = 2*M_PI) {
    Vector<Complex> y = Vector<Complex>(x);
    bool added_zero = false;

    size_t N = y.getSize();
    if (N == 1)
        return y;
    if (N % 2 == 1) {  // Fill with ceros if there's an odd size
        y += Vector<Complex>().append(Complex());
        N++;
        added_zero = true;
    }

    Vector<Complex> even = y.slice(0, -1, 2);  // take the even indexes
    Vector<Complex> odd = y.slice(1, -1, 2);  // take the odd indexes
    even = fft(even, w);
    odd = fft(odd, w);

    for (size_t i = 0; i < N/2; ++i) {
        double argument = -(w*i)/N;
        Complex twiddle(cos(argument), sin(argument));
        odd[i] *= twiddle;

        y[i] = even[i] + odd[i];
        y[i+N/2] = even[i] - odd[i];
    }
    if (added_zero)
       y.remove(N-1);

    return y;
}

class FourierTransform {
 public:
    virtual ~FourierTransform() {}
    virtual Vector<Complex> transform(const Vector<Complex>& x) = 0;
};

class DFT: public FourierTransform {
 public:
    virtual Vector<Complex> transform(const Vector<Complex>& x) {
        Vector<Complex> y = Vector<Complex>(x);

        size_t N = x.getSize();
        double argument = -(2*M_PI)/N;
        Complex wn(cos(argument), sin(argument));

        size_t y_size = y.getSize();
        size_t x_size = x.getSize();
        for (size_t yi = 0; yi < y_size; ++yi) {
            y[yi] = Complex();
            for (size_t xj = 0; xj < x_size; ++xj) {
                y[yi] += x[xj]*(wn^(yi*xj));
            }
        }
        return y;
    }
};

class IDFT: public FourierTransform {
 public:
    virtual Vector<Complex> transform(const Vector<Complex>& x){
        Vector<Complex> y = Vector<Complex>(x);

        size_t N = x.getSize();
        double argument = (2*M_PI)/N;
        Complex wn(cos(argument), sin(argument));
        for (Vector<Complex>::iterator yi = y.begin(); yi != y.end(); ++yi) {
            size_t i = yi - y.begin();
            *yi = Complex();
            for (Vector<Complex>::iterator xj = x.begin(); xj != x.end(); ++xj) {
                size_t j = xj - x.begin();
                *yi += (*xj)*(wn^(i*j));
            }
            *yi /= N;
        }
        return y;
    }
};

class FFT: public FourierTransform {
 public:
    virtual Vector<Complex> transform(const Vector<Complex> &x) {
        return fft(x);
    }
};

class IFFT: public FourierTransform {
 public:
    virtual Vector<Complex> transform(const Vector<Complex> & x){
        return fft(x, -2*M_PI)/x.getSize();
    }
};

#endif  // FT_HPP_
