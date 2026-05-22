# Brewin Interpreter — Version 1

The first version of a from-scratch interpreter for **Brewin**, a custom dynamically-typed language with Python-like syntax.

## What it supports

- Variable declaration (`var x;`) and assignment (`x = 5;`)
- Integer literals and string literals
- Arithmetic: `+` and `-`
- Built-in functions: `print(...)` and `inputi(prompt)`
- A single implicit global scope

## How it works

The entry point is `Interpreter.run(program_string)`:
1. The program string is parsed into an AST by the course-provided `brewparse.py` (PLY-based).
2. `find_main()` locates the `main` function node.
3. `run_function()` iterates over statements and dispatches to statement handlers.
4. Expressions are evaluated recursively in `eval_expr()`.

Variables are stored in a flat dictionary (`self.vars`). No call stack — only global scope at this stage.

## Running

```bash
cd f25_starter
python3 interpreterv1.py
```

Or against the test suite:

```bash
python3 tester.py 1
```
