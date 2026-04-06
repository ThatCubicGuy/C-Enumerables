# Compiler settings
CC:=gcc
CFLAGS:=-Wextra -Iinclude
OUTPUT:=thing.exe

# Library files
LIBSRCS:=$(wildcard src/*.c)
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
	echo ${LIBOBJS}
	echo ${LIBSRCS}
	mkdir -p bin

bin/lib:
	mkdir bin/lib

cc:
	${CC} --version
