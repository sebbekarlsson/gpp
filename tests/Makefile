exec = a.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g -Wall -ldl -fPIC -rdynamic -L../extlib/. -L./ -lgpp -ljson -lm -I../extlib/libjson/src/include -I../src/include

$(info ${objects})

$(exec): $(objects)
	cd ../extlib/libjson && make && cp *.a ../../tests
	cd ../ && make libgpp.a && cp *.a ./tests
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm src/*.o

lint:
	clang-tidy src/*.c src/include/*.h
