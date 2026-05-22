//
//  Game.h
//  CS32 Project 1
//
//  Created by Duncan Hackmann on 4/10/23.
//

#ifndef Game_h
#define Game_h

#include <string>
#include <iostream>

using namespace std;

class Arena;

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nRabbits);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    string takePlayerTurn();
};


#endif /* Game_h */
