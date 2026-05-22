//
//  badvector.cpp
//  Homework 4
//
//  Created by Duncan Hackmann on 5/30/23.
//


    void removeBad(vector<Restaurant*>& v) {
        v.erase(remove_if(v.begin(), v.end(), [](Restaurant* r) {
                if (r->stars() <= 2) {
                    delete r;
                    return true;
                }
                return false;
            }), v.end());
    }
