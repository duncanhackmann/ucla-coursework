# CS131 — Programming Languages

UCLA upper-division course covering the theory and implementation of programming languages.

## Projects

A four-part sequence building a complete interpreter for **Brewin**, a custom language with Python-like syntax. Each version adds a new layer of language features:

| Version | New Features |
|---------|-------------|
| [Project 1](Projects/Project1/) | Variables, arithmetic (`+`, `-`), `print`, `inputi` |
| [Project 2](Projects/Project2/) | User-defined functions, call stack, `return` |
| [Project 3](Projects/Project3/) | Typed variables, closures, first-class functions |
| [Project 4](Projects/Project4/) | Full type system with interfaces and object-oriented features |

The interpreter is implemented in Python, using the PLY library for lexing and parsing (provided as course infrastructure). Each `interpreterv{N}.py` file is a standalone, runnable interpreter.

### Running a project

```bash
cd Projects/Project1/f25_starter
python3 interpreterv1.py
```

Or run against the local test suite:

```bash
python3 tester.py 1   # runs test cases for version 1
```

## Homeworks

| Folder | Topic |
|--------|-------|
| [Homework0](Homeworks/Homework0/) | Course setup, Python warm-up |
| [Homework1](Homeworks/Homework1/) | Language concepts |
| [Homework2](Homeworks/Homework2/) | Parsing and semantics |

## Tech Stack

- **Python 3.11+**
- **PLY** (Python Lex-Yacc) — lexer and parser for Brewin
