/* Title: Copy Paste Game
 * File: Main.cpp
 * Branch: Orderoo
 *
 *  Created on: Apr 4, 2025
 *      Author: Mason Dunn
 */

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <stdlib.h>
#include <memory>
#include <thread> // This isn't needed for the game, I'm just using it for testing.
#include <chrono> // This isn't needed for the game, I'm just using it for testing.
#include "rang.hpp"
using namespace std;

// Widget token
struct widgetToken {
    char widgetType;
    int x, y;

    widgetToken()
        : widgetType('-'), x(0), y(0) {
    }
    widgetToken(char widgetTypeA, int xA, int yA)
        : widgetType(widgetTypeA), x(xA), y(yA) {
    }
};

enum class Direction {
    UP,
    DOWN,
    RIGHT,
    LEFT
};

// Function Prototypes

void addToCounter(int n, char counterType);
void charToColor(char inputChar);
void everythingElse();
void findPlayers();
void itsSoOver(int status);
void performMove(int x, int y, Direction direction);
void playerTurn();
void printMap();
void setCharOnTheMap(int x, int y, char newChar);
void widgetTroupe();
void xStepsOnYInteraction(char x, char y);
bool attemptMove(int x, int y, Direction direction);
bool attemptPush(int x, int y, Direction direction, int pushCount);
// bool isDangerous(char); I'm leaving this commented out until I actually add hazardous widgets 
bool canBash(char input);
bool canBeBashed(char input);
bool hasAgency(char input);
bool isNextToChar(int x, int y, char theChar);
bool isPushable(char);
bool isOnMap(int x, int y);
bool isThereAPlayer();
bool xCanStepOnY(char x, char y);
int clamp(int input);
int clamp(int input, int lower, int upper);
int determineNumber(char);
char getFromTheMap(int x, int y);
char numberToChar(int n);
string getMapString();
vector<vector<char>> makeMapFromString(string);
vector<widgetToken> gatherAgents();

// I should probably make a function(s) that increments or decrements counters (or at least checks if they can)
// Maybe I should make one function for each counter, and the input of the function can be the number that I
// want to add to the counter (eg 1, -1, 2, etc...).

// Global Variables

const char BAIT = '%';
const char BOMB = '&';
const char CHASER_DOWN = 'V';
const char CHASER_LEFT = '<';
const char CHASER_RIGHT = '>';
const char CHASER_STILL = 'X';
const char CHASER_UP = '^';
const char COIN = '$'; // This could be a '*' or a '$'.
const char COIN_COUNTER = 'C';
const char DOOR = 'D'; // Note: I'm going to say that doors don't chain.
const char EMPTY_SPACE = '-';
const char KEY = 'K';
const char NEW_ROW = ']';
const char PLAYER = '@';
const char ROOK = '!';
const char WALL = '#';
const int BASH_RANGE = 10;
const int BLAST_RADIUS = 2;
const int MAGNET_RANGE = 5;
const int PUSH_LIMIT = 5;

// int globalCoinCount = 0; // This may not be necessary.
vector<pair<int, int>> playerCoordinates;

// The Game Map
vector<vector<char>> theMap;
int columns;
int rows;

void addToCounter(int n, char counterType) {
    // This function will deposit n into the first number widget it sees.
    // If the first number widget can not fit n, then the function will
    // deposit what it can and then move onto the next number.
    // Once n has been fully deposited, the function quits. 
    // The function will not return anything.
    // It is basically the "setter" of the counters.
    // there will also be a "getter" function for the counters.
    
    // When looping through the map, 
    // when you check to see what the current character is,
    // use getFromTheMap(x, y).

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            char currentChar = getFromTheMap(x, y);
            int currentNum = determineNumber(currentChar); // Returns -1 if it's not a number.

            // If currentChar is a number 0-9 (IE not equal to -1) AND it is right next to the specified
            // counter:
            if (currentNum != -1 && isNextToChar(x, y, counterType) == true) {
                // If the current widget is a number, and it is next to the specified counter, update it.
                int newNum = clamp(currentNum + n);
                setCharOnTheMap(x, y, numberToChar(newNum));
                int difference = newNum - currentNum;
                n -= difference; // Works for both positive and negative n!
            }
            if (n == 0) { break; }
        }
        if (n == 0) { break; }
    }
}

