#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ncurses.h>
#include <unistd.h>


#include "../include/tetris.hpp"

using namespace std;

int main(int argc, char ** argv) {

    ifstream t("screen.txt");
    stringstream buffer;
    buffer << t.rdbuf();
    string screenstr = buffer.str();

    setlocale(LC_CTYPE, "");
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    Screen screen(screenstr);
    int score = 0;
    while (true) {

        unsigned int microseconds = 20000;
        usleep(microseconds);

        wmove(stdscr,0,0);
        int ch = getch();

        screen.updateScore(score);
        screen.draw();
        // mvprintw(0,0,to_string(ch).c_str());
        wrefresh(stdscr);
        score++;
    }
    endwin();

    return 0;
}
