// the tetris header

// including an unreasonable 
// amount of libraries:
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ncurses.h>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <random>
#include <cstring>
#include <algorithm>
#include <unistd.h>

using namespace std;

// function declarations
string readLine(string str, int n);
bool isNumber(const string& s);
void checkNext(int startLevel, bool easy, string basename);
int randNum(int start, int end);

// declaring classes for the shape and screen
class Shape {
    public: //! should probably make better use of encapsulation
        Shape(); // blank constructor

        // shape properties
        int defaultPos[2]; // the default position of a shape
        int trCoord[2]; // the current top right coordinate of the shape
        int shapeHeight = 0;

        vector<vector<bool> > selected; // the current selected shape
        vector<vector<bool> > nextUp; // the next shape
        
        vector<int> chosenchars; // the chosen set of print keys
        vector<int> nextchars;   // the next set of print keys

        vector<vector<vector<bool> > > shapecoords = { // a vector of shapes
            {   // the 'o' block
                {0,0,0,0},
                {0,1,1,0},
                {0,1,1,0},
                {0,0,0,0}
            },
            {   // the long block
                {0,0,0,0},
                {1,1,1,1},
                {0,0,0,0},
                {0,0,0,0}
            },
            {   // the l block
                {0,1,1,0},
                {0,1,0,0},
                {0,1,0,0},
                {0,0,0,0}
            },
            {   // the j block
                {0,1,1,0},
                {0,0,1,0},
                {0,0,1,0},
                {0,0,0,0}
            },
            {   // the zag block
                {0,0,1,0},
                {0,1,1,0},
                {0,1,0,0},
                {0,0,0,0}
            },
            {   // the zig block
                {0,1,0,0},
                {0,1,1,0},
                {0,0,1,0},
                {0,0,0,0}
            },
            {   // the t block
                {0,0,0,0,0},
                {0,0,1,0,0},
                {0,1,1,1,0},
                {0,0,0,0,0},
                {0,0,0,0,0},
            }
        };

        vector<vector<int> > chars = { // keys for printing each shape
            {1, 2},
            {3, 4},
            {5, 6},
            {7, 8},
            {9, 10},
            {11, 12},
            {13, 14}
        };

        vector<vector<string> > currentWin;
        
        // vector<int> colors = { COLOR_WHITE, COLOR_WHITE, COLOR_BLUE, COLOR_RED, COLOR_RED, COLOR_BLUE, COLOR_WHITE };
        vector<int> colors = { COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_MAGENTA };
        int color; // the chosen color in the array

        int shapetype[2];

        // game states
        bool gameover = false;
        bool cannotMove = false;
        bool dead = false;
        int shapeRotation = 1;
        int isdropping;

        // create a new shape
        void generate(vector<vector<string> > window);
        
        // methods for changing the shape positions
        void draw();
        void drop();
        void fall();
        void rotate();
        void move(int movetype);
        void ground(int framerate);
        vector<int> charCoords(vector<vector<bool> > shape);

        // methods for the ghost tile
        void showGround();
        void groundDraw(int down);
        vector<int> groundCoords(vector<vector<bool> > shape, int down);
        
        // updating game states
        void checkDeath();
};



class Screen {
    // game states
    // bool gameover = false;
    int defaultPos[2] = {1,1};

    // game stats
    int score = 0;
    int lines = 0;
    int level = 0;

    public:

        // game states
        int startLevel = 1;
        bool advancingLevel = false;

        // display variables
        const string screenstr = "  ┏━━k-vernooy/tetris━━┓\n  ┃                    ┃\n  ┃                    ┃   ┏━━next━━━┓\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┗━━━━━━━━━┛\n  ┃                    ┃\n  ┃                    ┃   ┏━━score━━┓\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃  0      ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┗━━━━━━━━━┛\n  ┃                    ┃\n  ┃                    ┃   ┏━━lines━━┓\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃  0      ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┗━━━━━━━━━┛\n  ┗━━━━━━━━━━━━━━━━━━━━┛\n                 ";
        vector<vector<string> > window; 
        vector<int> colors = { COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_MAGENTA };

        Screen(int startlevel); // constructor for generating an array and setting a start level

        // graphics methods for updating the screen
        void draw();
        void top();
        void updateIntDisplays(int score, int x, int y);
        string getChar(int x, int y); // returns string at coordinates of screen

        void addStartLevel(int startlevel); // update level variables
        void advanceLevel(); // level up the game

        // void gameOver();

        void addShape(Shape shape);
        vector<int> pointCheck();
        void shiftLines(vector<int> lines);
        bool points();
        void addNext(vector<vector<bool> > shape, vector<int> color);
};

void game(Shape shape, Screen screen, int startLevel, bool easy, string basename);