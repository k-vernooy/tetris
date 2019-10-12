#include "../include/tetris.hpp"
#include <fstream>
#include <ncurses.h>
#include <cstring>
#include <random>
#include <algorithm>

using namespace std;

string readLine(string str, int n) {
   stringstream f(str);
   string s;
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

vector<vector<string> > Screen::getScr() {
    return window;
}

void Screen::draw() {
    for ( int i = 0; i < window.size(); i++ ) {
        for ( int j = 0; j < window[i].size(); j++) {
            printw(window[i][j].c_str());
        }
        printw(string("\n").c_str());
    }
}

void Screen::updateScore(int score) {
    int x = 13;
    int y = 36;

    window[x][y] = to_string(score);

    for ( int z = 37; z < 42; z++ ) {
        window[x][z] = " ";
    }

    for ( int i = 1; i < to_string(score).length(); i++ ) {
        window[x][y + i] = "";
    }
}


void Screen::gameOver() {
    gameover = true;
    // window[9][12] = "die";
}

void Screen::addShape(Shape shape) {
    int pos[2] = { shape.trCoord[0] + shape.defaultPos[0], shape.trCoord[1] + shape.defaultPos[1] };

    vector<int> coords = shape.charCoords(shape.selected, pos);

    for ( int i = 0; i <= coords.size(); i += 2) {
        window[coords[i]][coords[i + 1]] = string("o").c_str();
        window[coords[i]][coords[i + 1] + 1] = string("o").c_str();
    }
}

// basically a constructor to generate a new
// random shape and fill attributes:
void Shape::generate(vector<vector<string> > window) {
    dead = false;
    currentWin = window;
    trCoord[0] = 0;
    trCoord[1] = 9;

    random_device rd; 
    mt19937 eng(rd()); 
    uniform_int_distribution<> distr(1, 7);

    int rand = distr(eng) - 1; 
    selected = shapecoords[rand];
    color = colors[rand];        
    cs = to_string(rand);
    // selected = shapecoords[1];
    // color = colors[1];
    // find the height of the shape
    cs += ", ";
    shapeHeight = 0;

    for ( int i = 0; i < 4; i++ ) {
        vector<bool> line = selected[i];
        bool found = false;

        for ( int x = 0; x < line.size(); x++ ) {
            if ( line[x] ) {
                found = true;
            }
        }

        if ( found ) {
            shapeHeight++;
        }
    }

    cs += to_string(shapeHeight);
    defaultPos[0] = 2;
    defaultPos[1] = 4;

    // can prob add something here to center shape; ie shapeWidth;
    defaultPos[1] -= shapeHeight;
    isdropping = shapeHeight;

}

string Screen::getChar(int x, int y) {
    x += defaultPos[0];
    y += defaultPos[1];

    return window[x][y];
}

vector<int> Shape::charCoords(vector<vector<bool> > shape, int pos[2]) {
    vector<int> coords;
    int currentPos[2] = { trCoord[0] + defaultPos[1] + 1, trCoord[1] + defaultPos[0]};
    // top right coord is pos[2]
    for ( int i = 0; i < shape.size(); i++ ) {
        for ( int j = 0; j < shape[i].size(); j++ ) {
            if ( shape[i][j] ) {
                
                int thisPos[2] = { currentPos[0] + i, currentPos[1] + ( 2 * j ) - 1 };
                coords.push_back(thisPos[0]);
                coords.push_back(thisPos[1]);
                // add this coord to the array
            }
        }
    }
    return coords;
}

void Shape::drop() {
    bool cannotDrop = false;
    // vector<int> coords = charCoords(selected, trCoord);
    // for ( int i = 0; i < coords.size(); i = i + 2) {
    //     string check = currentWin[coords[i]][coords[i + 1]];
    //     if ( check == "█" || check == "─" ) {
    //         cannotDrop = true;
    //     };
    // }

    if ( cannotDrop ) {
        // gameOver();
    }
    else {
        trCoord[0]++;
        isdropping--;
        mvprintw(0,0,to_string(isdropping).c_str());
    }

}

void Shape::rotate() {
    // function to rotate the matrix

    // Consider all squares one by one 
    for (int x = 0; x < 4 / 2; x++) { 
        // Consider elements in group of 4 in  
        // current square 
        for (int y = x; y < 4 - x - 1; y++) { 
            // store current cell in temp variable 
            int temp = selected[x][y]; 
  
            // move values from right to top 
            selected[x][y] = selected[y][4-1-x]; 
  
            // move values from bottom to right 
            selected[y][4-1-x] = selected[4-1-x][4-1-y]; 
  
            // move values from left to bottom 
            selected[4-1-x][4-1-y] = selected[4-1-y][x]; 
  
            // assign temp to left 
            selected[4-1-y][x] = temp; 
        } 
    } 

    shapeHeight = 0;
    for ( int row = 0; row < selected.size(); row++ ) {
        bool found = false;
        for ( int cell = 0; cell < selected[row].size(); cell++ ) {
            if (selected[row][cell] == 1) {
                found = true;
            }
        }
        if ( found ) {
            shapeHeight++;
        }
    }
}

void Shape::draw() {
    if ( isdropping > 0 ) {
        // need to cut off something;
        // left = isdropping
        int draw = shapeHeight - isdropping;
        vector<vector<bool> > relevant;

        for ( int i = isdropping; i <= draw; i++ ) {
            relevant.push_back(selected[i]);
        }

        int currentPos[2] = { trCoord[0] + defaultPos[1] + 1, trCoord[1] + defaultPos[0]};

        init_pair(2, color, -1);
        attrset(COLOR_PAIR(2));

        for ( int i = 1; i < draw; i++  ) {
            // for each line;
            vector<bool> line = relevant[i];
            for ( int i = 0; i < 4; i++ ) {
                // for each el in line;
                if ( line[i] ) {
                    // need to draw two side by side fullblocks;
                    mvprintw(currentPos[0], currentPos[1], string("██").c_str());
                }
                currentPos[1] += 2;
            }
            currentPos[0] += 1;
            currentPos[1] = trCoord[1] + defaultPos[0];
        }
    }
    else {
        int currentPos[2] = { trCoord[0] + defaultPos[1], trCoord[1] + defaultPos[0]};

        init_pair(2, color, -1);
        attrset(COLOR_PAIR(2));
        for ( int i = 0; i < selected.size(); i++  ) {
            // for each line;
            vector<bool> line = selected[i];
            for ( int i = 0; i < 4; i++ ) {
                // for each el in line;
                if ( line[i] ) {
                    // need to draw two side by side fullblocks;
                    mvprintw(currentPos[0], currentPos[1], string("██").c_str());
                }
                else {
                    mvprintw(currentPos[0], currentPos[1], string("").c_str());
                }
                currentPos[1] += 2;
            }
            currentPos[0] += 1;
            currentPos[1] = trCoord[1] + defaultPos[0];
        }
    }
    init_pair(1, COLOR_WHITE, -1);
    attrset(COLOR_PAIR(1));
}

void Shape::fall() {

    bool cannotFall = false;
    vector<int> coords = charCoords(selected, trCoord);
    for ( int i = 0; i < coords.size(); i = i + 2) {
        string check = currentWin[coords[i]][coords[i + 1]];
        if ( check != " " && check != "│" ) {
            cannotFall = true;
        };
    }
    // psuedocode:
    if (cannotFall) {
        dead = true;
    }
    else {
        trCoord[0]++;
    }
    // just move down the shape, no need to draw here
    // if !( spaceBelowAfterMoveDown ) {
    //  //cannot move down any more, incorporate this one into the board
    //  screen.drawshape(selected, trcoord[0], trcoord[1]);
    //}

}

void Shape::move(int movetype) {
    int currentPos[2] = { trCoord[0] + defaultPos[1], trCoord[1] + defaultPos[0]};

    vector<int> coords = charCoords(selected, currentPos);

    if ( movetype == 1 ) {
        // move left
        trCoord[1] -= 2;

    }
    else if ( movetype == 2 ) {
        // move right
        trCoord[1] += 2;
    }
    else if ( movetype == 3) {
        // move down
        
        trCoord[0] += 1;

    }
}

void Shape::ground(int framerate) {

}