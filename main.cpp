//
//  main.cpp
//  Mancala
//
//  Created by Robert Sandilands on 12/28/14.
//  Copyright (c) 2014 Robert Sandilands. All rights reserved.
//

#include <iostream>
#include "MancalaState.h"

std::ostream & operator<<(std::ostream & ostr, const Mancala::State & other) {
    for (int cnt = 0; cnt < Mancala::State::SIDE_SIZE; cnt++) {
        ostr << other.getCountAt(false, cnt) << " ";
    }
    ostr << other.getMancalaCount(false);
    ostr << std::endl;
    for (int cnt = 0; cnt < Mancala::State::SIDE_SIZE; cnt++) {
        ostr << other.getCountAt(true, cnt) << " ";
    }
    ostr << other.getMancalaCount(true);
    ostr << std::endl;
    return ostr;
}

const unsigned int COST_MY_MANCALA = 5;
const unsigned int COST_F_EMPTY = 2;
const unsigned int COST_HAVE_MORE = 10;
const unsigned int BIGGEST_DEPRESSION = 7;
const unsigned int COST_LESS_THAN_8 = 5;
const unsigned int COST_WAS_BIG_NOW_SMALL = 10;

int costOfState(const Mancala::State & newState, const Mancala::State & oldState, bool youOrMe) {
    // The more in the mancala the better
    unsigned int total = (newState.getMancalaCount(youOrMe) - oldState.getMancalaCount(youOrMe)) * COST_MY_MANCALA;
    if (newState.getCountAt(youOrMe, Mancala::State::SIDE_SIZE - 1) == 0) {
        total += COST_F_EMPTY;
    }
    // we don't want too many of our items in a depression
    for (int cnt = 0; cnt < Mancala::State::SIDE_SIZE; cnt++) {
        if (newState.getCountAt(youOrMe, cnt) < BIGGEST_DEPRESSION) {
            total += COST_LESS_THAN_8;
        }
    }
    // if we have a lot of items in a depression and we move them then goody!!!
    for (int cnt = 0; cnt < Mancala::State::SIDE_SIZE; cnt++) {
        if (oldState.getCountAt(youOrMe, cnt) > BIGGEST_DEPRESSION &&
            oldState.getCountAt(youOrMe, cnt) > newState.getCountAt(youOrMe, cnt)) {
                total += COST_WAS_BIG_NOW_SMALL;
        }
    }
    int myCost, theirCost;
    myCost = newState.getTotal(youOrMe);
    theirCost = newState.getTotal(!youOrMe);
    // if this leaves us a in a better state than the oponenent then goody
    if (myCost > theirCost) {
        total += COST_HAVE_MORE;
    } else {
        if (newState.isEndState()) {
            // if this will make us lose
            total = 0;
        }
    }
    return total;
}

int randomRound(const Mancala::State & state, bool meOrYou) {
    int retval = Mancala::State::SIDE_SIZE;
    bool validMove = false;
    
    do {
        retval = random() % Mancala::State::SIDE_SIZE;
        if (state.getCountAt(meOrYou, retval) != 0) {
            validMove = true;
        }
    } while (!validMove);
    return retval;
}

int manualRound(const Mancala::State & state, bool meOrYou) {
    int option;
    
    do {
        std::cout << state << std::endl << "Enter choice: ";
        std::cin >> option;
    } while (option < 0 || option >= Mancala::State::SIDE_SIZE || state.getCountAt(meOrYou, option) == 0);
    return option;
}

int bestRound(const Mancala::State & state, bool meOrYou) {
    Mancala::State localState;
    unsigned int retval = Mancala::State::SIDE_SIZE;
    unsigned int max = 0;
    
    for (int cnt = 0; cnt < Mancala::State::SIDE_SIZE; cnt++) {
        if (state.getCountAt(meOrYou, cnt) == 0) {
            continue;
        }
        localState = state;
        localState.play(meOrYou, cnt);
        int current = costOfState(localState, state, meOrYou);
        if (current >= max) {
            retval = cnt;
            max = current;
        }
    }
    return retval;
}

typedef enum {
    pt_match,
    pt_random,
    pt_manual
} PlayerType;

int main(int argc, const char * argv[]) {
    Mancala::State state, oldState;
    PlayerType pt = pt_match;
    
    if (argv[1]) {
        switch (argv[1][0]) {
            case 'c':
                pt = pt_match;
                break;
            case 'r':
                pt = pt_random;
                break;
            default:
                pt = pt_manual;
                break;
        }
    }
    std::cout << "Playing against: ";
    switch (pt) {
        case pt_match:
            std::cout << " Computer player";
            break;
        case pt_random:
            std::cout << " Random moves";
            break;
        case pt_manual:
            std::cout << " Human player";
            break;
    }
    std::cout << std::endl;
    std::cout << "Beginning state" << std::endl << state << std::endl;
    /*
    oldState = state;
    state.play(true, 0);
    unsigned int cost = costOfState(state, oldState, true);
    std::cout << state << " : " << cost << std::endl;
    oldState = state;
    state.play(true, 1);
    cost = costOfState(state, oldState, true);
    std::cout << state << " : " << cost <<std::endl;
    oldState = state;
    state.play(true, 2);
    cost = costOfState(state, oldState, true);
    std::cout << state << " : " << cost <<std::endl;
    oldState = state;
    state.play(true, 3);
    cost = costOfState(state, oldState, true);
    std::cout << state << " : " << cost <<std::endl;
    oldState = state;
    state.play(true, 4);
    cost = costOfState(state, oldState, true);
    std::cout << state << " : " << cost <<std::endl;
    oldState = state;
    state.play(true, 5);
    cost = costOfState(state, oldState, true);
    std::cout << state << " : " << cost <<std::endl;
    */
    
    int round = 1;
    do {
        do {
            int bestMe = bestRound(state, false);
            std::cout << "Player 1 " << bestMe << std::endl;
            state.play(false, bestMe);
        } while (state.samePlayerAnotherRound() && !state.isEndState());
        if (!state.isEndState()) {
            do {
                int bestYou;
                switch (pt) {
                    case pt_manual:
                        bestYou = manualRound(state, true);
                        break;
                    case pt_random:
                        bestYou = randomRound(state, true);
                        break;
                    case pt_match:
                        bestYou = bestRound(state, true);
                        break;
                }
                std::cout << "Player 2 " << bestYou << std::endl;
                state.play(true, bestYou);
            } while (state.samePlayerAnotherRound() && !state.isEndState());
        }
        std::cout << "Round " << round << ":" << std::endl << state << std::endl;
        round += 1;
    } while (!state.isEndState());
    unsigned int player1Total = state.getTotal(false);
    unsigned int player2Total = state.getTotal(true);
    
    std::cout << "Player 1: " << player1Total << std::endl;
    std::cout << "Player 2: " << player2Total << std::endl;
    
    if (player1Total > player2Total) {
        std::cout << "Player 1 Wins!!!";
    } else {
        if (player1Total < player2Total) {
            std::cout << "Player 2 Wins!!!";
        } else {
            std::cout << "It is a draw";
        }
    }
    std::cout << std::endl;
    return 0;
}
