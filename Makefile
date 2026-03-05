# this sucks lmao
CC:=gcc
CFLAGS:=-Wall
SRCS=$(wildcard *.c)
OUTPUT:=thing.exe

buildnrun: build run

build:
	${CC} ${CFLAGS} ${SRCS} -o bin/${OUTPUT}

run:
	bin/${OUTPUT}

clean:
	rm -rf bin/*