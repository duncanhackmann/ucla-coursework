# CS161 — Algorithm Design & Analysis

UCLA upper-division course covering the design, analysis, and implementation of algorithms.

## Homeworks

| Folder | Topic |
|--------|-------|
| [Homework 1](Homework1/) | Recursion — integer sequences, tree traversal, tree anonymization |
| [Homework 2](Homework2/) | Tree search — breadth-first and depth-first traversal |
| [Homework 3](Homework3/) | A* search — Sokoban puzzle solver with admissible heuristic |
| [Homework 4](Homework4/) | Satisfiability — graph k-colorability via SAT reduction |

## Highlights

**HW3 — A* Sokoban Solver**  
Implements A* search (using a provided `astar.py` engine) to solve Sokoban puzzles. The key challenge is designing an admissible heuristic over board states represented as NumPy arrays. The solver handles complex multi-box configurations within memory constraints.

**HW4 — Graph Coloring as SAT**  
Encodes the graph k-colorability problem as a Boolean satisfiability (SAT) instance and feeds it to an external SAT solver (rsat). Includes generating at-least-one and at-most-one color clauses per node, and adjacency conflict clauses per edge.

## Tech Stack

- **Python 3** — all implementations
- **NumPy** — board state representation (HW3)
- **rsat** — SAT solver (HW4)
