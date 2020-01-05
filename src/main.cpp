#include "../include/tetris.hpp"

using namespace std;

int main(int argc, char ** argv) {

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

    // Initialize screen and shape
    Screen screen;
    Shape shape;
    
    // how the user is calling the program
    string basename = argv[0];

    game(shape, screen, startLevel, easy, basename);

    endwin();
    return 0;
}
