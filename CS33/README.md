# CS33 — Introduction to Computer Organization

UCLA course covering the hardware-software interface: C memory model, x86-64 assembly, linking, the memory hierarchy, and parallel programming.

## Labs & Projects

| Folder | Description |
|--------|-------------|
| [Lab 0](Lab0/) | **Bit Manipulation** — implement common bitwise operations using only restricted integer operations |
| [Lab 1](Lab1/) | **Systems lab** — low-level C/assembly exercises |
| [Lab 2 — Bomb Lab](Lab2-BombLab/) | **Binary Bomb** — reverse-engineer a compiled binary with gdb to find the disarm phrase for each phase |
| [Parallel Lab](ParallelLab/) | **OpenMP Image Processing** — parallelize a sequential image pipeline; debug a race condition in Phase 1, then optimize Phases 2–3 with OpenMP pragmas |
| [Homework 1](Homework1/) | Written and coding exercises |
| [Target 517](Target517/) | Binary exploitation exercise |

## Highlights

**Bomb Lab**  
The bomb is a compiled C binary with multiple phases, each requiring a specific input string to defuse. Disarming it requires reading x86-64 assembly, setting breakpoints, and tracing execution in gdb — no source code provided.

**Parallel Lab**  
Three-phase image processing task (mean pixel, grayscale max, Gaussian blur) originally implemented sequentially. Phase 1 contained a deliberate data race; Phases 2 and 3 required adding `#pragma omp parallel` directives and choosing appropriate reduction strategies to maximize throughput.

## Tech Stack

- **C / C++**
- **x86-64 Assembly**
- **OpenMP** (parallel lab)
- **gdb** (bomb lab)
