# Sequence — Doubly-Linked List

A generic `Sequence` container implemented from scratch as a doubly-linked list in C++.

## Interface

```cpp
Sequence s;
s.insert(0, "hello");   // insert at position
s.erase(0);             // remove at position
s.get(0, val);          // retrieve value
s.size();               // number of elements
```

Also implements `subsequence()` and `interleave()` free functions for combining sequences.

## Files

| File | Contents |
|------|----------|
| `Sequence.h` | Class declaration |
| `Sequence.cpp` | Doubly-linked list implementation |
| `main.cpp` | Test driver |
| `test.cpp` | Unit tests |

## Compiling

```bash
g++ -std=c++17 -o sequence Sequence.cpp main.cpp
./sequence
```
