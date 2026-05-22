# Brewin Interpreter — Version 2

Extends the v1 interpreter to support **user-defined functions and a call stack**.

## New in v2

- User-defined functions with positional arguments
- Function calls as both statements and expressions
- `return` statement (implemented via a `ReturnSignal` exception)
- Function registry indexed by `(name, arity)` to support overloading by argument count
- A fresh environment dictionary is created per call; the previous environment is restored on return

## How it differs from v1

v1 used a single flat `self.vars` dict for all variables. v2 introduces:
- `self.funcs` — maps `(name, arity)` to AST function nodes, populated by `index_funcs()` at startup
- `self.env` — the current call frame, swapped in/out by `call_user_func()`
- `ReturnSignal` exception to unwind the call stack cleanly on `return`

## Running

```bash
cd f25_starter
python3 interpreterv2.py
```

Or against the test suite:

```bash
python3 tester.py 2
```
