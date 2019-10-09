#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Screen {
    vector<vector<string> > window; 

    public:
        Screen(string test);
        void draw();
};