#include <fstream>
#include <ncurses.h>
#include <cstring>
#include <random>
#include <algorithm>
#include <unistd.h>
#include "../include/tetris.hpp"

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


bool isNumber(const string& s) {
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
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

void Screen::top() {
    move(0,0);
    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < window[i].size(); j++) {
            printw(window[i][j].c_str());
        }
        printw(string("\n").c_str());
    }
}

void Screen::draw() {
    vector<int> colors = { COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_MAGENTA };

    for ( int i = 0; i < window.size(); i++ ) {
        for ( int j = 0; j < window[i].size(); j++) {
            string cur = window[i][j];
            if ( (( i >= 4 && i < 22 ) && ( j > 4 && j < 25)) || ( ( i >= 6 && i < 9 ) && ( j > 33 && j < 43) )) {
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

void Screen::addNext(vector<vector<bool> > shape, vector<int> color) {
    int x = 34;
    int y = 6;

    for ( int i = 0; i < 3; i++ ) {

        vector<bool> line = shape[i];
        for ( int j = 0; j < line.size(); j++ ) {
            // window[y + i][x + 2 * j] = "t";
            // window[y + i][x + 2 * j + 1] = "t";
            if ( line[j] ) {
                window[y + i][x + ( 2 * j )] = to_string(color[0]);
                window[y + i][x + ( 2 * j ) + 1] = to_string(color[1]);
            }
            else {
                window[y + i][x + ( 2 * j )] = " ";
                window[y + i][x + ( 2 * j ) + 1] = " ";               
            }
        }
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

void Screen::updateLines(int score) {
    int x = 19;
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

    // if ( fullLines.size() == 1) {
    //     window[0][0] = to_string(fullLines[0]);
    // }

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

void Screen::points() {
    vector<int> fullLines = pointCheck();
        
    int linesNew = fullLines.size();
    lines += linesNew;
    vector<int> points = { 40, 100, 300, 1200};

    int point;

    if ( fullLines.size() == 0 ) {
        point = 0;
    }
    else {
        point = points[fullLines.size() - 1];
    }

    score += point;
    updateScore(score);
    updateLines(lines);
    shiftLines(fullLines);
}

// basically a constructor to generate a new
// random shape and fill attributes:

Shape::Shape() {
    random_device rd; 
    mt19937 eng(rd()); 
    uniform_int_distribution<> distr(1, 7);

    int rand = distr(eng) - 1; 

    shapetype[0] = 0;
    shapetype[1] = rand;
}


void Shape::generate(vector<vector<string> > window) {
    dead = false;
    currentWin = window;
    trCoord[0] = 0;
    trCoord[1] = 9;

    shapetype[0] = shapetype[1];

    random_device rd; 
    mt19937 eng(rd()); 
    uniform_int_distribution<> distr(1, 7);

    int rand = distr(eng) - 1; 

    shapetype[1] = rand;

    nextUp = shapecoords[shapetype[1]];
    nextchars = chars[shapetype[1]];

    selected = shapecoords[shapetype[0]];
    chosenchars = chars[shapetype[0]];
    color = colors[shapetype[0]];       

     
    // selected = shapecoords[1];
    // color = colors[1];
    // find the height of the shape
    shapeHeight = 0;

    for ( int i = 0; i < 4; i++ ) {
        vector<bool> line = selected[i];
        bool found = false;

        for ( int x = 0; x < line.size(); x++ ) {
            if ( i == 1 ) {
                found = true;
            }
        }

        if ( found ) {
            shapeHeight++;
        }
    }

    defaultPos[0] = 0;
    defaultPos[1] = 2;

    // can prob add something here to center shape; ie shapeWidth;
    trCoord[0] -= shapeHeight;
    trCoord[0] -= 1;
    isdropping = shapeHeight + 2;

}

string Screen::getChar(int x, int y) {
    x += defaultPos[0];
    y += defaultPos[1];

    return window[x][y];
}

vector<int> Screen::pointCheck() {
    vector<int> fullLines;

    for ( int i = 4; i < 22; i++ ) {
        bool full = true;

        for ( int k = 5; k < 25; k++ ) {
            const char * chosen = window[i][k].c_str();
            for ( int i = 0; i < strlen(chosen); i++ ) {
                if ( !isdigit(chosen[i]) ) {
                    full = false;
                }
                if ( !full ) {
                    break;
                }
            }
            if ( !full ) {
                break;
            }
        }
        if ( full ) {
            fullLines.push_back(i);
        }
    }

    return fullLines;
}

void Screen::shiftLines(vector<int> lines) {

    for ( int i = 0; i < lines.size(); i++ ) {
        for ( int x = 5; x < 25; x++) {
            window[lines[i]][x] = " ";
        } 
    }

    for ( int i = 0; i < lines.size(); i++ ) {
        vector<string> line = window[lines[i]];
        for ( int j = lines[i] - 1; j > 4; j-- ) {
            for ( int x = 5; x < 25; x++ ) {
                window[j + 1][x] = window[j][x];
            }
        }
    }

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
        if ( check != " " && isNumber(check) ) {
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
    vector<vector<bool> > tester = selected;

    // Consider all squares one by one 
    for (int x = 0; x < 4 / 2; x++) { 
        for (int y = x; y < 4 - x - 1; y++) { 
            int temp = tester[x][y]; 
            tester[x][y] = tester[y][4-1-x]; 
            tester[y][4-1-x] = tester[4-1-x][4-1-y]; 
            tester[4-1-x][4-1-y] = tester[4-1-y][x]; 
            tester[4-1-y][x] = temp; 
        }
    }

    vector<int> coords = charCoords(tester);
    bool rotate = true;
    for ( int i = 0; i < coords.size(); i += 2 ) {
        string chosen = currentWin[coords[i] - 1][coords[i + 1] + 2];
        if ( chosen != " " ) {
            rotate = false;
        }
    }

    if ( rotate ) {
        selected = tester;
    }
    else {
        beep();
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
            if ( currentWin[coords[i] - 1][coords[i + 1]] != " " ) {
                move = false;
            }
        }

        if ( move ) {
            trCoord[1] -= 2;
        }
        else {
            //cannot move, error noise
            beep();
        }
    }
    else if ( movetype == 2 ) {
        // move rigjt
        for ( int i = 0; i < coords.size(); i += 2 ) {
            if ( currentWin[coords[i] - 1][coords[i + 1] + 4] != " " ) {
                move = false;
            }
        }

        if ( move ) {
            trCoord[1] += 2;
        }
        else {
            //cannot move, error noise
            beep();
        }
    }
    else if ( movetype == 3) {
        // move down
        for ( int i = 0; i < coords.size(); i += 2 ) {
            if ( currentWin[coords[i] + 1][coords[i + 1] + 2] != " " ) {
                move = false;
            }
        }

        if ( move ) {
            trCoord[0] += 1;
        }
    }
}

void Shape::ground(int framerate) {
    bool moveDown;
    while (moveDown) {
        vector<int> coords = charCoords(selected);
        for ( int i = 0; i < coords.size(); i += 2) {
            if ( currentWin[coords[i] + 1][coords[i + 1] + 2] != " " ) {
                moveDown = false;
            }
        }
        
        if (moveDown) {
            trCoord[0] += 1;
        }
    }
}