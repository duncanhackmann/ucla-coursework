# Brewin Interpreter — Version 3

Extends v2 to support **closures, first-class functions, and typed variables**.

## New in v3

- **Typed variables** — variable names carry type suffixes (`i` for int, `s` for string, `b` for bool, `f` for function, `o` for object)
- **First-class functions** — functions are values; `lambdas` and named functions can be stored in variables and passed as arguments
- **Closures** — captured environments travel with lambda values, enabling higher-order programming
- **Block scoping** — `FuncContext` maintains a stack of scope frames; variables are resolved innermost-first
- Functions are now keyed by `(name, type_signature)` rather than `(name, arity)`

## Architecture

Each active function call is represented by a `FuncContext` holding:
- The function's AST node
- A list of scope frames (dicts) — new frames pushed on `if`/`while` blocks, popped on exit
- A set of names defined in the current context (for shadowing)

`VarCell` wraps each value with its declared type, enabling type-checking at assignment.

## Running

```bash
cd f25_starter
python3 interpreterv3.py
```

Or against the test suite:

```bash
python3 tester.py 3
```
