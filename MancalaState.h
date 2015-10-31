//
//  MancalaState.h
//  Mancala
//
//  Created by Robert Sandilands on 12/28/14.
//  Copyright (c) 2014 Robert Sandilands. All rights reserved.
//

#ifndef Mancala_MancalaState_h
#define Mancala_MancalaState_h

namespace Mancala {
  
    class State {
    public:
        static const unsigned int SIDE_SIZE = 6;
        static const unsigned int NUM_SIDES = 2;
        State() {
            // Initialize to default state
            for (int cnt = 0; cnt < SIDE_SIZE * NUM_SIDES; cnt++) {
                depressions[cnt] = 4;
            }
            for (int cnt = 0; cnt < NUM_SIDES; cnt++) {
                mancala[cnt] = 0;
            }
        }
        void play(bool meOrYou, unsigned int offset) {
            if (offset >= SIDE_SIZE) throw new std::exception();
            int startPos = (meOrYou)?(SIDE_SIZE + offset):offset;
            int numItems = depressions[startPos];
            depressions[startPos] = 0;
            startPos += 1;
            int mancalas = 0;
            playAgain = false;
            for (int cnt = 0; cnt < numItems; cnt++) {
                if ((startPos + cnt) == SIDE_SIZE ||
                    (startPos + cnt) == SIDE_SIZE * NUM_SIDES) {
                    if (mancalas == 0) {
                      mancala[((startPos + cnt)/SIDE_SIZE)-1] += 1;
                        if (cnt + 1 >= numItems) {
                            // if the last play is in the mancala then we get to play again
                            playAgain = true;
                        }
                    } else {
                        numItems += 1;
                    }
                    mancalas += 1;
                } else {
                    int workingOffset = (startPos + cnt - mancalas)%(SIDE_SIZE * NUM_SIDES);
                    if (mancalas == 0 && depressions[workingOffset] == 0 && cnt + 1 >= numItems) {
                        // do steal
                        int stealOffset = (workingOffset>SIDE_SIZE)?workingOffset-SIDE_SIZE:workingOffset+SIDE_SIZE;
                        mancala[(stealOffset>=SIDE_SIZE)?0:1] += depressions[stealOffset];
                        depressions[stealOffset] = 0;
                    }
                    depressions[workingOffset] += 1;
                }
            }
        }
        unsigned int getCountAt(bool meOrYou, unsigned int offset) const {
            if (offset >= SIDE_SIZE) throw new std::exception();
            int startPos = (meOrYou)?(SIDE_SIZE + offset):offset;
            return depressions[startPos];
        }
        unsigned int getMancalaCount(bool meOrYou) const {
            return mancala[(meOrYou)?1:0];
        }
        bool samePlayerAnotherRound() const {
            return playAgain;
        }
        bool isEndState() const {
            int total1 = 0;
            int total2 = 0;
            for (int cnt = 0; cnt < SIDE_SIZE; cnt++) {
                total1 += depressions[cnt];
                total2 += depressions[cnt + SIDE_SIZE];
            }
            return total1 == 0 || total2 == 0;
        }
        unsigned int getTotal(bool meOrYou) const {
            unsigned int total = 0;
            for (int cnt = 0; cnt < SIDE_SIZE; cnt++) {
                total += getCountAt(meOrYou, cnt);
            }
            total += getMancalaCount(meOrYou);
            return total;
        }
    protected:
        unsigned int depressions[SIDE_SIZE * NUM_SIDES];
        unsigned int mancala[NUM_SIDES];
        bool playAgain;
    private:
    };
};



#endif
