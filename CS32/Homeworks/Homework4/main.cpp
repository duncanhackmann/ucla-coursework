//
//  main.cpp
//  Homework 4
//
//  Created by Duncan Hackmann on 5/29/23.
//

#include "Sequence.h"
    #include <iostream>
    #include <string>
    #include <cassert>
    using namespace std;

    void test()
    {
        

        Sequence<string> ss;
        assert(ss.empty());
        assert(ss.size() == 0);
        assert(ss.insert(0, "Hello") == 0);
        assert(ss.insert("Goodbye") == 0);
        assert(ss.find("Hello") == 1);
        assert(ss.remove("Goodbye") == 1);
        string s;
        assert(ss.get(0, s));
        assert(ss.set(0, "Aloha"));
        assert(ss.erase(0));
        Sequence<string> ss2(ss);
        ss2.swap(ss);
        ss2 = ss;
        assert(subsequence(ss,ss2) == -1);
        concatReverse(ss,ss2,ss);
    }

    int main()
    {
        test();
        cout << "Passed all tests" << endl;
    }
