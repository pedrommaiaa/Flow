#!/bin/sh
# Make the output files for each test

cd ../

# Build our compiler if needed
if [ ! -f ../flow ]
then (cd ..; make)
fi

for i in input*c
do if [ ! -f "out.$i" -a ! -f "err.$i" ]
   then
     ../flow -o out $i 2> "err.$i"
     # If the err file is empty
     if [ ! -s "err.$i" ]
     then
        rm -f "err.$i"
        cc -o out $i ../lib/printint.c
        ./out > "out.$i"
      fi
   fi
   rm -f out out.s
done
