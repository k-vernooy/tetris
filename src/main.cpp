#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ncurses.h>

#include "../include/tetris.hpp"

using namespace std;

int main(int argc, char ** argv) {

    ifstream t("screen.txt");
    stringstream buffer;
    buffer << t.rdbuf();
    string screenstr = buffer.str();

    setlocale(LC_CTYPE, "");
    initscr();

    Screen screen(screenstr);
    screen.draw();
    
    getch();
    endwin();

    return 0;
}
