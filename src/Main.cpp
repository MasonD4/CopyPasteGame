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
bool isDangerous(char);
bool isPushable(char);
bool isOnMap(int x, int y);
bool isThereAPlayer();
int determineNumber(char);
char getFromTheMap(int x, int y);
string getMapString();
vector<vector<char>> makeMapFromString(string);

// Global Variables

const int PUSH_LIMIT = 5;
const char PLAYER = '@';
const char EMPTY_SPACE = '-';
const char WALL = '#';
const char NEW_ROW = ']';
const char COIN_COUNTER = 'C';
const char JUMP_COUNTER = 'J';
const char COIN = '*'; // Maybe '$'
const char HAZARD = '!'; 
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

int main() {
    // Get input from the player
    cout << PLAYER << EMPTY_SPACE << WALL << NEW_ROW << COIN_COUNTER << JUMP_COUNTER << COIN << HAZARD << endl;
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
    // findPlayers();
    // cout << "There are players at:" << endl;
    // for (int i = 0; i < playerCoordinates.size(); i++) {
    //     cout << "(" << playerCoordinates[i].first << ", " << playerCoordinates[i].second << ")" << endl;
    // }
    // theMap[playerCoordinates[0].second][playerCoordinates[0].first] = EMPTY_SPACE;
    // theMap[playerCoordinates[0].second - 1][playerCoordinates[0].first + 1] = PLAYER;
    // cout << endl;
    // cout << endl;


    // printMap();
    // findPlayers();
    // cout << "There are players at:" << endl;
    // for (int i = 0; i < playerCoordinates.size(); i++) {
    //     cout << "(" << playerCoordinates[i].first << ", " << playerCoordinates[i].second << ")" << endl;
    // }
    // cout << endl;
    // cout << endl;


    while (true) {
        playerTurn();
        printMap();
    }
}
// Move template:
// move(vector<vector<char>>& map, int rows, int cols, int x, int y, string direction, int recursiveCount)

// void playerMove() {
//     string input;
//     cout << "Player turn: ";
//     cin
// }

