#!/bin/sh
# del all the tests

cd ../

for i in input*flow
do if [ "results/out.$i" ]
   then
     rm -f results/out.$i
   fi
done

