# LING 185A — Computational Linguistics

UCLA course on the mathematical foundations of natural language processing: formal grammars, automata theory, and their application to linguistic phenomena.

All implementations are in **Haskell**.

## Homeworks

| Folder | Topic |
|--------|-------|
| [Homework 2](Homework2/) | **Regular Expressions** — define and compose regular languages |
| [Homework 3](Homework3/) | **Finite State Automata (FSA)** — implement FSA transitions and acceptance |
| [Homework 4](Homework4/) | **Epsilon-FSA** — non-deterministic automata with epsilon transitions |
| [Homework 5](Homework5/) | **Finite State Transducers (FST)** — string-to-string mappings for morphology |
| [Homework 6](Homework6/) | **Context-Free Grammars** — CYK parsing |
| [Homework 8](Homework8/) | **Probabilistic models** — weighted automata |

## Progression

The course builds from regular expressions → FSA → epsilon-FSA → FST, mirroring the Chomsky hierarchy. Each module is implemented from scratch in Haskell using algebraic data types and pattern matching — no external parsing libraries.

## Tech Stack

- **Haskell** (GHC)
- **GHCi** for interactive testing
