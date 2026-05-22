# Arena Simulation

A grid-based simulation game in C++ where a player navigates an arena full of rabbits.

## Description

The arena is a 2D grid. Each turn the player can move to an adjacent cell; rabbits move randomly. If a rabbit steps on the player's cell, the player loses. The player wins by surviving until all rabbits are gone (they die off over time).

A `History` object tracks which cells have been poisoned, and the display renders the current grid state each turn.

## Classes

| Class | Responsibility |
|-------|---------------|
| `Arena` | Owns the grid, manages all rabbits and the player |
| `Player` | Tracks player position and alive/dead state |
| `Rabbit` | Individual rabbit with position and lifespan |
| `Game` | Game loop — alternates player input and rabbit movement |
| `History` | Records per-cell event counts for display |

## Compiling & Running

```bash
g++ -std=c++17 -o arena *.cpp
./arena
```
