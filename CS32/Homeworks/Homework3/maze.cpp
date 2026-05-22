//
//  maze.cpp
//  Homework 3
//
//  Created by Duncan Hackmann on 5/10/23.
//

bool pathExists(char maze[][10], int sr, int sc, int er, int ec) {
    if (sr == er && sc == ec) { // base case: we've reached the end
        return true;
    }
    maze[sr][sc] = 'd'; // mark current location as visited
    
    // try moving south
    if (maze[sr-1][sc] == '.') {
        if (pathExists(maze, sr-1, sc, er, ec)) {
            return true;
        }
    }
    
    // try moving east
    if (maze[sr][sc+1] == '.') {
        if (pathExists(maze, sr, sc+1, er, ec)) {
            return true;
        }
    }
    
    // try moving north
    if (maze[sr+1][sc] == '.') {
        if (pathExists(maze, sr+1, sc, er, ec)) {
            return true;
        }
    }
    
    // try moving west
    if (maze[sr][sc-1] == '.') {
        if (pathExists(maze, sr, sc-1, er, ec)) {
            return true;
        }
    }
    
    return false; // couldn't find a path
}
