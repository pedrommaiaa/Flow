sources = $(wildcard src/*.c)

flow: $(sources)
	gcc -o flow -g -Wall $(sources)

clean:
	- rm -f flow out
	- rm -f *.o *.s src/*.o

test: flow tests/bash/runtests.sh
	(cd tests/bash; chmod +x runtests.sh; ./runtests.sh)

test2: flow tests/input15.flow lib/printint.c
	./flow tests/input15.flow 
	gcc -o out out.s lib/printint.c
	./out