void charToColor(char inputChar) {
    if (inputChar == PLAYER) { cout << rang::fg::green; }
    else if (inputChar == WALL) { cout << rang::bgB::black; }
    else if (inputChar == NEW_ROW) { cout << rang::bgB::black; }
    else if (inputChar == EMPTY_SPACE) { cout << rang::fgB::black; }
    else if (inputChar == COIN) { cout << rang::fg::yellow; }
    else if (inputChar == COIN_COUNTER) { cout << rang::fg::black << rang::bg::yellow; }
    else if (inputChar == ROOK) { cout << rang::fg::red; }
}

// This is the function that allows every widget besides the player takes its turn.
void everythingElse() {
    vector<widgetToken> agents;
    vector<widgetToken> imminentActors;
    
    agents = gatherAgents();

    // Loop through agents, and find any agents who can perform a bash.
    for (int i = 0; i < agents.size(); i++) {
        if ( canBash(agents.at(i).widgetType) ) {
            imminentActors.push_back(agents.at(i));
        }
    }
    
    for (const widgetToken& currentToken : agents) {
        cout << "{ Type: " << currentToken.widgetType << " }" << endl;
        cout << "{ Location: (" << currentToken.x << ", " << currentToken.y << ") }" << endl;
        cout << endl;
    }
}

// Get player coordinates
void findPlayers() { 
    playerCoordinates.clear();

    for (int rowNumber = 0; rowNumber < rows; rowNumber++) {
        for (int colNumber = 0; colNumber < columns; colNumber++) {
            if (theMap[rowNumber][colNumber] == PLAYER) {
                playerCoordinates.push_back({colNumber, rowNumber});
            }
        }
    }
}

void itsSoOver(int status) {
    rang::setControlMode(rang::control::Auto); // These may not be necessary.
    cout << rang::style::reset;
    exit(status);
}

void performMove(int x, int y, Direction direction) {
    char charA = getFromTheMap(x, y); // The widget that's moving.
    int deltaX = 0;
    int deltaY = 0;

    if (direction == Direction::DOWN) { deltaY++; } // Remember: an increase in y = down!
    else if (direction == Direction::LEFT) { deltaX--; }
    else if (direction == Direction::RIGHT) { deltaX++; }
    else if (direction == Direction::UP) { deltaY--; }
    else {
        cout << "performMove was given an invalid direction, somehow..." << endl;
        return;
    }

    char charB = getFromTheMap(x + deltaX, y + deltaY); // The char being stepped on.
    setCharOnTheMap(x + deltaX, y + deltaY, charA);
    setCharOnTheMap(x, y, EMPTY_SPACE);
    xStepsOnYInteraction(charA, charB);
}

void playerTurn() {
    // XXX under renovations
}

void printMap() {
    for (int rowI = 0; rowI < rows; rowI++) {
        for (int columnI = 0; columnI < columns; columnI++) {
            charToColor(theMap[rowI][columnI]);
            cout << theMap[rowI][columnI] << " ";
            cout << rang::style::reset;
        }
        cout << endl;
    }
}

void setCharOnTheMap(int x, int y, char newChar) {
    if (x < 0 || x >= columns) {
        cout << "Oh no! Can't use setCharOnTheMap; The x-value is invalid!" << endl;
        return;
    }
    if (y < 0 || y >= rows) {
        cout << "Oh no! Can't use setCharOnTheMap; The y-value is invalid!" << endl;
        return;
    }
    theMap[y][x] = newChar;
}

void widgetTroupe() {
    charToColor(WALL);
    cout << WALL;
    cout << rang::style::reset;
    charToColor(ROOK);
    cout << ROOK;
    cout << rang::style::reset;
    charToColor(COIN);
    cout << COIN;
    cout << rang::style::reset;
    charToColor(PLAYER);
    cout << PLAYER;
    cout << rang::style::reset;
    charToColor(EMPTY_SPACE);
    cout << EMPTY_SPACE;
    cout << rang::style::reset;
    charToColor(COIN_COUNTER);
    cout << COIN_COUNTER;
    cout << rang::style::reset;
    charToColor(NEW_ROW);
    cout << NEW_ROW;
    cout << rang::style::reset;
    cout << endl;
}

void xStepsOnYInteraction(char x, char y) {
    // NOTE: As of now, I am not planning on creating an action token when X steps on Y.
    // Just perform the action.
    
    // If a player steps on a coin.
    if (x == PLAYER && y == COIN) {
        addToCounter(1, COIN_COUNTER);
    }
}

// This function assumes that the specifics of a move have been figured out.
// Its job is to see if that move is possible. If so, it calls performMove() and
// returns true; Otherwise, it prevents the move and returns false.
bool attemptMove(int x, int y, Direction direction) {
    char movingWidget;
    try {
        movingWidget = getFromTheMap(x, y);
    } catch (...) { return false; } // Note: I might want to change this to true, it could cause issues when pushing widgets.
    
    // I'm not going to bother checking if it can move. For now anything can move
    // (that does NOT mean anything can be pushed)
}

