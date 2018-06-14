#!/usr/bin/env bash

echo "$ ./tp2 -m dft -o regressions2.txt < entrada2.txt"
./tp2 -m dft -o regressions2.txt < entrada2.txt
echo "$ ./tp2 -m fft -r regressions2.txt < entrada2.txt"
./tp2 -m fft -r regressions2.txt < entrada2.txt
v=$?
echo "$ echo $?"
echo $v
echo ""

echo "$ cat entrada3.txt"
cat entrada3.txt
echo "$ ./tp2 -m ifft < entrada3.txt"
./tp2 -m ifft < entrada3.txt
echo "$ ./tp2 -m idft -o salida3.txt < entrada3.txt"
./tp2 -m idft -o salida3.txt < entrada3.txt
echo "$ cat salida3.txt"
cat salida3.txt

echo ""

echo "$ cat entrada4.txt"
cat entrada4.txt
echo "$ cat regressions4.txt"
cat regressions4.txt
echo "$ ./tp2 -m idft -r regressions4.txt -o salida4.txt < entrada4.txt"
./tp2 -m idft -r regressions4.txt -o salida4.txt < entrada4.txt
v=$?
echo "$ echo $?"
echo $v
echo "$ cat salida4.txt"
cat salida4.txt
