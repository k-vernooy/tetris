
#include "../include/tetris.hpp"

using namespace std;

void checkNext(int startLevel, bool easy, string basename) {

    int ch = getch();
    string systring = basename;
    systring += " --start-level ";
    systring += to_string(startLevel);

    if (easy) {
        systring += " --easy";
    }
    
    if ( ch == 'y' ) {
        system(systring.c_str());
    }
    else if ( ch == 'n' ) {
        return;
    }
    else {
        checkNext(startLevel, easy, basename);
    }
}

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

int randNum() {
    // returns random number between one and 7
    random_device rd; 
    mt19937 eng(rd()); 
    uniform_int_distribution<> distr(1, 7);

    return distr(eng) - 1;  
}