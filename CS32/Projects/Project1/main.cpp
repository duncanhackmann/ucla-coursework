//
//  main.cpp
//  CS32 Project 1
//
//  Created by Duncan Hackmann on 4/10/23.
//

// rabbits.cpp
//
#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
#include "Game.h"
using namespace std;


int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 40);

      // Play the game
    g.play();
}




