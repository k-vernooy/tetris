#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ncurses.h>
using namespace std;


class Shape {
    public:
    int defaultPos[2] = {2,4};
    vector<vector<bool> > selected;
    vector<vector<vector<bool> > > shapecoords = {
        {
            // the 'o' block
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        {
            // the long block
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            // the l block
            {0,1,1,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            // the j block
            {0,1,1,0},
            {0,0,1,0},
            {0,0,1,0},
            {0,0,0,0}
        },
        {
            // the zag block
            {0,0,1,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            // the zig block
            {0,1,0,0},
            {0,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        },
        {
            // the t block
            {0,0,0,0},
            {0,1,0,0},
            {1,1,1,0},
            {0,0,0,0}
        }
    };

    vector<vector<int> > chars = {
        {1, 2},
        {3, 4},
        {5, 6},
        {7, 8},
        {9, 10},
        {11, 12},
        {13, 14}
    };
    vector<int> chosenchars;

    vector<vector<string> > currentWin;

    int shapeHeight = 0;
    int trCoord[2] = {0,9};
    int shapeRotation = 1;
    int isdropping;
    
    vector<int> colors = { COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_MAGENTA };
    int color;

    bool gameover = false;
    int shapetype;
    bool cannotMove;
    bool dead = false;

    vector<int> charCoords(vector<vector<bool> > shape);
    void generate(vector<vector<string> > window);
    void draw();
    void drop();
    void fall();
    void rotate();
    void move(int movetype);
    void ground(int framerate);
};

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
        vector<vector<string> > getScr();
        void addShape(Shape shape);
        vector<int> pointCheck();
        void shiftLines(vector<int> lines);
};
