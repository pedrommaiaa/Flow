#!/bin/sh
# Run each test and compare
# against known good output

cd ../

if [ ! -f ../flow ]
then echo "Need to build ../flow first!"; exit 1
fi

for i in input*c
do if [ ! -f "results/out.$i" ]
   then echo "Can't run test on $i, no output file!"
   else
     echo -n $i
     ../flow $i
     cc -o out out.s ../lib/printint.c
     ./out > trial.$i
     cmp -s "results/out.$i" "trial.$i"
     if [ "$?" -eq "1" ]
     then echo ": failed"
       diff -c "results/out.$i" "trial.$i"
       echo
     else echo ": OK"
     fi
     rm -f out out.s "trial.$i"
   fi
done
