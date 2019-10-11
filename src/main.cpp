#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>

#include "../include/tetris.hpp"

using namespace std;

int main(int argc, char ** argv) {

    ifstream t("screen.txt");
    stringstream buffer;
    buffer << t.rdbuf();
    string screenstr = buffer.str();


    // Curses setup: window, color, keyinput
    setlocale(LC_CTYPE, "");
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
	start_color();
    use_default_colors();
	init_pair(1, COLOR_MAGENTA, -1);
    attrset(COLOR_PAIR(1));

    // Initialize screen and global vars
    Screen screen(screenstr);
    int score = 0;
    int count = 1;
    int frameRate = 10;
    bool newShape = true;

    // in case the terminal doesnt support invis cursor
    int restingCursor[2] = { 23, 22 };

    // create the shape object
    Shape shape;

    while (true) {

        if ( newShape && count % frameRate == 0) {
            // if we need to generate a new shape, do so;
            // begin dropping the new shape, so
            // we no longer need a new shape.
            shape.generate();
            shape.drop();
            newShape = false;
        }

        if ( shape.isdropping > 0 && count % frameRate == 0) {
            // if the shape is high enough that we need to 
            // keep dropping it, do so without worrying about user input
            shape.drop();
        }
        else if ( count % frameRate == 0 ) {
            // otherwise, make the shape fall down 1; check for death
            shape.fall();

            if ( shape.cannotMove ) {
                //shape can no longer move, so we need to 
                // generate another shape next iteration of the loop
                newShape = true;
            };
        }

        // sleep for a (fraction of a block drop) 
        // in order to allow moving during frame        
        int microseconds = 60000;
        usleep(microseconds);


        // Get keyinput for if user has input in the fraction of frame
        // Perform actions based on keycode
        int ch = getch();

        if ( ch ) {
            switch (ch) {
                case KEY_UP:
                    shape.rotate();
                    break;
                case KEY_DOWN:
                    shape.move(3);
                    break;
                case KEY_LEFT:
                    shape.move(1);
                    break;
                case KEY_RIGHT:
                    shape.move(2);
                    break;
                case ' ':
                    shape.ground(frameRate);
                    break;
            }
        }

        // draw the score to the screen
        screen.updateScore(count);

        // move the cursor, print the screen,
        // and refresh the terminal
        wmove(stdscr,0,0);
        screen.draw();
        shape.draw();
        wmove(stdscr,restingCursor[0],restingCursor[1]);
        // printw(to_string(shape.shapeHeight).c_str());
        wrefresh(stdscr);

        // increment the fraction of a block drop count
        count++;
    }
    endwin();

    return 0;
}
