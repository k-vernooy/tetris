#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include "../include/tetris.hpp"

using namespace std;

void checkNext() {
    int ch = getch();

    if ( ch == 'y' ) {
        system("tetris");
    }
    else if ( ch == 'n' ) {
        return;
    }
    else {
        checkNext();
    }
}

void game(Shape shape, Screen screen) {

    // global game vars:
    int frameRate = 20;
    bool newShape = true;
    int count = 0;
    unsigned int microseconds = 10000;

    // in case the terminal doesnt support invis cursor
    int restingCursor[2] = { 23, 22 };


    while (!shape.gameover) {

        if ( shape.dead ) {
            newShape = true;
            screen.addShape(shape);
            screen.points();

            /// TODO HERE:
            // ADD A CHECK TO SEE IF WE CAN STILL 
            // MOVE DOWN, MOVING AFTER DEATH - 

        }
        if ( newShape ) {
            // if we need to generate a new shape, do so;
            // begin dropping the new shape, so
            // we no longer need a new shape.
            shape.generate(screen.getScr());
            screen.addNext(shape.nextUp, shape.nextchars);
            shape.drop();
            if (shape.gameover) {
                break;
            }
            newShape = false;
        }
        else if ( count % frameRate == 0) {
            // speed up:
            // if ( frameRate > 2 ) {
            //     frameRate--;
            // }
            // else {
            //     frameRate = 1;
            // }

            // if the shape is still dropping
            if (shape.isdropping > 0) {
                // if the shape is high enough that we need to 
                // keep dropping it, do so without worrying about user input
                shape.drop();
            }
            else {
                // otherwise, make the shape fall down 1; check for death
                shape.fall();

                if ( shape.cannotMove ) {
                    //shape can no longer move, so we need to 
                    // generate another shape next iteration of the loop
                    newShape = true;
                }
            }
        }

        // sleep for a (fraction of a block drop) 
        // in order to allow moving during frame        
        usleep(microseconds);


        // Get keyinput for if user has input in the fraction of frame
        // Perform actions based on keycode
        int ch = getch();
        bool ground = false;
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
                    ground = true;
                    break;
            }
        }

        if ( ground ) {
            shape.ground(frameRate);
        }

        // draw the score to the screen
        // screen.updateScore(count);

        // move the cursor, print the screen,
        // and refresh the terminal
        wmove(stdscr,0,0);

        screen.draw();
        shape.draw();
        screen.top();
        // printw(shape.cs.c_str());
        wmove(stdscr,restingCursor[0],restingCursor[1]);
        // wrefresh(stdscr);
        // increment the fraction of a block drop count
        count++;
    }
    nodelay(stdscr, FALSE);

    wrefresh(stdscr);
    for ( int i = 11; i < 15; i++  ) {
        for ( int j = 3; j < 23; j++ ) {
            mvprintw(i,j," ");
        }
    }

    mvprintw(14, 2, "├");
    mvprintw(11, 2, "├");

    for ( int i = 3; i < 24; i++ ) {
        mvprintw(11, i, "━");
        mvprintw(14, i, "━");
    }

    mvprintw(14, 23, "┤");
    mvprintw(11, 23, "┤");

    screen.top();

    mvprintw(12,8,string("Game over!").c_str());
    mvprintw(13,5,string("Try again? (y/n)").c_str());

    checkNext();
}

int main(int argc, char ** argv) {

    // ifstream t("screen.txt");
    // stringstream buffer;
    // buffer << t.rdbuf();
    // string screenstr = buffer.str();

    string screenstr = " ___ ____ ___ ____ _ ____ \n  |  |___  |  |__/ | [__  \n  |  |___  |  |  \\ | ___] \n  ┏━━━━━━━━━━━━━━━━━━━━┓      \n  ┃                    ┃   ┏━━━━━━━━━┓     \n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┗━━━━━━━━━┛   \n  ┃                    ┃   \n  ┃                    ┃   ┏━━━━━━━━━┓\n  ┃                    ┃   ┃  Score: ┃\n  ┃                    ┃   ┃    0    ┃\n  ┃                    ┃   ┃         ┃   \n  ┃                    ┃   ┗━━━━━━━━━┛\n  ┃                    ┃   \n  ┃                    ┃   ┏━━━━━━━━━┓   \n  ┃                    ┃   ┃  Line:  ┃    \n  ┃                    ┃   ┃   0     ┃   \n  ┃                    ┃   ┃         ┃   \n  ┃                    ┃   ┗━━━━━━━━━┛\n  ┗━━━━━━━━━━━━━━━━━━━━┛ \n    k-vernooy/tetris\n";

    // Curses setup: window, color, keyinput
    setlocale(LC_CTYPE, "");
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
	start_color();
    use_default_colors();


    // Initialize screen and global vars
    Screen screen(screenstr);

    // create the shape object
    Shape shape;

    game(shape, screen);


    endwin();
    
    // return 0;
}
