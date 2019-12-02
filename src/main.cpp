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

void checkNext(int startLevel, bool easy, string basename) {

    int ch = getch();
    string systring = basename;
    systring += " --start-level ";
    systring += to_string(startLevel);

    if (easy) {
        systring += " --easy";
    }
    
    if ( ch == 'y' ) {
        system(systring.c_str());
    }
    else if ( ch == 'n' ) {
        return;
    }
    else {
        checkNext(startLevel, easy, basename);
    }
}

void game(Shape shape, Screen screen, int startLevel, bool easy, string basename) {

    // global game vars:
    // startlevel = 1; framerate = 25
    // startlevel = 18; frarate = 10
    int frameRate = 24;
    for ( int i = 0; i < startLevel; i++ ) {
        frameRate--;
    }

    bool newShape = true;
    int count = 0;
    unsigned int microseconds = 10000;

    screen.addStartLevel(startLevel);
    // in case the terminal doesnt support invis cursor
    int restingCursor[2] = { 23, 22 };


    while (!shape.gameover) {
        if ( screen.advancingLevel ) {
            if ( frameRate > 2 ) {
                frameRate--;
            }
            shape.colors = screen.colors;
            screen.advancingLevel = false;
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
        else if ( (count + 1) % frameRate == 0 ) {
            shape.checkDeath();
        }
        else if ( count % frameRate == 0) {
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
        bool breake = false;

        if ( ch ) {
            if ( ch == KEY_UP) {
                shape.rotate();
            }
            else if ( ch == KEY_DOWN) {
                shape.move(3);
            }
            else if ( ch == KEY_LEFT) {
                shape.move(1);
            }
            else if ( ch == KEY_RIGHT) {
                shape.move(2);
            }
            else if ( ch == ' ' ) {
                ground = true;
            }   
            else if ( ch == 'e' ) {
                if ( easy ) {
                    easy = false;
                }
                else {
                    easy = true;
                }
            }
            else if ( ch == 'r' ) {
                breake = true;
            }
        }

        if ( ground ) {
            shape.ground(frameRate);
        }

        if ( breake ) {
            break;
        }

        // draw the score to the screen
        // screen.updateScore(count);

        // move the cursor, print the screen,
        // and refresh the terminal
        wmove(stdscr,0,0);

        screen.draw();

        if (easy) {
            shape.showGround();
        }

        shape.draw();
        screen.top();


        // printw(shape.cs.c_str());
        wmove(stdscr,0,0);
        // wrefresh(stdscr);
        // increment the fraction of a block drop count
        count++;

        if ( shape.dead ) {
            newShape = true;
            screen.addShape(shape);
            if (screen.points()) {
                shape.colors = screen.colors;
            };
        }
    }
    
    // wait for keyinput
    nodelay(stdscr, FALSE);
    wrefresh(stdscr);

    // print the death screen
    for ( int i = 8; i < 12; i++  ) {
        for ( int j = 3; j < 23; j++ ) {
            mvprintw(i,j," ");
        }
    }

    mvprintw(11, 2, "┣");
    mvprintw(8, 2, "┣");

    for ( int i = 3; i < 24; i++ ) {
        mvprintw(8, i, "━");
        mvprintw(11, i, "━");
    }

    mvprintw(11, 23, "┫");
    mvprintw(8, 23, "┫");

    screen.top();

    mvprintw(9,8,string("Game over!").c_str());
    mvprintw(10,5,string("Try again? (y/n)").c_str());

    checkNext(startLevel, easy, basename);
}

int main(int argc, char ** argv) {

    // ifstream t("screen.txt");
    // stringstream buffer;
    // buffer << t.rdbuf();
    // string screenstr = buffer.str();

    string screenstr = "  ┏━━k-vernooy/tetris━━┓\n  ┃                    ┃\n  ┃                    ┃   ┏━━next━━━┓\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┗━━━━━━━━━┛\n  ┃                    ┃\n  ┃                    ┃   ┏━━score━━┓\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃  0      ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┗━━━━━━━━━┛\n  ┃                    ┃\n  ┃                    ┃   ┏━━lines━━┓\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃  0      ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┗━━━━━━━━━┛\n  ┗━━━━━━━━━━━━━━━━━━━━┛\n                 ";

    // Curses setup: window, color, keyinput
    setlocale(LC_CTYPE, "");
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
	start_color();
    use_default_colors();

    int startLevel = 8;
    bool easy = false;

    for ( int i = 1; i < argc; i++ ) {
        if ( argv[i] == string("--start-level") ) {
            startLevel = stoi(argv[i + 1]);
        }
        else if ( argv[i] == string("--easy")) {
            easy = true;
        }
    }

    // Initialize screen and global vars
    Screen screen(screenstr);

    // create the shape object
    Shape shape;

    string basename = argv[0];

    game(shape, screen, startLevel, easy, basename);


    endwin();
    
    // return 0;
}
