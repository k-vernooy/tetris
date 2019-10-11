#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ncurses.h>
using namespace std;

class Screen {

    vector<vector<string> > window; 
    vector<string> shapes = {"1", "2", "3", "4", "5"};
    bool gameover = false;
    int defaultPos[2] = {4,4};

    public:
        Screen(string test);
        void draw();
        void updateScore(int score);
        void gameOver();
        string getChar(int x, int y);
};

class Shape {
    public:
    int defaultPos[2] = {2,4};
    vector<vector<bool> > selected;
    vector<vector<vector<bool> > > shapecoords = {
        {
            // the 'o' block
            {1,1,0,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            // the long block
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            // the l block
            {1,1,0,0},
            {1,0,0,0},
            {1,0,0,0},
            {0,0,0,0}
        },
        {
            // the j block
            {1,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            // the zag block
            {0,1,0,0},
            {1,1,0,0},
            {1,0,0,0},
            {0,0,0,0}
        },
        {
            // the zig block
            {1,0,0,0},
            {1,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            // the t block
            {0,1,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        }
    };

    unsigned int shapeHeight;

    bool dead = false;
    
    int trCoord[2] = { 0, 8 };
    int shapeRotation = 1;
    int isdropping;
    
    vector<int> colors = { COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_MAGENTA };
    int color;

    bool cannotMove;

    void generate();
    void draw();
    void drop();
    void fall();
    void rotate();
    void move(int movetype);
    void ground(int framerate);
};