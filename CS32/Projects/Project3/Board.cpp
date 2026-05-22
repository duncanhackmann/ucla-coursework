//
//  Board.cpp
//  Project 3
//
//  Created by Duncan Hackmann on 5/30/23.
//

#include <stdio.h>
#include "Board.h"


Board::Board(int nHoles, int nInitialBeansPerHole)
    : m_nHoles(nHoles), m_nInitialBeansPerHole(nInitialBeansPerHole), m_totalBeans(0)
{
    if (nHoles < 1)
        m_nHoles = 1;

    for (int i = 0; i < m_nHoles; i++) {
        Hole hole;
        hole.beans = nInitialBeansPerHole;
        hole.owner = SOUTH;
        m_holes.push_back(hole);

        hole.owner = NORTH;
        m_holes.push_back(hole);

        m_totalBeans += 2 * nInitialBeansPerHole;
    }
}

int Board::holes() const {
    return m_nHoles;
}

int Board::beans(Side s, int hole) const {
    if (hole < 0 || hole > 2 * m_nHoles)
        return -1;

    return m_holes[hole].beans;
}

int Board::beansInPlay(Side s) const {
    int beansInPlay = 0;
    int startHole = (s == SOUTH) ? 0 : m_nHoles + 1;
    int endHole = (s == SOUTH) ? m_nHoles : 2 * m_nHoles - 1;

    for (int hole = startHole; hole <= endHole; hole++) {
        beansInPlay += m_holes[hole].beans;
    }

    return beansInPlay;
}

int Board::totalBeans() const {
    return m_nHoles*m_nInitialBeansPerHole*2;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole) {
    if (hole < 1 || hole > m_nHoles || m_holes[hole - 1 + s * (m_nHoles + 1)].beans == 0)
        return false;

    endSide = s;
    endHole = hole;

    int beansToSow = m_holes[hole - 1 + s * (m_nHoles + 1)].beans;
    m_holes[hole - 1 + s * (m_nHoles + 1)].beans = 0;

    while (beansToSow > 0) {
        hole = (hole + 1) % (2 * m_nHoles + 1);
        if (hole == 0)
            endSide = (endSide == SOUTH) ? NORTH : SOUTH;
        if (hole != 0 || endSide == s) {
            if (endSide == s && hole == 0)
                continue;  // Skip opponent's pot

            m_holes[hole - 1 + endSide * (m_nHoles + 1)].beans++;
            beansToSow--;
            endHole = hole;
        }
    }

    if (hole != 0 && endSide == s && m_holes[endHole - 1 + endSide * (m_nHoles + 1)].beans == 1) {
        int oppositeHole = 2 * m_nHoles + 1 - endHole;
        if (m_holes[oppositeHole - 1].beans > 0) {
            m_holes[s * (m_nHoles + 1)].beans += m_holes[oppositeHole - 1].beans + 1;
            m_holes[oppositeHole - 1].beans = 0;
            m_holes[endHole - 1 + endSide * (m_nHoles + 1)].beans = 0;
        }
    }

    return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner) {
    if (hole < 1 || hole > m_nHoles)
        return false;

    if (potOwner != NORTH && potOwner != SOUTH)
        return false;

    int holeIndex = hole - 1 + s * (m_nHoles + 1);
    int potIndex = potOwner * (m_nHoles + 1);

    m_holes[potIndex].beans += m_holes[holeIndex].beans;
    m_holes[holeIndex].beans = 0;

    return true;
}

bool Board::setBeans(Side s, int hole, int beans) {
    if (hole < 0 || hole > m_nHoles || beans < 0)
        return false;

    m_totalBeans += beans - m_holes[hole - 1 + s * (m_nHoles + 1)].beans;
    m_holes[hole - 1 + s * (m_nHoles + 1)].beans = beans;

    return true;
}
