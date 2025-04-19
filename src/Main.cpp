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

int main() {
    // vector<vector<char>> map = {{'#', '#', '-', '-', ']'}, 
    //                             {'#', '-', '-', '-', ']'}, 
    //                             {'-', '-', '@', '-', ']'}, 
    //                             {'#', '-', '-', '-', ']'}, 
    //                             {'#', '#', '-', '-', ']'}};
    int columns;
    int rows;
    string mapString;
    cin >> mapString;
    vector<vector<char>> map;
    cout << "yay it worked!";
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