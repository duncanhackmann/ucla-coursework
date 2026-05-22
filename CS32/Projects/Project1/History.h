//
//  History.h
//  CS32 Project 1
//
//  Created by Duncan Hackmann on 4/10/23.
//

#ifndef History_h
#define History_h

#include "globals.h"

class History
    {
      public:
        History(int nRows, int nCols);
        bool record(int r, int c);
        void display() const;
    private:
        int m_Rows, m_Cols;
        int m_grid[MAXROWS][MAXCOLS];
    };

#endif /* History_h */
