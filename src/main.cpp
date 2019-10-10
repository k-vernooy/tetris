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
    curs_set(0);

    init_pair(1,2,0);
    wattron(stdscr, COLOR_PAIR(2));

    Screen screen(screenstr);
    int score = 0;
    int count = 1;
    int frameRate = 10;
    bool newShape = true;

    Shape shape;

    while (true) {

        // if ( newShape ) {
        //     shape.generate();
        //     shape.drop();
        //     newShape = false;
        // }

        // if ( shape.isDropping && count % frameRate == 0) {
        //     shape.drop();
        // }

        if ( count % frameRate == 0 ) {
            // shape.fall(shapePos);
            // if ( shape.checkDie() ) {
                // //shape can no longer move
                // newShape = true;
            // };
        }

        // sleep for a fraction of a frame 
        // (fraction of a block drop) in order to allow moving during frame
        unsigned int microseconds = 20000;
        usleep(microseconds);

        wmove(stdscr,0,0);

        int ch = getch();

        if ( ch ) {
            switch (ch) {
                // case KEY_BACKSPACE: /* user pressed backspace */ 
                //     ...
                case KEY_UP:
                    shape.rotate();
                    break;
                case KEY_DOWN:
                    shape.move(3);
                    break;
                case ' ':
                    shape.ground(frameRate);
                    break;
            }
        }

        screen.updateScore(count);
        screen.draw();
        wrefresh(stdscr);

        // score += 50;
        count++;
    }
    endwin();

    return 0;
}
