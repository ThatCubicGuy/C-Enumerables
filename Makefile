# Compiler settings
CC:=gcc
CFLAGS:=-Wall -Iinclude
OUTPUT:=thing.exe

# Library files
LIBSRCS:=$(wildcard src/*.c)
LIBOBJS:=${LIBSRCS:src/%.c=bin/lib/%.o}

# Tests
TESTSRCS:=$(wildcard tests/*.c)

test: buildtest runtest

buildtest: lib
	${CC} ${CFLAGS} main.c ${TESTSRCS} ${LIBOBJS} -o bin/test_${OUTPUT}

runtest:
	bin/test_${OUTPUT}

buildnrun: build run

build: lib
	${CC} ${CFLAGS} ${LIBOBJS} -o bin/${OUTPUT}

run:
	bin/${OUTPUT}

lib: ${LIBOBJS}

${LIBOBJS}: bin/lib/%.o: src/%.c
	${CC} ${CFLAGS} -c $^ -o $@

clean:
	rm -f bin/*

cleanlib:
	rm -f bin/lib/*

debug:
	echo ${LIBOBJS}
	echo ${LIBSRCS}
	mkdir -p bin
	mkdir -p bin/lib
