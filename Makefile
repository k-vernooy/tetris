.PHONY: clean setup

CXX := g++
BIN := bin
BUILD := build
STDV := -std=c++11
CXXFLAGS := -O2 -MMD -MP 
OFLAGS := -O2 -MMD -MP 

OBJECTS := main.o shape.o screen.o game.o functions.o
OBJECT_OUTPUTS := $(patsubst %, $(BUILD)/%, $(OBJECTS))
DEPENDS := $(patsubst %.o,%.d,$(OBJECTS))

ifneq ($(OS),Windows_NT)
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CXXFLAGS += -lncursesw
    endif
    ifeq ($(UNAME_S),Darwin)
        CXXFLAGS += -lncurses
    endif
endif

main: setup $(OBJECT_OUTPUTS)
	$(CXX) $(OBJECT_OUTPUTS) $(STDV) $(CXXFLAGS) -o $(BIN)/tetris

$(BUILD)/%.o: src/%.cpp
	$(CXX) $(STDV) $(CXXFLAGS) -MT $@ $< -c -o $@

install:
	install -m 0755 $(BIN)/tetris /usr/local/bin

brewinstall: setup
	install -m 0755 $(BIN)/tetris $(newpath)/$(BIN)/tetris

clean:
	rm -rf build/ bin/

setup:
	mkdir -p $(BIN) $(BUILD) ${newpath}