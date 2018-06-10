#!/usr/bin/env python

from cmath import pi, e

c = complex
# x = [c(0, 0),
#      c(0, 0),
#      c(2, 0),
#      c(3, 0),
#      c(4, 0),
#      c(0, 0),
#      c(0, 0),
#      c(0, 0)]

x = [c(1, 0),
     c(1, 0),
     c(1, 0),
     c(1, 0)]

X = []

wn = e**(-c(0,2*pi/10))

print(wn**3)
for k, val in enumerate(x):
    print(k, val)
    xk = 0
    for n, xn in enumerate(x):
        xk += xn*(wn**(k*n))
    X.append(xk)

for xk in X:
    print("%+.2f" % round(xk.real, 2), "%+.2f" % round(xk.imag, 2))
