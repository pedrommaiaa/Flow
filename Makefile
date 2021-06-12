sources = $(wildcard src/*.c)

flow: $(sources)
	gcc -o flow -g $(sources)

clean:
	- rm -f flow out
	- rm -f *.o *.s src/*.o

test: flow tests/bash/runtests.sh
	(cd tests/bash; chmod +x runtests.sh; ./runtests.sh)

test1: flow tests/input06
	./flow tests/input06
	gcc -o out out.s
	./out
