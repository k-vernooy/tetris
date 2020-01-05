#include "../include/tetris.hpp"

using namespace std;

int main(int argc, char ** argv) {

    // curses window setup
    setlocale(LC_CTYPE, "");
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
	start_color();
    use_default_colors();

    // set and modify user settings
    int startLevel = 8;
    bool easy = false;

    for ( int i = 1; i < argc; i++ ) {
        if ( argv[i] == string("--start-level") )
            startLevel = stoi(argv[i + 1]);
        else if ( argv[i] == string("--easy"))
            easy = true;
    }

    // initialize screen and shape
    Screen screen(startLevel);
    Shape shape;
    
    // record how the user is calling the program
    string basename = argv[0];
    
    // call mainloop
    game(shape, screen, startLevel, easy, basename);

    // curses cleanup
    endwin();
    return 0;
}
