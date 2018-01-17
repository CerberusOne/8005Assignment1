#!/bin/sh

function run() {
        echo "input ok"
        echo $1 >> a1_value
        gcc -Wall -g 8005a1.c primedecompose.c -o 8005a1 -l gmp && ./8005a1 $1
}

if [ "$#" == 0 ]; then
        echo "usage: a1.sh <number of mechanisms>"
        exit 1
fi

if [ -e a1_value ]; then 
        echo "removing a1_value"
        rm a1_value
fi

run $1 

