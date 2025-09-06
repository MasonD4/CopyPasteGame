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
// bool jump(vector<vector<char>>&, int, int, int, int, string);
// bool move(vector<vector<char>>&, int, int, int, int, string, int);
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
    // vector<vector<char>> map = {{'#', '#', '-', '-', ']'}, 
    //                             {'#', '-', '-', '-', ']'}, 
    //                             {'-', '-', '@', '-', ']'}, 
    //                             {'#', '-', '-', '-', ']'}, 
    //                             {'#', '#', '-', '-', ']'}};

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

/*
List of conditions:
* Jump from unavailable spot [DONE]
* Jump from empty spot [DONE]
* Jump off the edge [DONE]
* Jump onto enemy [DONE]
* Jump onto coin
* normal jump (onto air) [DONE]
* Enemy jump onto player [DONE]
* Jump onto occupied space [DONE]
* Jump over enemy
*/

/* Here lies jump:
bool jump(vector<vector<char>>& map, int rows, int cols, int x, int y, string direction) {
    if (x < 0 || y < 0 || x >= cols || y >= rows) {
        cout << "tried to jump from a non-existant spot." << endl;
        return false;
    }
    else if (map[y][x] == EMPTY_SPACE) {
        cout << "This is am empty space" << endl;
        return true; // Maybe change this.
    }

    int newX = x;
    int newY = y;
    if (direction == "up") { newY -= 2; }
    else if (direction == "right") { newX += 2; }
    else if (direction == "down") { newY += 2; }
    else if (direction == "left") { newX -= 2; }
    else {
        cout << "BAD DIRECTION: " << direction << endl;
        return false;
    }

    // "Edge" case
    if (newX < 0 || newY < 0 || newX >= cols || newY >= rows) {
        cout << "Attempting to jump off the edge... FAILED" << endl;
        return false;
    }
    // Player jumps onto an enemy/dangerous spot
    else if (map[y][x] == PLAYER && isDangerous(map[newY][newX]) == true) {
        map[y][x] = EMPTY_SPACE;
        return true;
    }
    // Enemy jumps onto a player.
    else if (isDangerous(map[y][y]) == true && map[newY][newX] == PLAYER) {
        map[newY][newX] = map[y][x];
        map[y][x] = EMPTY_SPACE;
        return true;
    }
    // Question: Should only the player be able to pick up coins, or can anything?
    // Answer: Primarily just the player, but there can be some exceptions. For most other moving objects,
    //          coins will simply be pushed
    else if (map[y][x] == PLAYER && map[newY][newX] == COIN) {
        map[newY][newX] = map[y][x];
        map[y][x] = EMPTY_SPACE;
        // Make a function or something that increments the coin counter.
        return true;
    }
    else if (map[newY][newX] != EMPTY_SPACE) {
        return false;
    }
    else if (map[newY][newX] == EMPTY_SPACE) {
        map[newY][newX] = map[y][x];
        map[y][x] = EMPTY_SPACE;
        return true;
    }
}
*/

/* Here lies move:
bool move(vector<vector<char>>& map, int rows, int cols, int x, int y, string direction, int recursiveCount) {
    // The recursive limit will be set in here, which means that external calls start at 0.
    const int RECURSIVE_LIMIT = 5;
    // In each of the if statements that call this function, they need to check to see if this threshold has been
    // reached.
    
    if (recursiveCount > RECURSIVE_LIMIT) {
        cout << "Recursive fail" << endl;
        return false;
    }
    if (x < 0 || y < 0 || x >= cols || y >= rows) {
        cout << "tried to move from a non-existant spot." << endl;
        return false;
    }
    else if (isMovable(map[y][x]) == false) {
        cout << "This is not movable" << endl;
        return false;
    }
    else if (map[y][x] == EMPTY_SPACE) {
        cout << "This is am empty space" << endl;
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

    // ToDo: I may want to rearrange these if statements. What if there is an immovable enemy?

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
        map[y][x] = EMPTY_SPACE;
        return true;
    }
    // An enemy/dangerous object moves onto a player
    else if (isDangerous(map[y][x]) == true && map[newY][newX] == PLAYER) {
        map[newY][newX] = map[y][x];
        map[y][x] = EMPTY_SPACE;
        return true;
    }
    // ToDo: add a case for if the player gets a coin.
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
*/

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

// TODO Jump function
/// Note: Everything can jump. 'Cause why not. Maybe it can be a puzzle mechanic. Like, you need to move a wall
/// out of your way by by using a block that can "push" any block by telling that block to jump in some direction.

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


/*

--------]
-@]
]
-#]
]
--0]
]
-@]
]
@]
]
-@#]
]
-@]
]
-@!]
-------m]
-!@---mm]
-----mmm]
-@0--mmm]
-----mmm]
mmm@-mmm]
-----@@@]

*/
/* Testing the move function, using the above testing-grounds map.
    move(map, rows, columns, 1, 1, "right", 5); // (1, 1) recursive fail.
    move(map, rows, columns, 8, 1, "up", 0); // Move the newline character.
    move(map, rows, columns, 9, 1, "up", 0); // Out of bounds move.
    move(map, rows, columns, 1, 3, "right", 0); // Move an immovable.
    move(map, rows, columns, 1, 5, "right", 0); // Move nothing.
    move(map, rows, columns, 1, 7, "Blorg", 0); // Bad direction.
    move(map, rows, columns, 0, 9, "left", 0); // Move to the edge.
    move(map, rows, columns, 1, 11, "right", 0); // Push an immovable
    move(map, rows, columns, 1, 13, "right", 0); // Valid move right.
    move(map, rows, columns, 1, 15, "right", 0); // Move into danger
    move(map, rows, columns, 1, 17, "right", 0); // Move danger.
    move(map, rows, columns, 1, 19, "right", 0); // Push a block.
    move(map, rows, columns, 3, 21, "left", 0); // Move 3 blocks to the egde.
    move(map, rows, columns, 5, 22, "up", 0); // Push 4 blocks.
    move(map, rows, columns, 6, 22, "up", 0); // Push 5 blocks.
    move(map, rows, columns, 7, 22, "up", 0); // Push 6 blocks.
*/

/* Testing ground for jump



*/