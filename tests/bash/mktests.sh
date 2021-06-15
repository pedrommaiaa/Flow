#!/bin/sh
# Make the output files for each test

cd ../

if [ ! -f ../flow ]
then echo "Need to build ../flow first!"; exit 1
fi

for i in input*
do if [ ! -f "out.$i" ]
   then
     ../flow $i
     cc -o out out.s ../lib/printint.c
     ./out > out.$i
     rm -f out out.s
   fi
done
