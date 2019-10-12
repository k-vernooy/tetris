#include "../include/tetris.hpp"
#include <fstream>
#include <ncurses.h>
#include <cstring>
#include <random>
#include <algorithm>
#include <unistd.h>

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
    vector<int> colors = { COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_MAGENTA };

    for ( int i = 0; i < window.size(); i++ ) {
        for ( int j = 0; j < window[i].size(); j++) {
            string cur = window[i][j];

            if ( ( i >= 4 && i < 22 ) && ( j > 4 && j < 25)) {
                // cur will be num between 1 and 14;
                if ( cur != " ") {

                    int colorNum;
                    string printChar;

                    if (stoi(cur) % 2 == 0) {
                        colorNum = stoi(cur) / 2 - 1;
                        printChar = "▋";
                    }
                    else {
                        colorNum = floor(stoi(cur) / 2);
                        printChar = "█";
                    }

                    int color = colors[colorNum];

                    init_pair(colorNum + 3, color, -1);
                    attrset(COLOR_PAIR(colorNum + 3));

                    printw(printChar.c_str());

                    init_pair(1, COLOR_WHITE, -1);
                    attrset(COLOR_PAIR(1));

                    // init_pair(3, COLOR_MAGENTA, -1);
                    // attrset(COLOR_PAIR(3));
                    // printw(string("█").c_str());
                    // init_pair(1, COLOR_WHITE, -1);
                    // attrset(COLOR_PAIR(1));
                }
                else {
                    printw(string(" ").c_str());
                }
            }
            else {
                printw(window[i][j].c_str());
            }
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


void Screen::addShape(Shape shape) {

    for ( int i = 0; i < shape.selected.size(); i++ ) {
        for ( int x = 0; x < shape.selected[i].size(); x++ ) {
            if ( shape.selected[i][x] ) {
                window[i + shape.trCoord[0] + shape.defaultPos[1]][(2 * x - 1) + shape.trCoord[1] + shape.defaultPos[0] + 3] = to_string(shape.chosenchars[0]);
                window[i + shape.trCoord[0] + shape.defaultPos[1]][(2 * x) + shape.trCoord[1] + shape.defaultPos[0] + 3] = to_string(shape.chosenchars[1]);
            }
            else {
                // window[i + shape.trCoord[0] + shape.defaultPos[1]][x + shape.trCoord[1] + shape.defaultPos[0]] = "o";
            }
        }
    }
    // shape.trCoord[0] -= 1;
    // vector<int> coords = shape.charCoords(shape.selected);

    // for ( int i = 0; i <= coords.size(); i += 2) {
    //     // if ( coords[i] == 21) {
    //         window[21][coords[i + 1]] = string("o").c_str();
    //     // } 

    //     // window[21][5] = string("o").c_str();
    //     // window[coords[i]][coords[i + 1]] = string("o").c_str();
    // }
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
    chosenchars = chars[rand];
    color = colors[rand];        
    // selected = shapecoords[1];
    // color = colors[1];
    // find the height of the shape
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

    defaultPos[0] = 2;
    defaultPos[1] = 4;

    // can prob add something here to center shape; ie shapeWidth;
    defaultPos[1] -= shapeHeight + 1;
    isdropping = shapeHeight + 1;

}

string Screen::getChar(int x, int y) {
    x += defaultPos[0];
    y += defaultPos[1];

    return window[x][y];
}

vector<int> Screen::pointCheck() {
    vector<int> fullLines;

    for ( int i = 4; i < 21; i++ ) {
        bool full = true;
        for ( int j = 4; j < 23; j++ ) {
            if (window[i][j] != "█" && window[i][j] != "▋" ) {
                full = false;
                break;
            }
        }
        if ( full ) {
            fullLines.push_back(i);
        }
    }
}

void Screen::shiftLines(vector<int> lines) {

}

vector<int> Shape::charCoords(vector<vector<bool> > shape) {
    vector<int> coords;
    int currentPos[2] = { trCoord[0] + defaultPos[1] + 1, trCoord[1] + defaultPos[0]};
    // top  coord is pos[2]
    for ( int i = 0; i < shape.size(); i++ ) {
        for ( int j = 0; j < shape[i].size(); j++ ) {
            if ( shape[i][j] ) {
                
                int thisPos[2] = { currentPos[0] + i, currentPos[1] + ( 2 * j ) };
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
    vector<int> coords = charCoords(selected);

    for ( int i = 0; i < coords.size(); i = i + 2) {
        string check = currentWin[coords[i]][coords[i + 1]];
        if ( check == "█" || check == "▋" ) {
            cannotDrop = true;
        }
    }

    if ( cannotDrop ) {
        gameover = true;
    }
    else {
        trCoord[0]++;
        isdropping--;
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
            selected[x][y] = selected[y][4-1-x]; 
            selected[y][4-1-x] = selected[4-1-x][4-1-y]; 
            selected[4-1-x][4-1-y] = selected[4-1-y][x]; 
            selected[4-1-y][x] = temp; 
        }
    }


    // add a check here for if the rotate cannot be done
    // if ( rotate cannot be done ) {
        // do nothing
    //}
    // else {
        //change shape height
    //} 
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
                    mvprintw(currentPos[0], currentPos[1], string("█▋").c_str());
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
                    mvprintw(currentPos[0], currentPos[1], string("█▋").c_str());
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
    vector<int> coords = charCoords(selected);
    for ( int i = 0; i < coords.size(); i = i + 2) {
        string check = currentWin[coords[i]][coords[i + 1] + 3];
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

    vector<int> coords = charCoords(selected);
    
    bool move = true;

    if ( movetype == 1 ) {
        // move left
        for ( int i = 0; i < coords.size(); i += 2 ) {
            // check the box left of coords[i], coords[i + 1]
            if ( currentWin[coords[i]][coords[i + 1]] != " " ) {
                move = false;
            }
        }

        if ( move ) {
            trCoord[1] -= 2;
        }
    }
    else if ( movetype == 2 ) {
        // move rigjt
        for ( int i = 0; i < coords.size(); i += 2 ) {
            if ( currentWin[coords[i]][coords[i + 1] + 4] != " " ) {
                move = false;
            }
        }

        if ( move ) {
            trCoord[1] += 2;
        }
    }
    else if ( movetype == 3) {
        // move down
        for ( int i = 0; i < coords.size(); i += 2 ) {
            if ( currentWin[coords[i]][coords[i + 1]] != " " ) {
                move = false;
            }
        }

        if ( move ) {
            trCoord[0] += 1;
        } 

    }
}

void Shape::ground(int framerate) {

}