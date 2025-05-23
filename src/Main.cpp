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

const char PLAYER = '@';
const char SPACE = '-';
const char WALL = '#';
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
    vector<vector<char>> output;
    int biggestRowLength = 1, currentRowLength = 1;
    
    if (input.length() < 1) {
        output = {{'E', 'M', 'P', 'T', 'Y'}, {'I', 'P', 'U', 'T', '!'}};
        rows = 2;
        columns = 5;
        return output;
    }

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
    }
    columns = biggestRowLength;

    output.push_back({' '});
    rows = 1;
    int currentRow = 0; 
    int currentCol = 0;

    for (int i = 0; i < input.length(); i++) {

        if (input[i] == NEW_ROW && currentCol < columns - 1) {
            for (int j = currentCol; j < columns; j++) {
                if (j == columns - 1) { output[currentRow][currentCol] = NEW_ROW; }
                else { output[currentRow][currentCol] = SPACE; currentCol++; }
            }

            output.push_back({' '});
            rows++;
            currentRow++;
            currentCol = 0;
        }
        else if (i == input.length() - 1 && currentCol < columns - 1) {
            output[currentRow][currentCol] = input[i];
            currentCol++;
            for (int j = currentCol; j < columns; j++) {
                output[currentRow][currentCol] = SPACE;
            }
        }
        else if (input[i] == NEW_ROW) {
            output[currentRow][currentCol] = input[i];
            output.push_back({' '});
            rows++;
            currentRow++;
            currentCol = 0;
        }
        else {
            output[currentRow][currentCol] = input[i];
            output[currentRow].push_back(' ');
            currentCol++;
        }
    }
    
    // output = {{'E', 'M', 'P', 'T', 'Y'}, {'I', 'P', 'U', 'T', '!'}}; // temporary
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
// What if some inputs a map where the row-length is inconsistent?

// vec2d.push_back(new_row);
// vec2d[row_index].push_back(new_element);