// This actually *executes* a move token (does the logic check, updates the map)
void executeMoveDownToken(MoveWidgetDown downToken) {
    stack<MoveWidgetDown> moveTheseWidgets;
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
            cout << "Cannot execute the move down token; The starting location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if (currentToken.widgetType == EMPTY_SPACE) {
            cout << "Will not execute the move down token; The Widget being moved is an empty space!" << endl;
            moveTheseWidgets.pop();
            break;
        }

        // Check the ending position

        if (!isOnMap(currentToken.endX, currentToken.endY)) {
            cout << "Cannot execute the move down token; The ending location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if (getFromTheMap(currentToken.endX, currentToken.endY) == EMPTY_SPACE) {
            break; // We have reached the end of the chain, finish looping and continue with the funciton.
        }
        if (!isPushable( getFromTheMap(currentToken.endX, currentToken.endY) )) {
            cout << "Cannot execute the move down token; The ending location is occupied by a" << endl;
            cout << "non-pushable widget!" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }

        // Pushing too much
        // The '+1' is there to include the original widget (downToken)
        // - - Update: Nvm, for some reason with the +1 it allows it to push 6 widgets instead of 5.
        if (moveTheseWidgets.size() > PUSH_LIMIT /* +1 */) {
            cout << "Unfortunately this down token is pushing too much!!!" << endl;
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
            cout << "Cannot execute the move left token; The starting location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if (currentToken.widgetType == EMPTY_SPACE) {
            cout << "Will not execute the move left token; The Widget being moved is an empty space!" << endl;
            moveTheseWidgets.pop();
            break;
        }

        // Check the ending position

        if (!isOnMap(currentToken.endX, currentToken.endY)) {
            cout << "Cannot execute the move left token; The ending location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if (getFromTheMap(currentToken.endX, currentToken.endY) == EMPTY_SPACE) {
            break; // We have reached the end of the chain, finish looping and continue with the funciton.
        }
        if (!isPushable( getFromTheMap(currentToken.endX, currentToken.endY) )) {
            cout << "Cannot execute the move left token; The ending location is occupied by a" << endl;
            cout << "non-pushable widget!" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }

        // Pushing too much
        if (moveTheseWidgets.size() > PUSH_LIMIT) {
            cout << "Unfortunately this left token is pushing too much!!!" << endl;
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
            cout << "Cannot execute the move right token; The starting location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if (currentToken.widgetType == EMPTY_SPACE) {
            cout << "Will not execute the move right token; The Widget being moved is an empty space!" << endl;
            moveTheseWidgets.pop();
            break;
        }

        // Check the ending position

        if (!isOnMap(currentToken.endX, currentToken.endY)) {
            cout << "Cannot execute the move right token; The ending location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if (getFromTheMap(currentToken.endX, currentToken.endY) == EMPTY_SPACE) {
            break; // We have reached the end of the chain, finish looping and continue with the funciton.
        }
        if (!isPushable( getFromTheMap(currentToken.endX, currentToken.endY) )) {
            cout << "Cannot execute the move right token; The ending location is occupied by a" << endl;
            cout << "non-pushable widget!" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }

        // Pushing too much
        if (moveTheseWidgets.size() > PUSH_LIMIT) {
            cout << "Unfortunately this right token is pushing too much!!!" << endl;
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
            cout << "Cannot execute the move up token; The starting location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if (currentToken.widgetType == EMPTY_SPACE) {
            cout << "Will not execute the move up token; The Widget being moved is an empty space!" << endl;
            moveTheseWidgets.pop();
            break;
        }

        // Check the ending position

        if (!isOnMap(currentToken.endX, currentToken.endY)) {
            cout << "Cannot execute the move up token; The ending location is off the map!(tm)" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }
        if (getFromTheMap(currentToken.endX, currentToken.endY) == EMPTY_SPACE) {
            break; // We have reached the end of the chain, finish looping and continue with the funciton.
        }
        if (!isPushable( getFromTheMap(currentToken.endX, currentToken.endY) )) {
            cout << "Cannot execute the move up token; The ending location is occupied by a" << endl;
            cout << "non-pushable widget!" << endl;
            moveTheseWidgets = clearTheStack;
            break;
        }

        // Pushing too much
        if (moveTheseWidgets.size() > PUSH_LIMIT) {
            cout << "Unfortunately this up token is pushing too much!!!" << endl;
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
        cout << "The player is moving up" << endl;
        for (int i = 0; i < playerCoordinates.size(); i++) {
            MoveWidgetUp newRequest(
                PLAYER, playerCoordinates[i].first, playerCoordinates[i].second, true
            );
            vectorOfMoveWidgetUpTokens.push_back(newRequest);
        }
        parseMoveWidgetUpVector();
        vectorOfMoveWidgetUpTokens.clear();
    } else if (input == "a" || input == "A") {
        cout << "The player is moving left" << endl;
        for (int i = 0; i < playerCoordinates.size(); i++) {
            MoveWidgetLeft newRequest(
                PLAYER, playerCoordinates[i].first, playerCoordinates[i].second, true
            );
            vectorOfMoveWidgetLeftTokens.push_back(newRequest);
        }
        parseMoveWidgetLeftVector();
        vectorOfMoveWidgetLeftTokens.clear();
    } else if (input == "s" || input == "S") {
        cout << "The player is moving down" << endl;
        for (int i = 0; i < playerCoordinates.size(); i++) {
            MoveWidgetDown newRequest(
                PLAYER, playerCoordinates[i].first, playerCoordinates[i].second, true
            );
            vectorOfMoveWidgetDownTokens.push_back(newRequest);
        }
        parseMoveWidgetDownVector();
        vectorOfMoveWidgetDownTokens.clear();
    } else if (input == "d" || input == "D") {
        cout << "The player is moving right" << endl;
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
            cout << theMap[rowI][columnI] << " ";
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

bool isDangerous(char input) {
    switch(input) {
    case HAZARD: // temporary(?)
        return true;
        break;
    default: 
        return false;
    }
}

bool isPushable(char input) {
    if (input == WALL) { return false; }
    else if (input == NEW_ROW) { return false; }
    // Note: Air SHOULD be considered as pushable
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
        cout << "Oh no! Can't use getFromTheMap; The x-value is invalid!" << endl;
        throw string("Oh no! Can't use getFromTheMap; The x-value is invalid!");
    }
    if (y < 0 || y >= rows) {
        cout << "Oh no! Can't use getFromTheMap; The y-value is invalid!" << endl;
        throw string("Oh no! Can't use getFromTheMap; The y-value is invalid!");
    }
    return theMap[y][x];
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