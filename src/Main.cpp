/* Title: Copy Paste Game
 * File: Main.cpp
 *
 *  Created on: Apr 4, 2025
 *      Author: Mason Dunn
 */

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <stdlib.h>
#include "rang.hpp"
using namespace std;

// The Action Tokens

// Action token: Move widget down
struct MoveWidgetDown {
    char widgetType;
    int startX, startY, endX, endY;
    bool stillValid;
    MoveWidgetDown(char wT, int x, int y, bool sV) :
    widgetType(wT), startX(x), startY(y), endX(x), endY(y + 1), stillValid(sV) {}
    void print() {
        cout << "MoveWidgetDown{'" << widgetType << "', (" << startX << ", " << startY << "), (" << endX << ", " << endY << "), " << stillValid << "}" << endl;
    }
};
// Action token: Move widget left
struct MoveWidgetLeft {
    char widgetType;
    int startX, startY, endX, endY;
    bool stillValid;
    MoveWidgetLeft(char wT, int x, int y, bool sV) :
    widgetType(wT), startX(x), startY(y), endX(x - 1), endY(y), stillValid(sV) {}
    void print() {
        cout << "MoveWidgetLeft{'" << widgetType << "', (" << startX << ", " << startY << "), (" << endX << ", " << endY << "), " << stillValid << "}" << endl;
    }
};
// Action token: Move widget right
struct MoveWidgetRight {
    char widgetType;
    int startX, startY, endX, endY;
    bool stillValid;
    MoveWidgetRight(char wT, int x, int y, bool sV) :
    widgetType(wT), startX(x), startY(y), endX(x + 1), endY(y), stillValid(sV) {}
    void print() {
        cout << "MoveWidgetRight{'" << widgetType << "', (" << startX << ", " << startY << "), (" << endX << ", " << endY << "), " << stillValid << "}" << endl;
    }
};
// Action token: Move widget up
struct MoveWidgetUp {
    char widgetType;
    int startX, startY, endX, endY;
    bool stillValid;
    MoveWidgetUp(char wT, int x, int y, bool sV) :
    widgetType(wT), startX(x), startY(y), endX(x), endY(y - 1), stillValid(sV) {}
    void print() {
        cout << "MoveWidgetUp{'" << widgetType << "', (" << startX << ", " << startY << "), (" << endX << ", " << endY << "), " << stillValid << "}" << endl;
    }
};

// Function Prototypes

void addToCounter(int n, char counterType);
void charToColor(char inputChar);
void executeMoveDownToken(MoveWidgetDown downToken);
void executeMoveLeftToken(MoveWidgetLeft leftToken);
void executeMoveRightToken(MoveWidgetRight rightToken);
void executeMoveUpToken(MoveWidgetUp upToken);
void findPlayers();
void parseMoveWidgetDownVector();
void parseMoveWidgetLeftVector();
void parseMoveWidgetRightVector();
void parseMoveWidgetUpVector();
void playerTurn();
void printMap();
void setCharOnTheMap(int x, int y, char newChar);
void xStepsOnYInteraction(char x, char y); // TODO
// bool isDangerous(char); I'm leaving this commented out until I actually add hazardous widgets 
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

// I should probably make a function(s) that increments or decrements counters (or at least checks if they can)
// Maybe I should make one function for each counter, and the input of the function can be the number that I
// want to add to the counter (eg 1, -1, 2, etc...).

// Global Variables

const int PUSH_LIMIT = 5;
const char PLAYER = '@';
const char EMPTY_SPACE = '-';
const char WALL = '#';
const char NEW_ROW = ']';
const char COIN_COUNTER = 'C';
const char COIN = '$'; // This could be a '*' or a '$'.

int globalCoinCount = 0;
vector<pair<int, int>> playerCoordinates;

// Action token vectors

vector<MoveWidgetDown> vectorOfMoveWidgetDownTokens;
vector<MoveWidgetLeft> vectorOfMoveWidgetLeftTokens;
vector<MoveWidgetRight> vectorOfMoveWidgetRightTokens;
vector<MoveWidgetUp> vectorOfMoveWidgetUpTokens;

