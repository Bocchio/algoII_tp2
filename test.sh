#!/usr/bin/env sh

PATH="./ejemplos/"
declare -i errors=0
for transformado in "$PATH""T"*; do
    original=${transformado#$PATH}
    original=$PATH${original:1}

    # Check if the DFT works
    #echo "ejecutando ./tp2 -i $original -m FFT -r $transformado"
    if ./tp2 -i "$original" -m FFT -r "$transformado" -e 2; then
        :
    else
        errors=$errors+1
        echo "Error in ""$original"
    fi

    # Check if the IDFT works
    #echo "ejecutando ./tp2 -i $transformado -m IFFT -r $original"
    if ./tp2 -i "$transformado" -m IFFT -r "$original" -e 2; then
        :
    else
        errors=$errors+1
        echo "Error in ""$transformado"
    fi

done
echo "A total of ""$errors"" errors were found."
