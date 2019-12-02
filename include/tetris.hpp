#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ncurses.h>

using namespace std;

class Shape {
    public:
    
    Shape();

    int defaultPos[2];
    int trCoord[2];

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
            {0,0,0,0,0},
            {0,0,1,0,0},
            {0,1,1,1,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
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
    vector<int> nextchars;

    vector<vector<string> > currentWin;
    vector<vector<bool> > nextUp;

    int shapeHeight = 0;
    int shapeRotation = 1;
    int isdropping;
    
    // vector<int> colors = { COLOR_WHITE, COLOR_WHITE, COLOR_BLUE, COLOR_RED, COLOR_RED, COLOR_BLUE, COLOR_WHITE };
    vector<int> colors = { COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_MAGENTA };
    int color;

    bool gameover = false;
    int shapetype[2];
    bool cannotMove = false;
    bool dead = false;

    vector<int> charCoords(vector<vector<bool> > shape);
    void generate(vector<vector<string> > window);
    void draw();
    void drop();
    void fall();
    void rotate();
    void move(int movetype);
    void ground(int framerate);
    void showGround();
    void groundDraw(int down);
    vector<int> groundCoords(vector<vector<bool> > shape, int down);
    void checkDeath();

};



class Screen {

    public: vector<vector<string> > window; 
    vector<string> shapes = {"1", "2", "3", "4", "5"};
    bool gameover = false;
    int defaultPos[2] = {1,1};
    vector<int> colors = { COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_MAGENTA };


    int score = 0;
    int lines = 0;
    int level = 0;
    int startLevel = 1;

    bool advancingLevel = false;
    public:
        void advanceLevel();
        void addStartLevel(int startlevel);
        Screen(string test);
        void draw();
        void updateScore(int score);
        void gameOver();
        string getChar(int x, int y);
        vector<vector<string> > getScr();
        void addShape(Shape shape);
        vector<int> pointCheck();
        void shiftLines(vector<int> lines);
        bool points();
        void updateLines(int score);
        void top();
        void addNext(vector<vector<bool> > shape, vector<int> color);
};
