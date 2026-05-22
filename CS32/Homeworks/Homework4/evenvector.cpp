//
//  evenvector.cpp
//  Homework 4
//
//  Created by Duncan Hackmann on 5/30/23.
//

    void removeEven(vector<int>& v) {
        v.erase(remove_if(v.begin(), v.end(), [](int num) { return num % 2 == 0; }), v.end());
    }

    
