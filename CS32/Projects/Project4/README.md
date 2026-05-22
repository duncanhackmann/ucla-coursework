# Anagram Dictionary

A hash-map backed dictionary that efficiently looks up all anagrams of a given input word.

## Interface

```cpp
Dictionary d;
d.insert("hello");
d.lookup("lleho", [](string word) { cout << word << "\n"; });
// prints "hello"
```

`lookup()` accepts a callback function called once for each word in the dictionary that is an anagram of the query string.

## Implementation

`DictionaryImpl` (in `Dictionary.cpp`) hides the data structure behind the `Dictionary` handle. Anagram lookup works by sorting the query string to produce a canonical key, then finding all dictionary words with the same sorted form.

## Compiling & Running

```bash
g++ -std=c++17 -o anagram Dictionary.cpp main.cpp
./anagram
```
