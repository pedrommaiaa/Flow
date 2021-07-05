sources = $(wildcard src/*.c)

flow: $(sources)
	gcc -o flow -g -Wall $(sources)

clean:
	- rm -f flow out
	- rm -f *.o *.s out src/*.o

test: flow tests/bash/runtests.sh
	(cd tests/bash; chmod +x runtests.sh; ./runtests.sh)

