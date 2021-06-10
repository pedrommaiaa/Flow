#!/bin/sh
# del all the tests

cd ../

for i in input*
do if [ "out.$i" ]
   then
     rm -f out.$i
   fi
done

