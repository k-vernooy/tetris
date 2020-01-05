#include "../include/tetris.hpp"

void game(Shape shape, Screen screen, int startLevel, bool easy, string basename) { 
    // the main gameloop
    
    // general game constants 
    int frameRate = 24;
    frameRate -= startLevel;

    bool newShape = true;
    int count = 0;
    unsigned int microseconds = 10000;
    
    // in case the terminal doesnt support invis cursor
    int restingCursor[2] = { 23, 22 };


    while (!shape.gameover) {

        if ( screen.advancingLevel ) { 
           // change the colors for the level
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
            shape.generate(screen.window);
            screen.addNext(shape.nextUp, shape.nextchars);
            shape.drop();
           
            if (shape.gameover) {
               break;
            }
          
            newShape = false;
        }
        else if ( (count + 1) % frameRate == 0 ) {
            // see if shape cannot fall anymore
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

        // sleep (for a fraction of a block drop) 
        // in order to allow moving during frame        
        usleep(microseconds);


        // Get keyinput for if user has input in the fraction of frame
        // Perform actions based on keycode
        int ch = getch();
        bool ground = false;
        bool breake = false;

        if ( ch ) {
            // handle different moves
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
            // turn on or off guide mode
            else if ( ch == 'e' ) {
                if ( easy ) {
                    easy = false;
                }
                else {
                    easy = true;
                }
            }
            // instant restart
            else if ( ch == 'r' ) {
                breake = true;
            }
        }

        // hard drop the shape
        if ( ground ) {
            shape.ground(frameRate);
        }

        // take the game to the death screen
        if ( breake ) {
            break;
        }

        // move the cursor, print the screen,
        screen.draw();

        if (easy) {
            shape.showGround();
        }

        shape.draw();
        screen.top();
        wmove(stdscr, 0, 0);

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

    // wait for input
    checkNext(startLevel, easy, basename);
}