//
//  Rabbit.h
//  CS32 Project 1
//
//  Created by Duncan Hackmann on 4/10/23.
//

#ifndef Rabbit_h
#define Rabbit_h


#include <iostream>
using namespace std;

class Arena;

class Rabbit
{
  public:
      // Constructor
    Rabbit(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
    int    m_idleTurnsRemaining;
};

#endif /* Rabbit_h */