// I'm leaving this commented out until I actually add hazardous widgets 
// bool isDangerous(char input) {
//     switch(input) {
//     case HAZARD: // temporary(?)
//         return true;
//         break;
//     default: 
//         return false;
//     }
// }

// Checks if a widget can perform the bash action.
bool canBash(char input) {
    if (input == ROOK) { return true; }
    else { return false; }
}

// Checks if a widget can be bashed.
bool canBeBashed(char input) {
    if (input == PLAYER) { return true; }
    else { return false; }
}

bool hasAgency(char widget) {
    if (widget == ROOK) { return true; }
    else { return false; }
}

bool isNextToChar(int x, int y, char theChar) {
    // When you check to see what each character is,
    // use getFromTheMap(x, y).

    // WARNING: this function does not allow you to check if (x, y) is next to a number. 
    // I might make another function that checks if (x, y) is next to a number.

    try {
        char aboveChar = getFromTheMap(x, y - 1);
        if (aboveChar == theChar) { return true; }
    } catch (...) { }
    try {
        char rightChar = getFromTheMap(x + 1, y);
        if (rightChar == theChar) { return true; }
    } catch (...) { }
    try {
        char belowChar = getFromTheMap(x, y + 1);
        if (belowChar == theChar) { return true; }
    } catch (...) { }
    try {
        char leftChar = getFromTheMap(x - 1, y);
        if (leftChar == theChar) { return true; }
    } catch (...) { }
    
    return false;
}

bool isPushable(char input) {
    // Note: Air SHOULD be considered as pushable, in the sense that if something tries to "push" it,
    // it won't encounter any resistence.
    if (input == WALL) { return false; }
    else if (input == NEW_ROW) { return false; }
    else if (input == COIN_COUNTER) { return false; }
    else { return true; }
}

bool isOnMap(int x, int y) {
    if (x < 0 || x >= columns) { return false; }
    if (y < 0 || y >= rows) { return false; }
    return true;
}

bool isThereAPlayer() {
    findPlayers();
    if (playerCoordinates.size() > 0) {
        return true;
    }
    else {
        return false;
    }
}

// Can one widget (x) step on another (y)
// IE, If widget x moves onto a space occupied by widget y,
// will widget y simply cease to exist?
bool xCanStepOnY(char x, char y) {
    if (y == EMPTY_SPACE) { return true; }
    if (x == PLAYER && y == COIN) { return true; }
    return false;
}

int clamp(int input) {
    return clamp(input, 0, 9);
}

int clamp(int input, int lower, int upper){
    if (input < lower) {
        return lower;
    } else if (input > upper) {
        return upper;
    } else {
        return input;
    }
}

int determineNumber(char c) {
    if (c == '0') {return 0;}
    else if (c == '1') {return 1;}
    else if (c == '2') {return 2;}
    else if (c == '3') {return 3;}
    else if (c == '4') {return 4;}
    else if (c == '5') {return 5;}
    else if (c == '6') {return 6;}
    else if (c == '7') {return 7;}
    else if (c == '8') {return 8;}
    else if (c == '9') {return 9;}
    else {return -1;}
}

char getFromTheMap(int x, int y) {
    if (x < 0 || x >= columns) {
        // cout << "Oh no! Can't use getFromTheMap; The x-value is invalid!" << endl;
        throw string("Oh no! Can't use getFromTheMap; The x-value is invalid!");
    }
    if (y < 0 || y >= rows) {
        // cout << "Oh no! Can't use getFromTheMap; The y-value is invalid!" << endl;
        throw string("Oh no! Can't use getFromTheMap; The y-value is invalid!");
    }
    return theMap[y][x];
}

char numberToChar(int n) {
    if (n == 0) {return '0';}
    else if (n == 1) {return '1';}
    else if (n == 2) {return '2';}
    else if (n == 3) {return '3';}
    else if (n == 4) {return '4';}
    else if (n == 5) {return '5';}
    else if (n == 6) {return '6';}
    else if (n == 7) {return '7';}
    else if (n == 8) {return '8';}
    else if (n == 9) {return '9';}
    else {return '-';}
}

