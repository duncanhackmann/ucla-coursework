# Mancala AI

A playable Mancala game in C++ with a pluggable AI player strategy.

## Description

Implements the classic Mancala board game with a North/South board, pots, and full move validation. Supports both human-interactive and computer-controlled players via a polymorphic `Player` interface.

## Classes

| Class | Responsibility |
|-------|---------------|
| `Board` | Game state — holes and pot counts for each side |
| `Side` | Enum (`NORTH`/`SOUTH`) with `opponent()` utility |
| `Player` | Abstract base — `isInteractive()`, `chooseMove()` |
| `Game` | Game loop, turn management, win detection |

A computer player can be implemented by subclassing `Player` and overriding `chooseMove()` with any search or heuristic strategy.

## Compiling & Running

```bash
g++ -std=c++17 -o mancala *.cpp
./mancala
```
