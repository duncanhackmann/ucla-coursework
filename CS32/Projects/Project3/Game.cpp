//
//  Game.cpp
//  Project 3
//
//  Created by Duncan Hackmann on 5/30/23.
//

#include <stdio.h>
#include "Game.h"
#include <iostream>

Game::Game(const Board& b, Player* south, Player* north)
    : m_board(b), m_southPlayer(south), m_northPlayer(north), m_currentPlayer(SOUTH), m_gameOver(false) {}

void Game::display() const {
    // Display the player names
    std::cout << m_southPlayer->name() << " (SOUTH)" << std::endl;
    std::cout << m_northPlayer->name() << " (NORTH)" << std::endl;

    // Display the board state
    int holes = m_board.holes();
    std::cout << "   ";
    for (int i = 1; i <= holes; i++) {
        std::cout << m_board.beans(NORTH, i) << " ";
    }
    std::cout << std::endl;

    std::cout << m_board.beans(NORTH, POT);
    for (int i = 0; i < holes; i++) {
        std::cout << "  ";
    }
    std::cout << m_board.beans(SOUTH, POT) << std::endl;

    std::cout << "   ";
    for (int i = 1; i <= holes; i++) {
        std::cout << m_board.beans(SOUTH, i) << " ";
    }
    std::cout << std::endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const {
    over = m_gameOver;

    if (m_gameOver) {
        int southScore = m_board.beansInPlay(SOUTH) + m_board.beans(SOUTH, POT);
        int northScore = m_board.beansInPlay(NORTH) + m_board.beans(NORTH, POT);

        if (southScore == northScore) {
            hasWinner = false;
        } else {
            hasWinner = true;
            winner = (southScore > northScore) ? SOUTH : NORTH;
        }
    }
}

bool Game::move(Side s) {
    if (m_gameOver || s != m_currentPlayer)
        return false;

    int move = m_currentPlayer == SOUTH ? m_southPlayer->chooseMove(m_board, SOUTH) : m_northPlayer->chooseMove(m_board, NORTH);

    if (move == -1)
        return false;

    Side endSide;
    int endHole;
    bool validMove = m_board.sow(m_currentPlayer, move, endSide, endHole);

    // Display the board after the move
    display();

    // Check for game over
    bool over;
    bool hasWinner;
    Side winner;
    status(over, hasWinner, winner);

    if (over) {
        m_gameOver = true;
        std::cout << "Game Over!" << std::endl;
        if (hasWinner)
            std::cout << "Player " << (winner == SOUTH ? m_southPlayer->name() : m_northPlayer->name()) << " wins!" << std::endl;
        else
            std::cout << "It's a tie!" << std::endl;
        return validMove;
    }

    // If the move ends in own pot, the current player gets another turn
    if (endSide == m_currentPlayer && endHole == POT)
        return true;

    // Switch to the next player
    switchPlayer();
    return validMove;
}

void Game::play() {
    display();

    while (!m_gameOver) {
        std::cout << "Player " << (m_currentPlayer == SOUTH ? m_southPlayer->name() : m_northPlayer->name()) << ", it's your turn." << std::endl;

        bool moveMade = false;
        while (!moveMade) {
            int move = -1;
            if (m_currentPlayer == SOUTH && m_southPlayer->isInteractive()) {
                std::cout << "Choose a hole to move: ";
                std::cin >> move;
            } else if (m_currentPlayer == NORTH && m_northPlayer->isInteractive()) {
                std::cout << "Choose a hole to move: ";
                std::cin >> move;
            } else {
                move = m_currentPlayer == SOUTH ? m_southPlayer->chooseMove(m_board, SOUTH) : m_northPlayer->chooseMove(m_board, NORTH);
            }

            moveMade = move != -1 && m_board.beans(m_currentPlayer, move) > 0;
            if (!moveMade) {
                std::cout << "Invalid move. Please try again." << std::endl;
            } else {
                Side endSide;
                int endHole;
                bool validMove = m_board.sow(m_currentPlayer, move, endSide, endHole);

                // Display the board after the move
                display();

                // Check for game over
                bool over;
                bool hasWinner;
                Side winner;
                status(over, hasWinner, winner);

                if (over) {
                    m_gameOver = true;
                    std::cout << "Game Over!" << std::endl;
                    if (hasWinner)
                        std::cout << "Player " << (winner == SOUTH ? m_southPlayer->name() : m_northPlayer->name()) << " wins!" << std::endl;
                    else
                        std::cout << "It's a tie!" << std::endl;
                    return;
                }

                // If the move ends in own pot, the current player gets another turn
                if (endSide == m_currentPlayer && endHole == POT) {
                    moveMade = false;  // Repeat the turn
                } else {
                    // Switch to the next player
                    switchPlayer();
                }
            }
        }
    }
}

int Game::beans(Side s, int hole) const {
    return m_board.beans(s, hole);
}

void Game::switchPlayer() {
    m_currentPlayer = (m_currentPlayer == SOUTH) ? NORTH : SOUTH;
}
