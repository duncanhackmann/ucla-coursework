//
//  mazestack.cpp
//  Homework 2
//
//  Created by Duncan Hackmann on 5/1/23.
//

#include <stdio.h>
#include <iostream>

#include <stack>
using namespace std;
class Coord
        {
          public:
            Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
            int r() const { return m_r; }
            int c() const { return m_c; }
          private:
            int m_r;
            int m_c;
        };
bool pathExists(char maze[][10], int sr, int sc, int er, int ec) {
    stack<Coord> coordStack;
    Coord start(sr, sc);
    Coord end(er, ec);
    Coord current(sr, sc);
    
    coordStack.push(start);
    maze[sr][sc]='d';
    
    while (!coordStack.empty()) {
        current=coordStack.top();
        coordStack.pop();
        
        if (current.r() == end.r() && current.c() == end.c()) {
            return true;
        }
        
        //south
        if (maze[current.r()-1][current.c()] == '.') {
            coordStack.push(Coord(current.r()-1, current.c()));
            maze[current.r()-1][current.c()] = 'd';
        }
        
        //east
        if (maze[current.r()][current.c()+1] == '.') {
            coordStack.push(Coord(current.r(), current.c()+1));
            maze[current.r()][current.c()+1] = 'd';
        }
        
        //north
        if (maze[current.r()+1][current.c()] == '.') {
            coordStack.push(Coord(current.r()+1, current.c()));
            maze[current.r()+1][current.c()] = 'd';
        }
        
        //west
        if (maze[current.r()][current.c()-1] == '.') {
            coordStack.push(Coord(current.r(), current.c()-1));
            maze[current.r()][current.c()-1] = 'd';
        }
    }
    return false;
}


