# this sucks lmao
INCLUDES:=$(foreach var,$(wildcard include/L*),${var:%=-I%})
CC:=gcc
CFLAGS:=-Wall -Iinclude
SRCS=main.c $(wildcard src/*.c)
OUTPUT:=thing.exe

test: buildtest runtest

buildtest:
	${CC} ${CFLAGS} ${SRCS} $(wildcard tests/*.c) -o bin/test_${OUTPUT}

runtest:
	bin/test_${OUTPUT}

buildnrun: build run

build:
	${CC} ${CFLAGS} ${SRCS} -o bin/${OUTPUT}

run:
	bin/${OUTPUT}

clean:
	rm -rf bin/*

debug:
	echo ${INCLUDES}
	echo ${SRCS}
