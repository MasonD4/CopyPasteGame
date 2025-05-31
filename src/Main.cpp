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

    // Get input from the player
    cout << "Insert the map string: \n";
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
    cout << "Rows, Cols: " << rows << ", " << columns << endl;
    cout << "First: " << map[0][0] << endl;
    cout << "yay it worked!";
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

// Possible solution to the 'space' problem and the ''\n' problem:
// // Use nextLine() or whatever it's called to get the input string, so
// // // that way it will be able to read spaces.
// // Then, whenever the user hits enter, it will simply allow them to add more
// // // characters to the string, and so on and so forth until they
// // // explicitly type "done" or something like that.
// // // Every time they press enter, the current input will be concatenated
// // // to the end of the string, and then the string will be re-evaluated
// // // as one whole string without spaces or newlines ('\n').
// // This way, the user can input the map as is, even if it has spaces or
// // // if each row is on a new line. It will still work, without the need
// // // for the user to remove all the spaces and newlines manually.
