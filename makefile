.PHONY: all

CC = g++
BINDIR = bin

all: compile

compile:
	@${CC} -std=c++11 src/main.cpp src/functions.cpp -lncurses -o ${BINDIR}/tetris

install:
	@install -m 0755 ${BINDIR}/tetris /usr/local/bin