//
//  evenlist.cpp
//  Homework 4
//
//  Created by Duncan Hackmann on 5/30/23.
//


void removeEven(list<int>& li) {
    for (auto it = li.begin(); it != li.end(); )
    {
        if (*it % 2 == 0)
            it = li.erase(it);
        else
            ++it;
    }
}
