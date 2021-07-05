#!/bin/sh
# Run each test and compare
# against known good output

cd ../

# Build our compiler if needed
if [ ! -f ../flow ]
then (cd ..; make)
fi

# Try to use each input source file
for i in input*
# We can't do anything if there's no file to test against
do if [ ! -f "results/out.$i" -a ! -f "err/err.$i" ]
   then echo "Can't run test on $i, no output file!"

   # Output file: compile the source, run it and
   # capture the output, and compare it against
   # the known-good output
   else if [ -f "results/out.$i" ]
        then
	        # Print the test name, compile it
	        # with our compiler
          echo -n $i
          ../flow -o out $i
          ./out > trial.$i

  	      # Compare this agains the correct output
          cmp -s "out.$i" "trial.$i"

	        # If different, announce failure
          # and print out the difference
          if [ "$?" -eq "1" ]
          then echo ": failed"
            diff -c "out.$i" "trial.$i"
            echo

	        # No failure, so announce success
          else echo ": OK"
          fi

   # Error file: compile the source and
   # capture the error messages. Compare
   # against the known-bad output. Same
   # mechanism as before
   else if [ -f "err/err.$i" ]
        then
          echo -n $i
          ../flow $i 2> "trial.$i"
          cmp -s "err.$i" "trial.$i"
          if [ "$?" -eq "1" ]
          then echo ": failed"
            diff -c "err/err.$i" "trial.$i"
            echo
          else echo ": OK"
          fi
        fi
     fi
   fi
   rm -f out out.s "input49.s" "trial.$i"
done
