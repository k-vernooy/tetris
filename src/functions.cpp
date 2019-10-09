#include "../include/tetris.hpp"
#include <fstream>
#include <ncurses.h>
#include <cstring>

using namespace std;

string readLine(string str, int n) {
   stringstream f(str);
   string s;
   //for performance
//    s.reserve(some_reasonable_max_line_length);    

   //skip N lines
   for (int i = 0; i < n; i++) {
       getline(f, s);
   }

   getline(f,s);
   return s; 
}

Screen::Screen(string screenstr) {
    string line;
    stringstream f(screenstr);

    int lineNum = 0;

    while (getline(f, line)) {
        lineNum++;
    }

    for ( int x = 0; x < lineNum; x++ ) {
        vector<string> rowVec;
        string line = readLine(screenstr, x);
        for ( int i = 0; i < line.length(); i++ ) {
            rowVec.push_back(line.substr(i,1));
        }
        window.push_back(rowVec);
    }
}

void Screen::draw() {
    for ( int i = 0; i < window.size(); i++ ) {
        for ( int j = 0; j < window[i].size(); j++) {
            string temp = window[i][j];
            const char *character = temp.c_str();
            printw("%s", character);
        }

        string nl = "\n";
        const char *newline = nl.c_str();
        printw("%s",newline);
    }
}