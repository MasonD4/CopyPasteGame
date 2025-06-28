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

void printMap(const vector<vector<char>>&, int, int);
vector<vector<char>> makeMapFromString(string, int&, int&);
string refineMapString(vector<string>);
bool isThereAPlayer(vector<vector<char>>);
int determineNumber(char);
void findPlayers(vector<vector<char>>, int, int);
bool move(vector<vector<char>>&, int, int, int, int, string, int);
bool isMovable(char);

const char PLAYER = '@';
const char EMPTY_SPACE = '-';
const char WALL = '#';
const char NEW_ROW = ']';
const char COIN_COUNTER = 'C';
const char JUMP_COUNTER = 'J';
// Lowercase letters will be like comments
vector<pair<int, int>> playerCoordinates;

int main() {
    // vector<vector<char>> map = {{'#', '#', '-', '-', ']'}, 
    //                             {'#', '-', '-', '-', ']'}, 
    //                             {'-', '-', '@', '-', ']'}, 
    //                             {'#', '-', '-', '-', ']'}, 
    //                             {'#', '#', '-', '-', ']'}};

    // TODO: Organize functions

    // Get input from the player
    cout << PLAYER << EMPTY_SPACE << WALL << NEW_ROW << endl;
    cout << "Insert the map string, and then press [ENTER] Twice: \n";
    string tempInputString;
    vector<string> tempInputVector;
    do {
        // Weed out the spaces and '\n's.
        getline(cin, tempInputString);
        if (tempInputString != "") {
            tempInputVector.push_back(tempInputString);
        }
    } while (tempInputString != "");

    // Take the input, refine it, and then turn it into a game map.
    int columns = 0;
    int rows = 0;
    string mapString = refineMapString(tempInputVector);
    vector<vector<char>> map = makeMapFromString(mapString, rows, columns);
    cout << "Just exited the makeMap function" << endl;

    // Print the map
    printMap(map, rows, columns);
    cout << "yay it worked!" << endl;
    move(map, rows, columns, 2, 2, "up", 0);
    printMap(map, rows, columns);
}

// void playerMove() {
//     string input;
//     cout << "Player turn: ";
//     cin
// }

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

string refineMapString(vector<string> input) {
    string tempString;
    string output;
    for (vector<string>::iterator i = input.begin(); i != input.end(); i++) {
        tempString += *i;
    }
    for (int i = 0; i < tempString.length(); i++) {
        if (tempString[i] != ' ') {
            output += tempString[i];
        }
    }
    cout << "The current refined-string is \"" << output << "\"" << endl;
    return output;
}

