# this sucks lmao
CC:=gcc
CFLAGS:=-Wall
SRCS=$(wildcard *.c)
OUTPUT:=thing.exe

buildnrun: build run

build:
	${CC} ${CFLAGS} ${SRCS} -o ${OUTPUT}

run:
	./${OUTPUT}