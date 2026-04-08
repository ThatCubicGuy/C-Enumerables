# Compiler settings
CC:=gcc
CFLAGS:=-Wextra -Iinclude -std=gnu23
OUTPUT:=thing.exe

# Library files
LIBSRCS:=$(wildcard src/*.c)
LIBHDRS:=$(wildcard include/*.h) $(wildcard include/*/*.h) $(wildcard include/*/*/*.h)
LIBOBJS:=${LIBSRCS:src/%.c=bin/lib/%.o}

# Tests
TESTSRCS:=$(wildcard tests/*.c)

test: buildtest runtest

pack: lib
	ar rcs libcollections.a $(wildcard bin/lib/*.o)
	tar -zcf CubeEnumerables.tar.gz CubeEnumerables.h CubeEnumerablesGenericImplementations.h include/* libcollections.a
	rm libcollections.a

buildtest: bin/lib lib
	${CC} ${CFLAGS} main.c ${TESTSRCS} ${LIBOBJS} -o bin/test_${OUTPUT}

runtest:
	bin/test_${OUTPUT}

buildnrun: build run

build: bin/lib lib
	${CC} ${CFLAGS} ${LIBOBJS} -o bin/${OUTPUT}

run:
	bin/${OUTPUT}

lib: ${LIBOBJS}

${LIBOBJS}: bin/lib/%.o: src/%.c
	${CC} ${CFLAGS} -c $^ -o $@

clean:
	rm -rf bin/*

cleanlib:
	rm -f bin/lib/*

debug:
	echo -e "Library \033[31mobjects\033[30m: ${LIBOBJS}"
	echo -e "Library \033[32msources\033[30m: ${LIBSRCS}"
	echo -e "Library \033[35mheaders\033[30m: ${LIBHDRS}"
	mkdir -p bin

bin/lib:
	mkdir bin/lib

cc:
	${CC} --version
