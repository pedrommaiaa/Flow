exec = flow
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -Wall -g


$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)


%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@


clean:
	- rm -f flow out
	- rm -f *.o *.s src/*.o


test: flow
	./flow examples/input01
	gcc -o out out.s
	./out
	
	./flow examples/input02
	gcc -o out out.s
	./out
	
	./flow examples/input03
	gcc -o out out.s
	./out