string getMapString() {
    // Obtains player input. Each item in the vector is a line entered by the player
    // (by inputting a string and pressing [Enter] once).
    string inputString;
    vector<string> rowsOfInputs;
    do {
        getline(cin, inputString);
        if (inputString != "") {
            rowsOfInputs.push_back(inputString);
        }
    } while (inputString != "");

    string concatenatedString;
    string output;
    // Takes each line from the input (each item in the vector) and concatenates it all into 1 string.
    for (vector<string>::iterator i = rowsOfInputs.begin(); i != rowsOfInputs.end(); i++) {
        concatenatedString += *i;
    }
    // Removes spaces.
    for (int i = 0; i < concatenatedString.length(); i++) {
        if (concatenatedString[i] != ' ') {
            output += concatenatedString[i];
        }
    }
    cout << "The current refined-string is \"" << output << "\"" << endl;
    return output;
}

vector<vector<char>> makeMapFromString(const string input) {

    if (input.length() < 1) {
        return makeMapFromString("EMPTY]-----]--@--]-----]INPUT!");
    }

    rows++;
    int biggestRowLength = 1, currentRowLength = 1;
    // Count the length of each row and find the longest (IT WORKS!!!)
    for (int i = 0; i < input.length(); i++) {
        if ((input[i] == NEW_ROW || i == input.length() - 1) && currentRowLength > biggestRowLength) {
            biggestRowLength = currentRowLength;
            currentRowLength = 1;
        }
        else if ((input[i] == NEW_ROW || i == input.length() - 1) && currentRowLength <= biggestRowLength) {
            currentRowLength = 1;
        }
        else {
            currentRowLength++;
        }

        if (input[i] == NEW_ROW) {rows++;}
    }
    columns = biggestRowLength;

    // This actually inserts the characters into the vector
    vector<vector<char>> output;
    vector<char> newRowVector(columns, EMPTY_SPACE);
    int positionInInput = 0;
    bool reachedEndOfInput = false;

    for (int currentRow = 0; currentRow < rows; currentRow++) {
        output.push_back(newRowVector);
        if (reachedEndOfInput == true) { break; }

        for (int currentCol = 0; currentCol < columns; currentCol++) {
            // If it encounters a new row character prematurely
            if (input[positionInInput] == NEW_ROW && currentCol != columns - 1) { continue; }
            // If it reaches the end of the input string correctly
            else if (positionInInput == input.length() - 1) {
                output[currentRow][currentCol] =  input[positionInInput];
                reachedEndOfInput = true;
                break;
            }
            // Otherwise.
            else {
                output[currentRow][currentCol] =  input[positionInInput];
                positionInInput++;
            }
        }

    }

    return output;
}

// It's called "getherAgents" because it creates a vector of widget tokens, and each token
// corresponds to a widget with agency (the ability to act on its own).
vector<widgetToken> gatherAgents() {
    // Loop through the whole map and create a widget token for each widget *WITH AGENCY*.
    // Thus far, the only widget with agency is the rook (!).
    // Remember: Just because any pushable object can "move itself" when being pushed, doesn't necessarily mean it
    // has agency. The action it's taking is being initiated by something else.

    vector<widgetToken> agents;

    for (int y = 0; y < theMap.size(); y++) {
        for (int x = 0; x < theMap.at(y).size(); x++) {
            // Check to see if the current widget has (firsthand) agency
            char currentWidget = getFromTheMap(x, y);
            if (hasAgency(currentWidget)) {
                // cout << currentWidget << ", (" << x << ", " << y << ")." << endl;
                agents.emplace_back(currentWidget, x, y);
            }
        }
    }

    return agents;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// MAIN /////////////////////////////////////////////////////
int main() {
    rang::setControlMode(rang::control::Auto); // These may not be necessary.
    cout << rang::style::reset;
    
    // Get input from the player
    cout << "Enable color? (Y = yes): ";
    string colorful;
    getline(cin, colorful);
    if (!(colorful == "Y" || colorful == "y" || colorful == "Yes" || colorful == "yes")) {
        rang::setControlMode(rang::control::Off);
    }
    
    widgetTroupe();
    cout << "Insert the map string, and then press [ENTER] Twice: \n";

    // Take the input, refine it, and then turn it into a game map.
    columns = 0;
    rows = 0;
    // string mapString = getMapString(); // Temporarily commented this out for testing.
    string mapString = "C0-]-@-]-$-#";
    theMap = makeMapFromString(mapString);
    cout << "Just exited the makeMap function" << endl;

    // Print the map
    printMap();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    performMove(1, 1, Direction::RIGHT);
    printMap();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    performMove(2, 1, Direction::DOWN);
    printMap();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    performMove(2, 2, Direction::LEFT);
    printMap();
}
////////////////////////////////////////////////////////////////////////////////////////////////