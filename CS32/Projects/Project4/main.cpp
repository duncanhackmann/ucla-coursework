//
//  main.cpp
//  Project 4
//
//  Created by Duncan Hackmann on 6/8/23.
//

#include <iostream>
#include "Dictionary.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void printWord(string w)
    {
        cout << w << endl;
    }

    vector<string> v;  // global variable

    void appendWord(string w)
    {
        v.push_back(w);
    }

    int main()
    {
        Dictionary d;
        d.insert("cat");
        d.insert("dog");
        d.lookup("tca", printWord);  // writes  cat
        assert(v.empty());
        d.lookup("tca", appendWord);
        assert(v.size() == 1  &&  v[0] == "cat");
    }
