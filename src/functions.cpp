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
                        printChar = "▌";
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
    int y = 3;

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

void whiteLines(vector<int> lines) {
    // if ( lines.size() > 0  ) {
    //     for ( int i = 0; i < lines.size(); i++ ) {
    //         for ( int x = 3; x < 23; x++) {
    //             mvprintw(lines[i], x, "██");
    //         }
    //     }
    //     // usleep(1250000);
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
    updateScore(score);
    updateLines(lines);
    whiteLines(fullLines);
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
    // change framerate?
    advancingLevel = true;
    // redefine colors
    colors = { COLOR_WHITE, COLOR_WHITE, COLOR_BLUE, COLOR_RED, COLOR_RED, COLOR_BLUE, COLOR_WHITE };
}

void Screen::addStartLevel(int startLevell) {
    startLevel = startLevell;
    level = startLevell;
};

// basically a constructor to generate a new
// random shape and fill attributes:
int randNum() {
    random_device rd; 
    mt19937 eng(rd()); 
    uniform_int_distribution<> distr(1, 7);

    return distr(eng) - 1;  
}


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

    shapetype[0] = shapetype[1];

    int rand = randNum();

    while ( rand == shapetype[0] ) {
        rand = randNum();
    }

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

    trCoord[0] = 0;
    trCoord[1] = 9;

    // can prob add something here to center shape; ie shapeWidth;
    trCoord[0] -= shapeHeight;
    trCoord[0] -= 2;
    isdropping = shapeHeight;

}

string Screen::getChar(int x, int y) {
    x += defaultPos[0];
    y += defaultPos[1];

    return window[x][y];
}

vector<int> Screen::pointCheck() {
    vector<int> fullLines;

    for ( int i = 1; i < 19; i++ ) {
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

    usleep(45000);
    move(0,0);
    refresh();

    if ( lines.size() > 0  ) {
        for ( int i = 0; i < lines.size(); i++ ) {
            for ( int x = 3; x < 23; x++) {
                mvprintw(lines[i],x,"█");
            } 
        }

        move(0,0);

        refresh();
        usleep(45000);

        
        draw();
        move(0,0);

        refresh();
        usleep(25000);

        for ( int i = 0; i < lines.size(); i++ ) {
            for ( int x = 3; x < 23; x++) {
                mvprintw(lines[i],x,"█");
            } 
        }
        move(0,0);

        refresh();
        usleep(45000);

    }

    for ( int i = 0; i < lines.size(); i++ ) {
        vector<string> line = window[lines[i]];
        for ( int j = lines[i] - 1; j > 0; j-- ) {
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
    // Need to allocate an array to store the transform
    vector<vector<bool> > temp;

    for ( int i = 0; i < selected.size(); i++ ) {
        vector<bool> row;
        for ( int j = 0; j < selected.size(); j++) {
            row.push_back(0);
        }
        temp.push_back(row);
    }

    int count = 0;        // count variable
    int count2 = 0;      // 2nd count variable

    for (count = 0; count < selected.size(); count++) {
        for (count2 = 0; count2 < selected[0].size(); count2++) {
            // Clockwise rotation
            // temp[count2][temp[0].size() - count - 1] = selected[count][count2];
            // Counter-clockwise rotation
            temp[temp.size() - count2 - 1][count] = selected[count][count2];
        }
    }


    vector<int> coords = charCoords(temp);
    bool rotate = true;
    for ( int i = 0; i < coords.size(); i += 2 ) {
        string chosen = currentWin[coords[i] - 1][coords[i + 1] + 2];
        if ( chosen != " " ) {
            rotate = false;
        }
    }

    if ( rotate ) {
        selected = temp;
    }
    else {
        // beep();
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

void Shape::draw( ) {
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
                mvprintw(currentPos[0], currentPos[1], string("█▌").c_str());
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
            // beep();
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
            // beep();
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
    bool moveDown = true;
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

vector<int> Shape::groundCoords(vector<vector<bool> > shape, int down) {
    vector<int> coords;
    int currentPos[2] = { trCoord[0] + defaultPos[1] + down, trCoord[1] + defaultPos[0]};
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

void Shape::groundDraw(int down)  {
    mvprintw(0,0,"5");
    int currentPos[2] = { trCoord[0] + defaultPos[1] + down, trCoord[1] + defaultPos[0]};

    init_pair(2, color, -1);
    attrset(COLOR_PAIR(2));
    for ( int i = 0; i < selected.size(); i++  ) {
        // for each line;
        vector<bool> line = selected[i];
        for ( int i = 0; i < 4; i++ ) {
            // for each el in line;
            if ( line[i] ) {
                // need to draw two side by side fullblocks;
                mvprintw(currentPos[0], currentPos[1], string("░░").c_str());
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

void Shape::showGround() {
    bool moveDown = true;
    int down = 0;
    while (moveDown) {
        vector<int> coords = groundCoords(selected, down);
        for ( int i = 0; i < coords.size(); i += 2) {
            if ( currentWin[coords[i] + 1][coords[i + 1] + 2] != " " ) {
                moveDown = false;
            }
        }
        
        if (moveDown) {
            down += 1;
        }
    }

    // down = 1;
    // down = 5;

    groundDraw(down);
}
