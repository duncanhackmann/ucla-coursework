//
//  History.cpp
//  CS32 Project 1
//
//  Created by Duncan Hackmann on 4/10/23.
//

#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols) {
    m_Rows = nRows;
    m_Cols = nCols;
    
    for (int i = 0; i < m_Rows; i++) {
            for (int j = 0; j < m_Cols; j++) {
                m_grid[i][j] = 0;
            }
        }
}

bool History::record(int r, int c) {
    if (r > m_Rows || c > m_Cols) {
        return false;
    }
    
    m_grid[r-1][c-1]++;
    return true;
    
}

void History::display() const {
    clearScreen();
    
    for (int r = 0; r < m_Rows; r++) {
        for (int c = 0; c < m_Cols; c++) {
            if (m_grid[r][c] == 1) {
                cout << "A";
            }
            else if (m_grid[r][c] == 2) {
                cout << "B";
            }
            else if (m_grid[r][c] == 3) {
                cout << "C";
            }
            else if (m_grid[r][c] == 4) {
                cout << "D";
            }
            else if (m_grid[r][c] == 5) {
                cout << "E";
            }
            else if (m_grid[r][c] == 6) {
                cout << "F";
            }
            else if (m_grid[r][c] == 7) {
                cout << "G";
            }
            else if (m_grid[r][c] == 8) {
                cout << "H";
            }
            else if (m_grid[r][c] == 9) {
                cout << "I";
            }
            else if (m_grid[r][c] == 10) {
                cout << "J";
            }
            else if (m_grid[r][c] == 11) {
                cout << "K";
            }
            else if (m_grid[r][c] == 12) {
                cout << "L";
            }
            else if (m_grid[r][c] == 13) {
                cout << "M";
            }
            else if (m_grid[r][c] == 14) {
                cout << "N";
            }
            else if (m_grid[r][c] == 15) {
                cout << "O";
            }
            else if (m_grid[r][c] == 16) {
                cout << "P";
            }
            else if (m_grid[r][c] == 17) {
                cout << "Q";
            }
            else if (m_grid[r][c] == 18) {
                cout << "R";
            }
            else if (m_grid[r][c] == 19) {
                cout << "S";
            }
            else if (m_grid[r][c] == 20) {
                cout << "T";
            }
            else if (m_grid[r][c] == 21) {
                cout << "U";
            }
            else if (m_grid[r][c] == 22) {
                cout << "V";
            }
            else if (m_grid[r][c] == 23) {
                cout << "W";
            }
            else if (m_grid[r][c] == 24) {
                cout << "X";
            }
            else if (m_grid[r][c] == 25) {
                cout << "Y";
            }
            else if (m_grid[r][c] >= 26) {
                cout << "Y";
            } else {
                cout << ".";
            }
        }
        cout << endl;
    }
}
