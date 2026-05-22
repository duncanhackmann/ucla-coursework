//
//  Player.h
//  Project 3
//
//  Created by Duncan Hackmann on 5/30/23.
//

#ifndef Player_h
#define Player_h

#include "Board.h"
#include "Side.h"
#include <string>


class Player {
public:
    Player(std::string name);
    std::string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player();
private:
    std::string m_name;
};

class HumanPlayer : public Player {
public:
    HumanPlayer(std::string name);
    bool isInteractive() const;
    int chooseMove(const Board& b, Side s) const override;
};

class BadPlayer : public Player {
public:
    BadPlayer(std::string name);
    int chooseMove(const Board& b, Side s) const override;
};

class SmartPlayer : public Player {
public:
    SmartPlayer(std::string name);
    int chooseMove(const Board& b, Side s) const override;
private:
    void findBestMove(const Board& b, Side s, int& bestHole, int& value) const;
    int evaluateMove(const Board& b, Side s, int hole) const;
};

#endif /* Player_h */
