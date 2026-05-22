//
//  Board.h
//  Project 3
//
//  Created by Duncan Hackmann on 5/30/23.
//

#ifndef Board_h
#define Board_h

#include "Side.h"
#include <vector>

class Board {
public:
    Board(int nHoles, int nInitialBeansPerHole);
    int holes() const;
    int beans(Side s, int hole) const;
    int beansInPlay(Side s) const;
    int totalBeans() const;
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    bool moveToPot(Side s, int hole, Side potOwner);
    bool setBeans(Side s, int hole, int beans);
private:
    struct Hole {
        int beans;
        Side owner;
    };

    std::vector<Hole> m_holes;
    int m_nHoles;
    int m_nInitialBeansPerHole;
    int m_totalBeans;
};

#endif /* Board_h */
