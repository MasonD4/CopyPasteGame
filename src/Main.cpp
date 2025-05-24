/* Title: Copy Paste Game
 * File: Main.cpp
 *
 *  Created on: Apr 4, 2025
 *      Author: Mason Dunn
 */

#include <iostream>
#include <vector>
using namespace std;

void printMap(const vector<vector<char>>&, int, int);
vector<vector<char>> makeMapFromString(string, int&, int&);

// const char PLAYER = '@';
const char SPACE = '-';
// const char WALL = '#';
const char NEW_ROW = ']';

int main() {
    // vector<vector<char>> map = {{'#', '#', '-', '-', ']'}, 
    //                             {'#', '-', '-', '-', ']'}, 
    //                             {'-', '-', '@', '-', ']'}, 
    //                             {'#', '-', '-', '-', ']'}, 
    //                             {'#', '#', '-', '-', ']'}};
    int columns = 0;
    int rows = 0;
    cout << "Insert the map string: ";
    string mapString;
    cin >> mapString;

    vector<vector<char>> map = makeMapFromString(mapString, rows, columns);
    cout << "Just exited the makeMap function" << endl;

    printMap(map, rows, columns);
    cout << "Rows, Cols: " << rows << ", " << columns << endl;
    cout << "First: " << map[0][0] << endl;
    cout << "yay it worked!";
}

vector<vector<char>> makeMapFromString(const string input, int& rows, int& columns) {

    if (input.length() < 1) {
        vector<vector<char>> output = {{'E', 'M', 'P', 'T', 'Y'}, {'I', 'P', 'U', 'T', '!'}};
        rows = 2;
        columns = 5;
        return output;
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
    vector<char> newRowVector(columns, SPACE);
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

void printMap(const vector<vector<char>>& MAP, int rows, int columns) {
    for (int rowI = 0; rowI < rows; rowI++) {
        for (int columnI = 0; columnI < columns; columnI++) {
            cout << MAP[rowI][columnI] << " ";
        }
        cout << endl;
    }
}

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
