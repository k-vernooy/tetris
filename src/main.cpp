#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "../include/tetris.hpp"

using namespace std;

int main(int argc, char ** argv) {

    ifstream t("screen.txt");
    stringstream buffer;
    buffer << t.rdbuf();
    string screenstr = buffer.str();

    Screen screen(screenstr);
    
    screen.draw();
    return 0;
}