vector<vector<char>> makeMapFromString(const string input, int& rows, int& columns) {

    if (input.length() < 1) {
        return makeMapFromString("EMPTY]-----]--@--]-----]INPUT!", rows, columns);
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

void findPlayers(vector<vector<char>> input, int rows, int columns) {
    for (int rowNumber = 0; rowNumber < rows; rowNumber++) {
        for (int colNumber = 0; colNumber < columns; colNumber++) {
            if (input[rowNumber][colNumber] == '@') {
                playerCoordinates.push_back({colNumber, rowNumber});
            }
        }
    }
}

bool isThereAPlayer(vector<vector<char>> input) {
    for (vector<char> theRow : input) {
        for (char theChar : theRow) {
            if (theChar == '@') {return true;}
        }
    }

    return false;
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

bool isDangerous(char input) {
    switch(input) {
    case '!': // temporary
        return true;
        break;
    default: 
        return false;
    }
}

bool move(vector<vector<char>>& map, int rows, int cols, int x, int y, string direction, int recursiveCount) {
    // The recursive limit will be set in here, which means that external calls start at 0.
    const int RECURSIVE_LIMIT = 5;
    // In each of the if statements that call this function, they need to check to see if this threshold has been
    // reached.
    
    if (recursiveCount >= RECURSIVE_LIMIT) {
        return false;
    }
    if (x < 0 || y < 0 || x >= cols || y >= rows) {
        cout << "tried to move from a non-existant spot." << endl;
        return false;
    }
    else if (isMovable(map[y][x]) == false) {
        return false;
    }
    else if (map[y][x] == EMPTY_SPACE) {
        return true; // Maybe change this.
    }
    
    int newX = x;
    int newY = y;
    if (direction == "up") { newY--; }
    else if (direction == "right") { newX++; }
    else if (direction == "down") { newY++; }
    else if (direction == "left") { newX--; }
    else {
        cout << "BAD DIRECTION: " << direction << endl;
        return false;
    }
    
    // Space: Free move [DONE]
    // Walls: No move [DONE]
    // General movables: Recursive call. Can be pushed under certain circumstances.
    // Enemy: Can be moved, but if a player moves into an enemy, player dies.
    // Player: Can be moved, but if an enemy moves into a player, player dies.

    // Literal "edge" case, haha! 
    if (newX < 0 || newY < 0 || newX >= cols || newY >= rows) {
        cout << "Attempting to move off the edge... FAILED" << endl;
        return false;
    }
    // Moving into a wall/immovable
    else if (isMovable(map[newY][newX]) == false) {
        cout << "Tried to move into a wall/immovable. Lol no." << endl;
        return false;
    }
    // Moving into an already empty space.
    else if (map[newY][newX] == EMPTY_SPACE) {
        map[newY][newX] = map[y][x];
        map[y][x] = EMPTY_SPACE;
        return true;
    }
    // Player moves to a dangerous spot
    else if (map[y][x] == PLAYER && isDangerous(map[newY][newX]) == true) {
        map[y][x] == EMPTY_SPACE;
        return true;
    }
    // An enemy/dangerous object moves onto a player
    else if (isDangerous(map[y][x]) == true && map[newY][newX] == PLAYER) {
        map[newY][newX] = map[y][x];
        map[y][x] = EMPTY_SPACE;
        return true;
    }
    // Player pushes a movable object
    else if (isMovable(map[newY][newX]) == true) {
        if (move(map, rows, cols, newX, newY, direction, recursiveCount + 1) == true) {
            map[newY][newX] = map[y][x];
            map[y][x] = EMPTY_SPACE;
            return true;
        }
        else {
            return false;
        }
    }
    else { return false; }
}

// TODO Jump function

void printMap(const vector<vector<char>>& MAP, int rows, int columns) {
    for (int rowI = 0; rowI < rows; rowI++) {
        for (int columnI = 0; columnI < columns; columnI++) {
            cout << MAP[rowI][columnI] << " ";
        }
        cout << endl;
    }
}

// Perhaps I can create a move function that is something like this:
// ` bool forceMove(int x, int y, string direction){} `
// It's purpose is to find the thing sitting at (x, y), and move it in the specified direction (if possible).
// If it is succseful, it returns true. Otherwise, it returns false. <This return value may not always be utilized>

// vec2d.push_back(new_row);
// vec2d[row_index].push_back(new_element);

// 000]000]0]
//          ^

// 0 0 0 ]
// 0 0 0 ]
// 0 - - -
// - - - -

// 000]0000]00]0]0000000]]]0

// <brief> Explanation of the new for-loop:
// The vector is populated using a nested for-loop. The outer loop iterates 
// // through each row, the inner loop iterates through each column.
// In each pass of the outer loop, a new default row is added to the vector. It
// // also checks if the end of the input string has been reached.
// Within the inner loop, it basically checks for 2 possibilities:
// // // <A> The current character in the input string is premature, and needs
// // // to be held off to the side until it is ready to be used.
// // // <B> The current character in the input string is good and ready to be
// // // used.
// The <A> case has two possibilities:
// // // 1: it has reached a new row character prematurely
// // // 2: it has reached the end of the input string prematurely
