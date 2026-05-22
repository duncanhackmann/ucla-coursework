# Bomb Lab — Binary Reverse Engineering

Defuse a multi-phase binary bomb by reverse-engineering its x86-64 assembly.

## Description

The bomb is a compiled C binary (`bomb350`) with multiple phases, each expecting a specific input string. A wrong answer triggers a simulated "explosion." No source code is provided — the task is to disassemble the binary, read the assembly, and determine the expected input for each phase.

## Approach

1. Run the bomb in **gdb** and set a breakpoint before the explosion check
2. Disassemble each phase function with `disas <function>`
3. Trace the control flow — comparisons, jumps, and function calls — to reverse-engineer the required input
4. Repeat for each phase

## Skills demonstrated

- x86-64 calling convention and register usage
- Reading compiler-generated assembly (loops, switches, recursion)
- gdb: breakpoints, `stepi`, `x` (examine memory), `info registers`
- Recognizing common idioms: string comparison, linked-list traversal, recursive functions
