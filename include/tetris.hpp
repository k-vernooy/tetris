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
    vector<int> coords;
};