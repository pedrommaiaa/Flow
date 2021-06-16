#!/bin/sh
# Make the output files for each test

cd ../

for i in input*flow
do if [ ! -f "results/out.$i" ]
   then
     ../flow $i
     cc -o out out.s ../lib/printint.c
     ./out > results/out.$i
     rm -f out out.s
   fi
done
