#!/usr/bin/env python

import sys
from math import pi, ceil, floor, sin, cos
import time

def fft(x):
    y = list(x)
    N = len(y)
    if len(y) == 1:
        return y
    even = fft(y[0::2])
    odd = fft(y[1::2])

    m = floor(N/2)
    for i in range(m):
        argument = -(2*pi*i)/N
        twiddle = complex(cos(argument), sin(argument))
        odd[i] *= twiddle
        y[i] = even[i] + odd[i]
        y[i+m] = even[i] - odd[i]
    return y


datos = ""
v = []
with open(sys.argv[1], 'r') as f:
	datos = f.read()
datos = datos.split(')')
for dato in datos:
	if dato == "":
		continue
	dato = dato.split('(')[1].split(',')
	v.append(complex(float(dato[0]), float(dato[1])))

print('Comenzó')
s = time.time()
fft(v)
e = time.time() - s
print('Terminó', e, 's')

if len(sys.argv) == 3 and sys.argv[2] == 'v':
	print(v)
	t = fft(v)
	for i,l in enumerate(t):
		t[i] = complex('{:.2f}'.format(l))
	print(t)
