#!/usr/bin/env python

import numpy as np
import scipy
from matplotlib import pyplot as plt
from scipy import optimize

n_fft, t_fft = np.loadtxt('datosFFT.txt', unpack=True)
n_dft, t_dft = np.loadtxt('datosDFT.txt', unpack=True)

def n_squared(x, c, d, f):
    return c * (x*x)

def nlogn(x, c):
    return c*x*np.log2(x)

fft_fit, cov = optimize.curve_fit(nlogn, n_fft, t_fft)
dft_fit, cov = optimize.curve_fit(n_squared, n_dft, t_dft)
t_fft_fit = nlogn(n_fft, *fft_fit)
t_dft_fit = n_squared(n_fft, *dft_fit)

plt.loglog(n_fft, t_fft, 'o', label='FFT',
           basex=2, basey=2)
plt.loglog(n_dft, t_dft, 'x', label='DFT',
           basex=2, basey=2)
# fitting
plt.loglog(n_fft, t_fft_fit,
           basex=2, basey=2)
plt.loglog(n_fft, t_dft_fit,
           basex=2, basey=2)
plt.xlabel('tamaño del vector')
plt.ylabel('tiempo de ejecución (segundos)')
print(fft_fit, dft_fit)
plt.legend()
plt.savefig('tiempos.png')
plt.show()
