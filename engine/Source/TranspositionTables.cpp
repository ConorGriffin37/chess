#include "Board.hpp"
#include <algorithm>
#include "TranspositionTables.hpp"
#include <iostream>

entry test;
u64 TranspositionTables::zobrist[] = {0};
entry TranspositionTables::Table[] = {test};

void outbitboard(u64 n);
bool checkbit(u64 bitboard, int pos);
u64 setbit(u64 bitboard, int pos);
u64 unsetbit(u64 bitboard, int pos);
int getpos(int x, int y);

u64 rand64()
{
    return rand() ^ ((u64)rand() << 15) ^ ((u64)rand() << 30) ^ ((u64)rand() << 45) ^ ((u64)rand() << 60);
}

mov bad;

void TranspositionTables::initZobrist()
{
    bad.code = -1;
    srand(1262340);
    for (int i = 0; i < 781; i++) {
        u64 r = rand64();
        zobrist[i] = r;
        //std::cout << zobrist[i] << std::endl;
    }
}

u64 TranspositionTables::getSquareHash(int pos, int code, int playerColor)
{
    if (playerColor == 6) {
        return zobrist[code*64 + pos];
    } else {
        return zobrist[code*64 + pos + 6*64];
    }
}

u64 TranspositionTables::getBlackHash()
{
    return zobrist[64*12];
}

u64 TranspositionTables::getCastleHash(int n)
{
    return zobrist[64*12 + 1 + n];
}

u64 TranspositionTables::getCastleSquare(int pos)
{
    if (pos == 0) {
        return getCastleHash(0);
    } else if (pos == 7) {
        return getCastleHash(1);
    } else if (pos == 56) {
        return getCastleHash(2);
    } else {
        return getCastleHash(3);
    }
}

u64 TranspositionTables::getEnpasentHash(int x)
{
    return zobrist[64*12 + 5 + x];
}

u64 TranspositionTables::getBoardHash(Board& gameBoard, int playerColor)
{
    u64 bittest = 1;
    u64 rethash = 0;
    for (int i = 0; i < 64; i++) {
        for (int x = 0; x < 5; x++) {
            if (bittest & gameBoard.getPiece(x)) {
                if (bittest & gameBoard.getPieceColor(6)) {
                    rethash ^= getSquareHash(i, x, 6);
                } else {
                    rethash ^= getSquareHash(i, x, 7);
                }
            }
        }
        bittest <<= 1;
    }
    if (playerColor == 7) {
        rethash ^= getBlackHash();
    }
    if (checkbit(gameBoard.getCastleOrEnpasent(), 0)) {
        rethash ^= getCastleHash(0);
    }
    if (checkbit(gameBoard.getCastleOrEnpasent(), 7)) {
        rethash ^= getCastleHash(1);
    }
    if (checkbit(gameBoard.getCastleOrEnpasent(), 56)) {
        rethash ^= getCastleHash(2);
    }
    if (checkbit(gameBoard.getCastleOrEnpasent(), 63)) {
        rethash ^= getCastleHash(3);
    }
    if (gameBoard.getEnpasentCol() > -1) {
        rethash ^= getEnpasentHash(gameBoard.getEnpasentCol());
    }
    return rethash;
}

mov TranspositionTables::getBest(u64 signature)
{
    if (Table[signature & tab_mask].signature == signature) {
        return Table[signature & tab_mask].best;
    }
    return bad;
}

void TranspositionTables::setEntry(u64 signature, mov bestmove, int depth, int score)
{
    u64 key = signature & tab_mask;
    if (Table[key].depth < depth) {
        Table[key].signature = signature;
        Table[key].best = bestmove;
        Table[key].depth = depth;
        Table[key].score = score;
        Table[key].ancient = false;
    } else if (Table[key].ancient) {
        if (Table[key].signature != signature) {
            Table[key].signature = signature;
            Table[key].best = bestmove;
            Table[key].depth = depth;
            Table[key].score = score;
            Table[key].ancient = false;
        }
    }
}

void TranspositionTables::setOld()
{
    for (int i = 0; i < tab_size; i++) {
        Table[i].ancient = true;
    }
}


std::string TranspositionTables::getPrincipalVariation(Board gameBoard, int depth)
{
    if (depth == 0) {
        return "";
    }
    mov principal = getBest(gameBoard.getZorHash());
    MoveList movList;
    if (principal.code != -1) {
        gameBoard.makeMov(principal);
        return movList.getMoveCode(principal) + " " + getPrincipalVariation(gameBoard, depth - 1);
    }
    return "";
}



