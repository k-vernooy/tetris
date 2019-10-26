.PHONY: all

CC = g++
BINDIR = bin
newpath = ""

all: compile

compile:
	@mkdir -p ${BINDIR}
	@${CC} -std=c++11 src/main.cpp src/functions.cpp -lncurses -o ${BINDIR}/tetris

ubuntucompile:
	@mkdir -p ${BINDIR}
	@${CC} -std=c++11 src/main.cpp src/functions.cpp -lncursesw -o ${BINDIR}/tetris

install:
	@install -m 0755 ${BINDIR}/tetris /usr/local/bin

brewinstall:
	@mkdir -p ${newpath}/bin
	@install -m 0755 bin/tetris ${newpath}/bin/tetris