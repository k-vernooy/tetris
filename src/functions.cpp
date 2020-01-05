
#include "../include/tetris.hpp"

using namespace std;

void checkNext(int startLevel, bool easy, string basename) {
    // reads user input for a (y/n) input,
    // restarts game or quits based on input

    int ch = getch();
    
    // create a string with cli options
    // for calling the program again
    string systring = basename;
    systring += " --start-level ";
    systring += to_string(startLevel);
    if (easy)
        systring += " --easy";
    
    // perform action based on input
    if ( ch == 'y' )
        system(systring.c_str()); // call binary
    else if ( ch == 'n' )
        return; // exit out of program
    else // did not input y or n, try again
        checkNext(startLevel, easy, basename);
}

string readLine(string str, int n) {
    // returns the nth line of a string
    stringstream f(str);
    string s;
    
    for (int i = 0; i < n; i++) {
        getline(f, s);
    }

    getline(f,s);
    return s; 
}

bool isNumber(const string& str) {
    // returns whether the input string is a number
    string::const_iterator it = str.begin();
    while (it != str.end() && isdigit(*it))
        ++it;

    // if iterator got to the end of the string 
    // and string is not empty, return true
    return !str.empty() && it == str.end();
}

int randNum(int start, int end) {
    // returns random number between start and end
    random_device rd; 
    mt19937 eng(rd()); 
    uniform_int_distribution<> distr(start, end);
    return distr(eng) - 1;  
}