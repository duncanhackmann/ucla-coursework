//
//  Side.h
//  Project 3
//
//  Created by Duncan Hackmann on 5/30/23.
//

#ifndef Side_h
#define Side_h

enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;

inline
Side opponent(Side s)
{
    return Side(NSIDES - 1 - s);
}

#endif /* Side_h */
