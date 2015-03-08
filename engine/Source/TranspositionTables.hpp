#ifndef Transposition_HPP_INCLUDED
#define Transposition_HPP_INCLUDED

#define tab_size 1048576 //4194304
#define tab_mask 0b1111111111111111111
#include <string>
#include <vector>
#include <utility>
#include "Board.hpp"
#include <string>

struct entry
{
    u64 signature;
    mov best;
    int depth;
    int score;
    bool ancient = false;
};

class TranspositionTables
{
    private:
        static u64 zobrist[781];
        static entry Table[tab_size];

    public:
        static void initZobrist();
        static u64 getSquareHash(int pos, int code, int playerColor);
        static u64 getBlackHash();
        static u64 getCastleHash(int n);
        static u64 getEnpasentHash(int x);
        static u64 getBoardHash(Board& gameBoard, int playerColor);
        static u64 getCastleSquare(int pos);
        static mov getBest(u64 signature);
        static void setEntry(u64 signature, mov bestmove, int depth, int score);
        static void setOld();
};


#endif // Transposition_HPP_INCLUDED
