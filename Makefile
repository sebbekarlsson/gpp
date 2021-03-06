exec = a.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
sources_no_main = $(filter-out src/main.c, $(sources))
objects_no_main = $(sources_no_main:.c=.o)
flags = -g -Wall -lm -ldl -fPIC -rdynamic -L./extlib/. -ljson -I./extlib/libjson/src/include
LPATH=$$HOME/.local

$(info ${objects_no_main})


$(exec): $(objects)
	cd extlib/libjson && make && cp *.a ../.
	gcc $(objects) $(flags) -o $(exec)

libgpp.a: $(objects_no_main)
	ar rcs $@ $^

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make
	make libgpp.a
	mkdir -p $(LPATH)/include/gpp
	cp -r ./src/include/* $(LPATH)/include/gpp/.
	cp ./libgpp.a $(LPATH)/lib/.
	cp ./a.out $(LPATH)/bin/gpp

clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm src/*.o

lint:
	clang-tidy src/*.c src/include/*.h
