# Brewin Interpreter — Version 4

The final, fully-featured interpreter adding a **static type system with interfaces and object-oriented programming**.

## New in v4

- **Full type enum** — `INT`, `STRING`, `BOOL`, `OBJECT`, `FUNCTION`, `VOID`, `ERROR`
- **Interface types** — uppercase-suffixed variable names (e.g., `myObj_A`) bind to a named interface `A`; the interpreter enforces that assigned objects implement that interface
- **Type inference from variable name** — `get_decl_type_and_iface(name)` extracts both the base type and optional interface from the last character of the variable name
- **`Value` class** — every runtime value is a typed `Value(type, data)` wrapper with a default for each type; supports `set()` and copy semantics
- **Object creation and method dispatch**
- **`void` functions** — functions that return no value are now explicitly typed

## Type system design

Type information is encoded in variable name suffixes (a deliberate language design decision in Brewin):

| Suffix | Type |
|--------|------|
| `i` | `int` |
| `s` | `string` |
| `b` | `bool` |
| `o` | `object` |
| `f` | `function` |
| `v` | `void` |
| Uppercase letter | `object` implementing interface `<Letter>` |

## Running

```bash
cd f25_starter
python3 interpreterv4.py
```

Or against the test suite:

```bash
python3 tester.py 4
```
