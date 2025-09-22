/* Title: Copy Paste Game
 * File: Main.cpp
 *
 *  Created on: Apr 4, 2025
 *      Author: Mason Dunn
 */

#include <iostream>
#include <vector>
#include <string>
using namespace std;

// TODO: Make functions that can check and update counters.
void findPlayers();
void printMap();
bool isDangerous(char);
bool isMovable(char);
bool isThereAPlayer();
int determineNumber(char);
string getMapString();
vector<vector<char>> makeMapFromString(string);

const int PUSH_LIMIT = 5;
const char PLAYER = '@';
const char EMPTY_SPACE = '-';
const char WALL = '#';
const char NEW_ROW = ']';
const char COIN_COUNTER = 'C';
const char JUMP_COUNTER = 'J';
const char COIN = '*'; // Maybe '$'
const char HAZARD = '!'; 
// Lowercase letters will be like comments
vector<pair<int, int>> playerCoordinates;
// This boolean is because, if findPlayers gets called twice, I believe that it will add to the vector without
// removing what was already in there.
bool hasFindPlayersBeenCalled = false;

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

    // Print the map
    printMap();
    cout << "yay it worked!" << endl;
    cout << "Are there any players here?";
    if (isThereAPlayer() == true) { cout << " Yes!" << endl; }
    else { cout << " No!" << endl; } 
    cout << "How many? " << playerCoordinates.size() << "!";
}
// Move template:
// move(vector<vector<char>>& map, int rows, int cols, int x, int y, string direction, int recursiveCount)

// void playerMove() {
//     string input;
//     cout << "Player turn: ";
//     cin
// }

// Get player coordinates
void findPlayers() {
    if (hasFindPlayersBeenCalled == true) {
        playerCoordinates.clear();
    }

    for (int rowNumber = 0; rowNumber < rows; rowNumber++) {
        for (int colNumber = 0; colNumber < columns; colNumber++) {
            if (theMap[rowNumber][colNumber] == PLAYER) {
                playerCoordinates.push_back({colNumber, rowNumber});
            }
        }
    }
    hasFindPlayersBeenCalled = true;
}

void printMap() {
    for (int rowI = 0; rowI < rows; rowI++) {
        for (int columnI = 0; columnI < columns; columnI++) {
            cout << theMap[rowI][columnI] << " ";
        }
        cout << endl;
    }
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

bool isMovable(char input) {
    switch(input) {
    case WALL:
        return false;
        break;
    case NEW_ROW:
        return false;
        break;
    case COIN_COUNTER:
        return false;
        break;
    case JUMP_COUNTER:
        return false;
        break;
    default:
        return true;
    }
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