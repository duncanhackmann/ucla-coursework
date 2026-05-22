//
//  utilities.cpp
//  CS32 Project 1
//
//  Created by Duncan Hackmann on 4/10/23.
//

#include "globals.h"
#include "Arena.h"
#include <utility>
#include <random>

using namespace std;

int randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(lowest, highest);
    return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
      default:  return false;
      case 'n': dir = NORTH; break;
      case 'e': dir = EAST;  break;
      case 's': dir = SOUTH; break;
      case 'w': dir = WEST;  break;
    }
    return true;
}

  // Return false without changing anything if moving one step from (r,c)
  // in the indicated direction would run off the edge of the arena.
  // Otherwise, update r and c to the position resulting from the move and
  // return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
    int rnew = r;
    int cnew = c;
    switch (dir)
    {
      case NORTH:  if (r <= 1)        return false; else rnew--; break;
      case EAST:   if (c >= a.cols()) return false; else cnew++; break;
      case SOUTH:  if (r >= a.rows()) return false; else rnew++; break;
      case WEST:   if (c <= 1)        return false; else cnew--; break;
    }
    r = rnew;
    c = cnew;
    return true;
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should drop a poisoned carrot and
  // not move; otherwise, this function sets bestDir to the recommended
  // direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
      // How dangerous is it to stand?
    int standDanger = computeDanger(a, r, c);

      // if it's not safe, see if moving is safer
    if (standDanger > 0)
    {
        int bestMoveDanger = standDanger;
        int bestMoveDir = NORTH;  // arbitrary initialization

          // check the four directions to see if any move is
          // better than standing, and if so, record the best
        for (int dir = 0; dir < NUMDIRS; dir++)
        {
            int rnew = r;
            int cnew = c;
            if (attemptMove(a, dir, rnew, cnew))
            {
                int danger = computeDanger(a, rnew, cnew);
                if (danger < bestMoveDanger)
                {
                    bestMoveDanger = danger;
                    bestMoveDir = dir;
                }
            }
        }

          // if moving is better than standing, recommend move
        if (bestMoveDanger < standDanger)
        {
            bestDir = bestMoveDir;
            return true;
        }
    }
    return false;  // recommend standing
}

int computeDanger(const Arena& a, int r, int c)
{
      // Our measure of danger will be the number of rabbits that might move
      // to position (r,c).  If a rabbit is at that position, it is fatal,
      // so a large value is returned.

    if (a.numberOfRabbitsAt(r,c) > 0)
        return MAXRABBITS+1;

    int danger = 0;
    if (r > 1)
        danger += a.numberOfRabbitsAt(r-1,c);
    if (r < a.rows())
        danger += a.numberOfRabbitsAt(r+1,c);
    if (c > 1)
        danger += a.numberOfRabbitsAt(r,c-1);
    if (c < a.cols())
        danger += a.numberOfRabbitsAt(r,c+1);

    return danger;
}

#ifdef _WIN32

#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not _WIN32

#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
