#!/bin/bash

cd ../

if [ ! -f ../flow ]
then echo "Need to build ../flow first!"; exit 1
fi

for i in input*
do if [ ! -f "out.$i" ]
   then echo "Can't run test on $i, no output file!"
   else
     echo -n $i
     ../flow $i
     cc -o out out.s
     ./out > trial.$i
     cmp -s "out.$i" "trial.$i"
     if [ "$?" -eq "1" ]
     then echo ": failed"
       diff -c "out.$i" "trial.$i"
       echo
     else echo ": OK"
     fi
     rm -f out out.s "trial.$i"
   fi
done