// The Game Map
vector<vector<char>> theMap;
int columns;
int rows;



// ===================================================================================================
// MAIN (top) 
// ===================================================================================================
int main() {
    // Get input from the player
    cout << PLAYER << EMPTY_SPACE << WALL << NEW_ROW << COIN_COUNTER << COIN << endl;
    cout << "Insert the map string, and then press [ENTER] Twice: \n";

    // Take the input, refine it, and then turn it into a game map.
    columns = 0;
    rows = 0;
    string mapString = getMapString();
    theMap = makeMapFromString(mapString);
    cout << "Just exited the makeMap function" << endl;
    parseMoveWidgetDownVector();

    // Print the map
    printMap();

    while (true) {
        playerTurn();
        printMap();
    }
}
// ===================================================================================================
// MAIN (bottom) 
// ===================================================================================================



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
    if (inputChar == PLAYER) { cout << rang::fg::cyan; }
    else if (inputChar == WALL) { cout << rang::fg::red; }
    else { cout << rang::fg::reset; }
}

// This actually *executes* a move token (does the logic check, updates the map)
void executeMoveDownToken(MoveWidgetDown downToken) {
    stack<MoveWidgetDown> moveTheseWidgets; // A stack of all of the widgets that are being pushed by downToken, as well as downToken.
    stack<MoveWidgetDown> clearTheStack; // std::stack has no clear member :( I must improvise.
    MoveWidgetDown currentToken = downToken;
    // bool keepLooping = true;

    while (true /*keepLooping*/) {
        moveTheseWidgets.push(currentToken);

        // I may want to turn these if's into an else-if chain.
        // Also, the cout statements will most-likely be temporary. Also, they will NOT be replaced
        // by throws, as these types of errors are common and normal in the game.

        // Check the starting position
        
        if (!isOnMap(currentToken.startX, currentToken.startY)) {
            // cout << "Cannot execute the move down token; The starting location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if (currentToken.widgetType == EMPTY_SPACE) {
            // cout << "Will not execute the move down token; The Widget being moved is an empty space!" << endl;
            moveTheseWidgets.pop();
            break;
        }

        // Check the ending position

        if (!isOnMap(currentToken.endX, currentToken.endY)) {
            // cout << "Cannot execute the move down token; The ending location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if ( xCanStepOnY(currentToken.widgetType, getFromTheMap(currentToken.endX, currentToken.endY)) ) {
            // Initially the above condition was `getFromTheMap(currentToken.endX, currentToken.endY) == EMPTY_SPACE`
            // I'll leave it here, commented out just in case I need it again.
            xStepsOnYInteraction(currentToken.widgetType, getFromTheMap(currentToken.endX, currentToken.endY));
            
            // We have reached the end of the chain, finish looping and continue with the funciton.
            break; 
        }
        if (!isPushable( getFromTheMap(currentToken.endX, currentToken.endY) )) {
            // cout << "Cannot execute the move down token; The ending location is occupied by a" << endl;
            // cout << "non-pushable widget!" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }

        // Pushing too much
        // The '+1' is there to include the original widget (downToken)
        // - - Update: Nvm, for some reason with the +1 it allows it to push 6 widgets instead of 5.
        if (moveTheseWidgets.size() > PUSH_LIMIT /* +1 */) {
            // cout << "Unfortunately this down token is pushing too much!!!" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        
        // If we get past all of that, create a token for the next widget in line.
        MoveWidgetDown nextToken(
            getFromTheMap(currentToken.endX, currentToken.endY),
            currentToken.endX,
            currentToken.endY,
            true
        );
        currentToken = nextToken;
    }

    while (moveTheseWidgets.size() > 0) {
        // Set the destination spot to be the widget that is moving.
        setCharOnTheMap(moveTheseWidgets.top().endX, moveTheseWidgets.top().endY, moveTheseWidgets.top().widgetType);
        // Set the original spot to be an EMPTY_SPACE.
        setCharOnTheMap(moveTheseWidgets.top().startX, moveTheseWidgets.top().startY, EMPTY_SPACE);
        
        moveTheseWidgets.pop();
    }
}

void executeMoveLeftToken(MoveWidgetLeft leftToken) {
    stack<MoveWidgetLeft> moveTheseWidgets;
    stack<MoveWidgetLeft> clearTheStack;
    MoveWidgetLeft currentToken = leftToken;

    while (true) {
        moveTheseWidgets.push(currentToken);

        // Check the starting position

        if (!isOnMap(currentToken.startX, currentToken.startY)) {
            // cout << "Cannot execute the move left token; The starting location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if (currentToken.widgetType == EMPTY_SPACE) {
            // cout << "Will not execute the move left token; The Widget being moved is an empty space!" << endl;
            moveTheseWidgets.pop();
            break;
        }

        // Check the ending position

        if (!isOnMap(currentToken.endX, currentToken.endY)) {
            // cout << "Cannot execute the move left token; The ending location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if ( xCanStepOnY(currentToken.widgetType, getFromTheMap(currentToken.endX, currentToken.endY)) ) {
            xStepsOnYInteraction(currentToken.widgetType, getFromTheMap(currentToken.endX, currentToken.endY));
            break; // We have reached the end of the chain, finish looping and continue with the funciton.
        }
        if (!isPushable( getFromTheMap(currentToken.endX, currentToken.endY) )) {
            // cout << "Cannot execute the move left token; The ending location is occupied by a" << endl;
            // cout << "non-pushable widget!" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }

        // Pushing too much
        if (moveTheseWidgets.size() > PUSH_LIMIT) {
            // cout << "Unfortunately this left token is pushing too much!!!" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        
        // If we get past all of that, create a token for the next widget in line.
        MoveWidgetLeft nextToken(
            getFromTheMap(currentToken.endX, currentToken.endY),
            currentToken.endX,
            currentToken.endY,
            true
        );
        currentToken = nextToken;
    }

    while (moveTheseWidgets.size() > 0) {
        // Set the destination spot to be the widget that is moving.
        setCharOnTheMap(moveTheseWidgets.top().endX, moveTheseWidgets.top().endY, moveTheseWidgets.top().widgetType);
        // Set the original spot to be an EMPTY_SPACE.
        setCharOnTheMap(moveTheseWidgets.top().startX, moveTheseWidgets.top().startY, EMPTY_SPACE);
        
        moveTheseWidgets.pop();
    }
}

void executeMoveRightToken(MoveWidgetRight rightToken) {
    stack<MoveWidgetRight> moveTheseWidgets;
    stack<MoveWidgetRight> clearTheStack;
    MoveWidgetRight currentToken = rightToken;

    while (true) {
        moveTheseWidgets.push(currentToken);

        // Check the starting position

        if (!isOnMap(currentToken.startX, currentToken.startY)) {
            // cout << "Cannot execute the move right token; The starting location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if (currentToken.widgetType == EMPTY_SPACE) {
            // cout << "Will not execute the move right token; The Widget being moved is an empty space!" << endl;
            moveTheseWidgets.pop();
            break;
        }

        // Check the ending position

        if (!isOnMap(currentToken.endX, currentToken.endY)) {
            // cout << "Cannot execute the move right token; The ending location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if ( xCanStepOnY(currentToken.widgetType, getFromTheMap(currentToken.endX, currentToken.endY)) ) {
            xStepsOnYInteraction(currentToken.widgetType, getFromTheMap(currentToken.endX, currentToken.endY));
            break; // We have reached the end of the chain, finish looping and continue with the funciton.
        }
        if (!isPushable( getFromTheMap(currentToken.endX, currentToken.endY) )) {
            // cout << "Cannot execute the move right token; The ending location is occupied by a" << endl;
            // cout << "non-pushable widget!" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }

        // Pushing too much
        if (moveTheseWidgets.size() > PUSH_LIMIT) {
            // cout << "Unfortunately this right token is pushing too much!!!" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        
        // If we get past all of that, create a token for the next widget in line.
        MoveWidgetRight nextToken(
            getFromTheMap(currentToken.endX, currentToken.endY),
            currentToken.endX,
            currentToken.endY,
            true
        );
        currentToken = nextToken;
    }

    while (moveTheseWidgets.size() > 0) {
        // Set the destination spot to be the widget that is moving.
        setCharOnTheMap(moveTheseWidgets.top().endX, moveTheseWidgets.top().endY, moveTheseWidgets.top().widgetType);
        // Set the original spot to be an EMPTY_SPACE.
        setCharOnTheMap(moveTheseWidgets.top().startX, moveTheseWidgets.top().startY, EMPTY_SPACE);
        
        moveTheseWidgets.pop();
    }
}

void executeMoveUpToken(MoveWidgetUp upToken) {
    stack<MoveWidgetUp> moveTheseWidgets;
    stack<MoveWidgetUp> clearTheStack;
    MoveWidgetUp currentToken = upToken;

    while (true) {
        moveTheseWidgets.push(currentToken);

        // Check the starting position

        if (!isOnMap(currentToken.startX, currentToken.startY)) {
            // cout << "Cannot execute the move up token; The starting location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if (currentToken.widgetType == EMPTY_SPACE) {
            // cout << "Will not execute the move up token; The Widget being moved is an empty space!" << endl;
            moveTheseWidgets.pop();
            break;
        }

        // Check the ending position

        if (!isOnMap(currentToken.endX, currentToken.endY)) {
            // cout << "Cannot execute the move up token; The ending location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if ( xCanStepOnY(currentToken.widgetType, getFromTheMap(currentToken.endX, currentToken.endY)) ) {
            xStepsOnYInteraction(currentToken.widgetType, getFromTheMap(currentToken.endX, currentToken.endY));
            break; // We have reached the end of the chain, finish looping and continue with the funciton.
        }
        if (!isPushable( getFromTheMap(currentToken.endX, currentToken.endY) )) {
            // cout << "Cannot execute the move up token; The ending location is occupied by a" << endl;
            // cout << "non-pushable widget!" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }

        // Pushing too much
        if (moveTheseWidgets.size() > PUSH_LIMIT) {
            // cout << "Unfortunately this up token is pushing too much!!!" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        
        // If we get past all of that, create a token for the next widget in line.
        MoveWidgetUp nextToken(
            getFromTheMap(currentToken.endX, currentToken.endY),
            currentToken.endX,
            currentToken.endY,
            true
        );
        currentToken = nextToken;
    }

    while (moveTheseWidgets.size() > 0) {
        // Set the destination spot to be the widget that is moving.
        setCharOnTheMap(moveTheseWidgets.top().endX, moveTheseWidgets.top().endY, moveTheseWidgets.top().widgetType);
        // Set the original spot to be an EMPTY_SPACE.
        setCharOnTheMap(moveTheseWidgets.top().startX, moveTheseWidgets.top().startY, EMPTY_SPACE);
        
        moveTheseWidgets.pop();
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

// This runs through the global `vectorOfMoveWidgetDownTokens` and executes each one via an execute function.
void parseMoveWidgetDownVector() {
    // This for-loop runs in reverse because the widgets at the end of the list are the ones
    // closest to the bottom of the map (due to the game parsing from left-to-right, top-to-bottom).
    //  For widgets that move down, we want the ones at the bottom to move first.
    // (See chronolog Friday October 3 2025)
    for (int i = vectorOfMoveWidgetDownTokens.size() - 1; i >= 0; i--) {
        executeMoveDownToken(vectorOfMoveWidgetDownTokens.at(i));
    }
}

void parseMoveWidgetLeftVector() {
    // In contrast to parseMoveWidgetDownVector(), this one parses from 
    // 0 to vectorOfMoveWidgetLeftTokens.size() 
    // (i.e. left-to-right) so that the leftmost widgets move first.
    // (See chronolog Friday October 3 2025)
    for (int i = 0; i < vectorOfMoveWidgetLeftTokens.size(); i++) {
        executeMoveLeftToken(vectorOfMoveWidgetLeftTokens.at(i));
    }
}

void parseMoveWidgetRightVector() {
    for (int i = vectorOfMoveWidgetRightTokens.size() - 1; i >= 0; i--) {
        executeMoveRightToken(vectorOfMoveWidgetRightTokens.at(i));
    }
}

void parseMoveWidgetUpVector() {
    for (int i = 0; i < vectorOfMoveWidgetUpTokens.size(); i++) {
        executeMoveUpToken(vectorOfMoveWidgetUpTokens.at(i));
    }
}

void playerTurn() {
    // clear and re-populate the vector of player coordinates
    playerCoordinates.clear();
    findPlayers();
    if (playerCoordinates.size() == 0){
        cout << "There are no players, so the player's turn will be skipped..." << endl;
        exit(EXIT_SUCCESS);    // This is temporary, and should generally be `return;`
    }

    cout << "It is now the player's turn.\n> ";
    string input;
    cin >> input;
    if (input == "w" || input == "W") {
        // cout << "The player is moving up" << endl;
        for (int i = 0; i < playerCoordinates.size(); i++) {
            MoveWidgetUp newRequest(
                PLAYER, playerCoordinates[i].first, playerCoordinates[i].second, true
            );
            vectorOfMoveWidgetUpTokens.push_back(newRequest);
        }
        parseMoveWidgetUpVector();
        vectorOfMoveWidgetUpTokens.clear();
    } else if (input == "a" || input == "A") {
        // cout << "The player is moving left" << endl;
        for (int i = 0; i < playerCoordinates.size(); i++) {
            MoveWidgetLeft newRequest(
                PLAYER, playerCoordinates[i].first, playerCoordinates[i].second, true
            );
            vectorOfMoveWidgetLeftTokens.push_back(newRequest);
        }
        parseMoveWidgetLeftVector();
        vectorOfMoveWidgetLeftTokens.clear();
    } else if (input == "s" || input == "S") {
        // cout << "The player is moving down" << endl;
        for (int i = 0; i < playerCoordinates.size(); i++) {
            MoveWidgetDown newRequest(
                PLAYER, playerCoordinates[i].first, playerCoordinates[i].second, true
            );
            vectorOfMoveWidgetDownTokens.push_back(newRequest);
        }
        parseMoveWidgetDownVector();
        vectorOfMoveWidgetDownTokens.clear();
    } else if (input == "d" || input == "D") {
        // cout << "The player is moving right" << endl;
        for (int i = 0; i < playerCoordinates.size(); i++) {
            MoveWidgetRight newRequest(
                PLAYER, playerCoordinates[i].first, playerCoordinates[i].second, true
            );
            vectorOfMoveWidgetRightTokens.push_back(newRequest);
        }
        parseMoveWidgetRightVector();
        vectorOfMoveWidgetRightTokens.clear();
    } else {
        cout << "Goobye Loser" << endl;
        exit(EXIT_SUCCESS); // This is probably temporray
    }
}

void printMap() {
    for (int rowI = 0; rowI < rows; rowI++) {
        for (int columnI = 0; columnI < columns; columnI++) {
            charToColor(theMap[rowI][columnI]);
            cout << theMap[rowI][columnI] << " ";
        }
        cout << endl;
    }
    cout << rang::fg::reset;
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

void xStepsOnYInteraction(char x, char y) {
    // NOTE: As of now, I am not planning on creating an action token when X steps on Y.
    // Just perform the action.
    
    // If a player steps on a coin.
    if (x == PLAYER && y == COIN) {
        addToCounter(1, COIN_COUNTER);
    }
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
