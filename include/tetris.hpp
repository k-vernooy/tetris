#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Screen {
    vector<vector<string> > window; 
    vector<string> shapes = {"1", "2", "3", "4", "5"};



    public:
        Screen(string test);
        void draw();
        void updateScore(int score);
};

class Shape {
    public:
    Shape();
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

    vector<vector<bool> > shape;
    void draw();
    void rotate();
};