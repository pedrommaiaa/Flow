exec = flow
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -Wall -g

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@


clean:
	- rm -f flow
	- rm -f *.o
	- rm -f src/*.o

test: flow
	-(./flow examples/input01; \
	 ./flow examples/input02; \
	 ./flow examples/input03; \
	 ./flow examples/input04; \
	 ./flow examples/input05)
