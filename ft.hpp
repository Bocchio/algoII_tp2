#ifndef FT_HPP__
#define FT_HPP__

#include <cfloat>

#ifdef _WIN32
    #define _USE_MATH_DEFINES
    #include <math.h>
#else
    #include <cmath>
#endif

#include <iostream>
#include "vector.hpp"
#include "complex.hpp"

// Helper function, a generic implementation of the Cooley-Tukey fft algorithm
static Vector<Complex> fft(const Vector<Complex>& x, const Vector<Complex> &W, double direction = 1) {
    if (x.getSize() == 0 || x.getSize() == 1)
        return x;

    Vector<Complex> y = Vector<Complex>(x);
    // pad with 0s
    size_t N = 1 << (size_t) ceil(log2(x.getSize()));
    for (size_t i = x.getSize(); i < N; i++) {
        y.append(Complex());
    }

    Vector<Complex> even = y.slice(0, N, 2);  // take the even indexes
    Vector<Complex> odd = y.slice(1, N, 2);  // take the odd indexes
    even = fft(even, W, direction);
    odd = fft(odd, W, direction);

    Vector<Complex> Wn = W.slice(0, W.getSize(), W.getSize()/N);
    for (size_t i = 0; i < N/2; ++i) {
        odd[i] *= Wn[i];

        y[i] = even[i] + odd[i];
        y[i+N/2] = even[i] - odd[i];
    }

    return y;
}

class FourierTransform {
 protected:
    Vector<Complex> W;
 public:
    FourierTransform() {
        resizeW(1024);
    }

    void resizeW(size_t new_size){
        W = Vector<Complex>();
        new_size = 1 << (size_t) ceil(log2(new_size));
        double theta = (2*M_PI)/new_size;
        for (size_t i = 0; i < new_size; i++) {
            W.append(Complex(cos(i*theta), sin(i*theta)));
        }
    }

    virtual ~FourierTransform() {}
    virtual Vector<Complex> transform(const Vector<Complex>& x) = 0;
};

class DFT: public FourierTransform {
 public:
    virtual Vector<Complex> transform(const Vector<Complex>& x) {
        Vector<Complex> y = Vector<Complex>(x);

        if (x.getSize() == 0)
            return y;

        // pad with 0s
        size_t N = 1 << (size_t) ceil(log2(x.getSize()));
        for (size_t i = x.getSize(); i < N; i++) {
            y.append(Complex());
        }
        Vector<Complex> original_vector = y;
        if (N > W.getSize())
            resizeW(N);

        Vector<Complex> Wn = W.slice(0, W.getSize(), W.getSize()/N);
        for (size_t yi = 0; yi < N; ++yi) {
            y[yi] = Complex();
            for (size_t xj = 0; xj < N; ++xj) {
                y[yi] += original_vector[xj]*(Wn[(yi*xj)%N]);
            }
        }

        return y;
    }
};

class IDFT: public FourierTransform {
 public:
    virtual Vector<Complex> transform(const Vector<Complex>& x){
        Vector<Complex> y = Vector<Complex>(x);

        if (x.getSize() == 0)
            return y;

        // pad with 0s
        size_t N = 1 << (size_t) ceil(log2(x.getSize()));
        for (size_t i = x.getSize(); i < N; i++) {
            y.append(Complex());
        }
        Vector<Complex> original_vector = y;
        if (N > W.getSize())
            resizeW(N);

        Vector<Complex> Wn = W.slice(0, W.getSize(), W.getSize()/N);
        for (Vector<Complex>::iterator yi = y.begin(); yi != y.end(); ++yi) {
            size_t i = yi - y.begin();
            *yi = Complex();
            for (Vector<Complex>::iterator xj = original_vector.begin(); xj != original_vector.end(); ++xj) {
                size_t j = (size_t) (xj - original_vector.begin());
                *yi += (*xj)*(Wn[(j*i)%N]);
            }
            *yi /= N;
        }

        return y;
    }
};

class FFT: public FourierTransform {
 public:
    virtual Vector<Complex> transform(const Vector<Complex> &x) {
        if (x.getSize() == 0)
            return x;
        size_t N = 1 << (size_t) ceil(log2(x.getSize()));
        if (N > W.getSize())
            resizeW(N);

        return fft(x, W);
    }
};

class IFFT: public FourierTransform {
 public:
    virtual Vector<Complex> transform(const Vector<Complex> & x){
        if (x.getSize() == 0)
            return x;
        size_t N = 1 << (size_t) ceil(log2(x.getSize()));
        if (N > W.getSize())
            resizeW(N);

        return fft(x, W, -1)/N;
    }
};

#endif  // FT_HPP_
