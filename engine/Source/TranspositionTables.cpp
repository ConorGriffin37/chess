#include "Board.hpp"
#include "TranspositionTables.hpp"
#include "MoveList.hpp"

#include <iostream>
#include <algorithm>

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

entry bad;

void TranspositionTables::initZobrist()
{
    bad.depth = -1;
    bad.best = 0;
    srand(1262340);
    for (int i = 0; i < 781; i++) {
        u64 r = rand64();
        zobrist[i] = r;
        //std::cout << zobrist[i] << std::endl;
    }
}

u64 TranspositionTables::getSquareHash(int pos, int code, int playerColor)
{
    if (playerColor == WHITE_CODE) {
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
        for (int x = PAWN_CODE; x <= KING_CODE; x++) {
            if (bittest & gameBoard.getPiece(x)) {
                if (bittest & gameBoard.getPieceColor(WHITE_CODE)) {
                    rethash ^= getSquareHash(i, x, WHITE_CODE);
                } else {
                    rethash ^= getSquareHash(i, x, BLACK_CODE);
                }
            }
        }
        bittest <<= 1;
    }
    if (playerColor == BLACK_CODE) {
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

entry TranspositionTables::getBest(u64 signature)
{
    if (Table[signature & tab_mask].signature == signature) {
        return Table[signature & tab_mask];
    }
    return bad;
}

void TranspositionTables::setEntry(u64 signature, u64 bestmove, int depth, int score, int type)
{
    u64 key = signature & tab_mask;
    if (Table[key].depth < depth) {
        Table[key].signature = signature;
        Table[key].best = bestmove;
        Table[key].depth = depth;
        Table[key].score = score;
        Table[key].type = type;
        Table[key].ancient = false;
    } else if (Table[key].ancient) {
        if (Table[key].signature != signature) {
            Table[key].signature = signature;
            Table[key].best = bestmove;
            Table[key].depth = depth;
            Table[key].score = score;
            Table[key].type = type;
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

void TranspositionTables::initEntryCount()
{
    for (int i = 0; i < tab_size; i++) {
        Table[i].depth = -1;
    }
}

int TranspositionTables::getEntryCount()
{
    int TTcount = 0;
    for (int i = 0; i < tab_size; i++) {
        if (Table[i].depth > -1) {
            TTcount++;
        }
    }
    return TTcount;
}

std::string TranspositionTables::getPrincipalVariation(Board gameBoard, int depth)
{
    if (depth == 0) {
        return "";
    }
    u64 principal = getBest(gameBoard.getZorHash()).best;
    MoveList movList;
    if (principal != 0) {
        gameBoard.makeMov(principal);
        return movList.getMoveCode(principal) + " " + getPrincipalVariation(gameBoard, depth - 1);
    }
    return "";
}
