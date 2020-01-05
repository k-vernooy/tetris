#include "../include/tetris.hpp"

Screen::Screen() {
    // creates an array from a string
    string line;
    stringstream f(screenstr);
    int lineNum = 0;

    while (getline(f, line)) {
        // get number of lines in string
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
    printw("  ┏━━k-vernooy/tetris━━┓       ");
}

void Screen::draw() {
    // colors = { COLOR_WHITE, COLOR_WHITE, COLOR_BLUE, COLOR_RED, COLOR_RED, COLOR_BLUE, COLOR_WHITE };
    for ( int i = 0; i < window.size(); i++ ) {
        for ( int j = 0; j < window[i].size(); j++) {
            string cur = window[i][j];
            if ( (( i >= 1 && i < 19 ) && ( j > 4 && j < 25)) || ( ( i >= 3 && i < 6 ) && ( j > 33 && j < 43) )) {
                // printw("*");
                // cur will be num between 1 and 14;

                if ( cur != " ") {

                    int colorNum;
                    string printChar;

                    if (stoi(cur) % 2 == 0) {
                        colorNum = stoi(cur) / 2 - 1;
                        printChar = "█";
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

void Screen::addNext(vector<vector<bool> > shape, vector<int> colors) {
    int x = 34;
    int y = 3;

    for ( int i = 0; i < 3; i++ ) {

        vector<bool> line = shape[i];
        for ( int j = 0; j < 4; j++ ) {
            if ( line[j] ) {
                window[y + i][x + ( 2 * j )] = to_string(colors[0]);
                window[y + i][x + ( 2 * j ) + 1] = to_string(colors[1]);
            }
            else {
                window[y + i][x + ( 2 * j )] = " ";
                window[y + i][x + ( 2 * j ) + 1] = " ";               
            }
        }
    }
}

void Screen::updateScore(int score) {
    int x = 10;
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
    int x = 16;
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

bool Screen::points() {
    vector<int> fullLines = pointCheck();
        
    int linesNew = fullLines.size();
    lines += linesNew;
    vector<int> points = { 40 * (level), 100 * (level), 300 * (level), 1200 * level};

    int point;

    if ( fullLines.size() == 0 ) {
        point = 0;
    }
    else {
        point = points[fullLines.size() - 1];
    }

    score += point;
    // print the stats
    updateScore(score);
    updateLines(lines);

    // move the lines
    shiftLines(fullLines);
    int test = level - startLevel;

    if ( level == startLevel ) {
        if ( lines > startLevel * 10 + 10) {
            window[0][0] = to_string(0);
            advanceLevel();
            return true;
        }
    }
    else if ( (lines - startLevel * 10 + 10) - (test * 10) >= 10 ) {
            window[0][0] = to_string(0);
            advanceLevel();
            return true;
    }
   return false;
}

void Screen::advanceLevel() {
    level++;
    advancingLevel = true;
    // redefine colors
    colors = { COLOR_WHITE, COLOR_WHITE, COLOR_BLUE, COLOR_RED, COLOR_RED, COLOR_BLUE, COLOR_WHITE };
}

void Screen::addStartLevel(int startLevell) {
    startLevel = startLevell;
    level = startLevell;
};