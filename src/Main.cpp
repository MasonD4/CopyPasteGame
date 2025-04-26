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
vector<vector<char>> makeMapFromString(string);

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
    // vector<vector<char>> map;
    makeMapFromString(mapString);
    cout << "yay it worked!";
}

vector<vector<char>> makeMapFromString(string input) {
    vector<vector<char>> output;
    int biggestRowLength = 1, currentRowLength = 1;
    
    if (input.length() < 1)
    {
        output = {{'E', 'M', 'P', 'T', 'Y'}, {'I', 'P', 'U', 'T'}};
        return output;
    }

    // Count the length of each row (IT WORKS!!!)
    for (int i = 0; i < input.length(); i++) {
        if ((input[i] == ']' || i == input.length() - 1) && currentRowLength > biggestRowLength) {
            biggestRowLength = currentRowLength;
            currentRowLength = 1;
        }
        else if ((input[i] == ']' || i == input.length() - 1) && currentRowLength <= biggestRowLength) {
            currentRowLength = 1;
        }
        else {
            currentRowLength++;
        }
    }
    cout << "Biggest: " << biggestRowLength << endl; // temporary
    output = {{'E', 'M', 'P', 'T', 'Y'}, {'I', 'P', 'U', 'T'}}; // temporary
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