exec = a.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g -Wall -lm -ldl -fPIC -rdynamic


$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

libgpp.a: $(objects)
	ar rcs $@ $^

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make
	make libgpp.a
	mkdir -p /usr/local/include/gpp
	cp -r ./src/include/* /usr/local/include/gpp/.
	cp ./libgpp.a /usr/local/lib/.
	cp ./gpp.out /usr/local/bin/gpp

clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm src/*.o

lint:
	clang-tidy src/*.c src/include/*.h
