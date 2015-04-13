#include "Board.hpp"
#include "Evaluation.hpp"
#include "TranspositionTables.hpp"

#include <iostream>
#include <sstream>

void outbitboard(u64 n);
int getOppColor(int x);

u64 bmasks[64];

void initMasks()
{
    u64 bittest = 1;
    for (int i = 0; i < 64; i++) {
        bmasks[i] = bittest;
        bittest <<= 1;
    }
}

bool checkbit(u64 bitboard, int pos)
{
    return bitboard & bmasks[pos];
}


u64 unsetbit(u64 bitboard, int pos)
{
    bitboard &= ~(bmasks[pos]);
    return bitboard;
}

u64 setbit(u64 bitboard, int pos)
{
    bitboard |= bmasks[pos];
    return bitboard;
}

const int index64[64] = {
    0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};

int bitScanForward(u64 bb)
{
   const u64 debruijn64 = u64(0x03f79d71b4cb0a89);
   return index64[((bb ^ (bb-1)) * debruijn64) >> 58];
}

int getpos(int x, int y)
{
    return y*8 + (7 - x);
}

Board::Board()
{

}

Board::Board(std::string fen)
{
    if (fen == "") {
        return;
    }

    castleorenpasent = 0;
    enpasentCol = -1;
    for (int i = 0; i < 8; i++) {
        pieceBB[i] = 0;
    }
    std::stringstream ss(fen);
    std::string boardRepresentation;
    std::getline(ss, boardRepresentation, ' ');

    int square = 63;
    for (unsigned int i = 0; i < boardRepresentation.length(); i++) {
        int pieceCode = getPieceCode(boardRepresentation[i]);
        if (pieceCode == -1) {
            if (isdigit(boardRepresentation[i])) {
                square = square - (boardRepresentation[i] - '0');
            }
        } else {
            pieceBB[pieceCode] = setbit(pieceBB[pieceCode], square);
            pieceBB[getColorCode(boardRepresentation[i])] = setbit(pieceBB[getColorCode(boardRepresentation[i])], square);
            square--;
        }
    }

    std::string activeColor;
    std::getline(ss, activeColor, ' ');
    //Active color is handled by UCI currently, could be rewritten in the future

    std::string castlingAvailability;
    std::getline(ss, castlingAvailability, ' ');
    for (unsigned int i = 0; i < castlingAvailability.length(); i++) {
        if (castlingAvailability[i] == 'K') {
            castleorenpasent = setbit(castleorenpasent, 7);
        } else if (castlingAvailability[i] == 'Q') {
            castleorenpasent = setbit(castleorenpasent, 0);
        } else if (castlingAvailability[i] == 'k') {
            castleorenpasent = setbit(castleorenpasent, 56);
        } else if (castlingAvailability[i] == 'q') {
            castleorenpasent = setbit(castleorenpasent, 63);
        }
    }

    std::string enpassentSquare;
    std::getline(ss, enpassentSquare, ' ');
    if (enpassentSquare != "-") {
        enpasentCol = 'h' - enpassentSquare[0];
        castleorenpasent = setbit(castleorenpasent, (8*(enpassentSquare[1] - '1') + enpasentCol));
    }

    halfMoveClock = 0;
    std::string halfMoveClockString;
    std::getline(ss, halfMoveClockString, ' ');
    if (halfMoveClockString != "-") {
        std::istringstream iss(halfMoveClockString);
        iss >> halfMoveClock;
    }
}

int Board::getColorCode(char p)
{
    return ((p > 'a') ? BLACK_CODE : WHITE_CODE);
}

int Board::getPieceCode(char p)
{
    if (p > 'a' and p < 'z') {
        p = 'A' + (p - 'a');
    }
    if (p == 'P') {
        return PAWN_CODE;
    } else if (p == 'R') {
        return ROOK_CODE;
    } else if (p == 'N') {
        return KNIGHT_CODE;
    } else if (p == 'B') {
        return BISHOP_CODE;
    } else if (p == 'Q') {
        return QUEEN_CODE;
    } else if (p == 'K') {
        return KING_CODE;
    }
    return -1;
}

u64 Board::getPieces()
{
    return pieceBB[WHITE_CODE] | pieceBB[BLACK_CODE];
}

u64 Board::getPiece(int code)
{
    return pieceBB[code];
}

u64 Board::getPieceAndColor(int code, int colorcode)
{
    return pieceBB[code] & pieceBB[colorcode];
}

u64 Board::getPieceColor(int colorcode)
{
    return getPieces() & pieceBB[colorcode];
}

u64 Board::getCastleOrEnpasent()
{
    return castleorenpasent;
}


void Board::promotePawn(int colorcode, int from, int to, int code)
{
    pieceBB[PAWN_CODE] = unsetbit(pieceBB[PAWN_CODE], from);
    pieceBB[colorcode] = unsetbit(pieceBB[colorcode], from);
    pieceBB[code] = setbit(pieceBB[code], to);
    pieceBB[colorcode] = setbit(pieceBB[colorcode], to);
}


void Board::takePiece(std::pair<int, int> position)
{
    int ammount = position.second*8 + (7 - position.first);
    for (int i = 0; i < 8; i++) {
        pieceBB[i] = unsetbit(pieceBB[i], ammount);
    }
}

void Board::makeMove(int code, int colorcode, int from, int to)
{
    pieceBB[code] = unsetbit(pieceBB[code], from);
    pieceBB[colorcode] = unsetbit(pieceBB[colorcode], from);
    pieceBB[code] = setbit(pieceBB[code], to);
    pieceBB[colorcode] = setbit(pieceBB[colorcode], to);
}

bool Board::getAttackedPawn(int colorcode, int pos, u64 oppcolorboard)
{
    u64 opppawnboard = oppcolorboard & getPiece(PAWN_CODE);
    int x = 7 - (pos % 8);
    if (colorcode == WHITE_CODE) {
        if (pos + 8 < 64) {
            if (x > 0) {
                if (checkbit(opppawnboard, pos + 9)) {
                    return true;
                }
            }
            if (x < 7) {
                if (checkbit(opppawnboard, pos + 7)) {
                    return true;
                }
            }
        }
    } else {
        if (pos - 8 >= 0) {
            if (x > 0) {
                if (checkbit(opppawnboard, pos - 7)) {
                    return true;
                }
            }
            if (x < 7) {
                if (checkbit(opppawnboard, pos - 9)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::getAttackedKing(int pos, u64 oppcolorboard)
{
    int opppos = bitScanForward(oppcolorboard & getPiece(KING_CODE));
    int x = 7 - (pos % 8);
    int diff = opppos - pos;
    if (diff == 1 and x > 0) {
        return true;
    }
    if (diff == -1 and x < 7) {
        return true;
    }
    if (diff == 8 or diff == -8) {
        return true;
    }
    if (diff == 9 or diff == -7) {
        if (x > 0) {
            return true;
        }
    }
    if (diff == 7 or diff == -9) {
        if (x < 7) {
            return true;
        }
    }
    return false;
}

bool Board::getAttackedKnight(int pos, u64 oppcolorboard)
{
    u64 knightboard = getPiece(KNIGHT_CODE) & oppcolorboard;
    int x = 7 - (pos % 8);
    int y = pos/8;
    if (x < 6) {
        if (y < 7) {
            if (checkbit(knightboard, pos + 6)) {
                return true;
            }
        }
        if (y > 0) {
            if (checkbit(knightboard, pos - 10)) {
                return true;
            }
        }
    }
    if (x > 1) {
        if (y < 7) {
            if (checkbit(knightboard, pos + 10)) {
                return true;
            }
        }
        if (y > 0) {
            if (checkbit(knightboard, pos - 6)) {
                return true;
            }
        }
    }
    if (y < 6) {
        if (x < 7) {
            if (checkbit(knightboard, pos + 15)) {
                return true;
            }
        }
        if (x > 0) {
            if (checkbit(knightboard, pos + 17)) {
                return true;
            }
        }
    }
    if (y > 1) {
        if (x < 7) {
            if (checkbit(knightboard, pos - 17)) {
                return true;
            }
        }
        if (x > 0) {
            if (checkbit(knightboard, pos - 15)) {
                return true;
            }
        }
    }
    return false;
}

bool Board::getAttackedBishopQueen(int pos, u64 oppbisqueen)
{
    u64 occ = getPieces();
    int x = 7 - (pos % 8);
    int y = pos/8;
    for (int j = 1; j <= std::min(7 - x, 7 - y); j++) {
        if (checkbit(oppbisqueen, pos + 7*j)) {
            return true;
        }
        if (checkbit(occ, pos + 7*j)) {
            break;
        }
    }
    for (int j = 1; j <= std::min(7 - x, y); j++) {
        if (checkbit(oppbisqueen, pos - 9*j)) {
            return true;
        }
        if (checkbit(occ, pos - 9*j)) {
            break;
        }
    }
    for (int j = 1; j <= std::min(x, y); j++) {
        if (checkbit(oppbisqueen, pos - 7*j)) {
            return true;
        }
        if (checkbit(occ, pos - 7*j)) {
            break;
        }
    }
    for (int j = 1; j <= std::min(x, 7 - y); j++) {
        if (checkbit(oppbisqueen, pos + 9*j)) {
            return true;
        }
        if (checkbit(occ, pos + 9*j)) {
            break;
        }
    }
    return false;
}

bool Board::getAttackedRookQueen(int pos, u64 opprookqueen)
{
    u64 occ = getPieces();
    int x = 7 - (pos % 8);
    int y = pos/8;
    for (int j = 1; x + j < 8; j++) {
        if (checkbit(opprookqueen, pos - j)) {
            return true;
        }
        if (checkbit(occ, pos - j)) {
            break;
        }
    }
    for (int j = 1; x - j >= 0; j++) {
        if (checkbit(opprookqueen, pos + j)) {
            return true;
        }
        if (checkbit(occ, pos + j)) {
            break;
        }
    }
    for (int j = 1; y + j < 8; j++) {
        if (checkbit(opprookqueen, pos + j*8)) {
            return true;
        }
        if (checkbit(occ, pos + j*8)) {
            break;
        }
    }
    for (int j = 1; y - j >= 0; j++) {
        if (checkbit(opprookqueen, pos - j*8)) {
            return true;
        }
        if (checkbit(occ, pos - j*8)) {
            break;
        }
    }
    return false;
}

bool Board::inCheck(int colorcode)
{
    u64 oppcolorboard;
    if (colorcode == WHITE_CODE) {
        oppcolorboard = getPieceColor(BLACK_CODE);
    } else {
        oppcolorboard = getPieceColor(WHITE_CODE);
    }
    u64 opprookqueen = (oppcolorboard & getPiece(QUEEN_CODE)) | (getPiece(ROOK_CODE) & oppcolorboard);
    u64 oppbisqueen = (oppcolorboard & getPiece(QUEEN_CODE)) | (getPiece(BISHOP_CODE) & oppcolorboard);
    int pos = bitScanForward(getPieceAndColor(KING_CODE, colorcode));
    if (getAttackedPawn(colorcode, pos, oppcolorboard)) {
        return true;
    }
    if (getAttackedKing(pos, oppcolorboard)) {
        return true;
    }
    if (getAttackedKnight(pos, oppcolorboard)) {
        return true;
    }
    if (getAttackedBishopQueen(pos, oppbisqueen)) {
        return true;
    }
    if (getAttackedRookQueen(pos, opprookqueen)) {
        return true;
    }
    return false;
}

bool Board::getAttacked(int colorcode, int pos)
{
    u64 oppcolorboard;
    if (colorcode == WHITE_CODE) {
        oppcolorboard = getPieceColor(BLACK_CODE);
    } else {
        oppcolorboard = getPieceColor(WHITE_CODE);
    }
    u64 opprookqueen = (oppcolorboard & getPiece(QUEEN_CODE)) | (getPiece(ROOK_CODE) & oppcolorboard);
    u64 oppbisqueen = (oppcolorboard & getPiece(QUEEN_CODE)) | (getPiece(BISHOP_CODE) & oppcolorboard);
    if (getAttackedPawn(colorcode, pos, oppcolorboard)) {
        return true;
    }
    if (getAttackedKing(pos, oppcolorboard)) {
        return true;
    }
    if (getAttackedKnight(pos, oppcolorboard)) {
        return true;
    }
    if (getAttackedBishopQueen(pos, oppbisqueen)) {
        return true;
    }
    if (getAttackedRookQueen(pos, opprookqueen)) {
        return true;
    }
    return false;
}

void Board::setBB(int code, u64 value)
{
    pieceBB[code] = value;
}

void Board::setCastleOrEnpas(u64 value)
{
    castleorenpasent = value;
}

u64 Board::nextCastleOrEnpasent()
{
    return getCastleOrEnpasent() & CASTLE_OR_ENPASENT_BASIC;
}

int Board::getPieceFromPos(int pos)
{
    for (int i = 0; i < 6; i++) {
        if (checkbit(getPiece(i), pos)) {
            return i;
        }
    }
    return -1;
}

int abs(int x)
{
    return ((x < 0) ? x*-1 : x);
}

bool Board::simpleMakeMove(std::pair <int, int> from, std::pair <int, int> to, char promote)
{
    halfMoveClock++;
    if (checkbit(getPieces(), getpos(to.first, to.second)) == true) {
        halfMoveClock = 0;
    }
    int pcode = getPieceFromPos(getpos(from.first, from.second));
    if (pcode == -1) {
        return false;
    }
    setCastleOrEnpas(nextCastleOrEnpasent());
    enpasentCol = -1;
    int colorcode = WHITE_CODE;
    if (checkbit(getPieceColor(BLACK_CODE), getpos(from.first, from.second))) {
        colorcode = BLACK_CODE;
    }
    if (pcode == PAWN_CODE and from.first != to.first) {
        if (checkbit(getPieces(), getpos(to.first, to.second)) == false) { //enpasent
            halfMoveClock = 0;
            takePiece(std::make_pair(to.first, from.second));
            makeMove(pcode, colorcode, getpos(from.first, from.second), getpos(to.first, to.second));
            return true;
        }
    }
    if (pcode == KING_CODE) {
        if (colorcode == WHITE_CODE) {
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 0));
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 7));
        } else {
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 56));
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 63));
        }
        if (abs(from.first - to.first) > 1) { //castling
            if (to.first == 6) { //"e1g1" or "e8g8"
                makeMove(pcode, colorcode, getpos(from.first, from.second), getpos(to.first, to.second));
                makeMove(ROOK_CODE, colorcode, getpos(7, from.second), getpos(5, from.second));
                return true;
            } else { //"e1c1" or "e8c8"
                makeMove(pcode, colorcode, getpos(from.first, from.second), getpos(to.first, to.second));
                makeMove(ROOK_CODE, colorcode, getpos(0, from.second), getpos(3, from.second));
                return true;
            }
        }
    }
    if (pcode == ROOK_CODE) { //rook moving = no castle
        if (from.second == 0 or from.second == 7) {
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), getpos(from.first, from.second)));
        }
    }
    if (pcode == PAWN_CODE) { //enpasent
        halfMoveClock = 0;
        if (from.second - to.second == 2) { //moving out 2 at first
            setCastleOrEnpas(setbit(getCastleOrEnpasent(), getpos(from.first, from.second - 1)));
            enpasentCol = 7 - from.first;
        } else if (from.second - to.second == -2) {
            setCastleOrEnpas(setbit(getCastleOrEnpasent(), getpos(from.first, from.second + 1)));
            enpasentCol = 7 - from.first;
        }
    }
    takePiece(to);
    if (promote == ' ') {
        makeMove(pcode, colorcode, getpos(from.first, from.second), getpos(to.first, to.second));
    } else {
        promotePawn(colorcode, getpos(from.first, from.second), getpos(to.first, to.second), getPieceCode(promote));
    }
    return true;
}

void Board::putPiece(int code, int colorcode, int position)
{
    pieceBB[code] = setbit(pieceBB[code], position);
    pieceBB[colorcode] = setbit(pieceBB[colorcode], position);
}

void Board::specTakePiece(int code, int colorcode, int position)
{
    pieceBB[code] = unsetbit(pieceBB[code], position);
    pieceBB[colorcode] = unsetbit(pieceBB[colorcode], position);
}

int getOppColor(int x)
{
    return ((x == WHITE_CODE) ? BLACK_CODE : WHITE_CODE);
}

int getMultiplyColor(int x)
{
    return ((x == WHITE_CODE) ? 1 : -1);
}

u64 mask_3 = 0b111;
u64 mask_6 = 0b111111;

void Board::makeMov(u64 theMove)
{
    halfMoveClock++;
    int code = theMove & mask_3;
    theMove >>= 3;
    int colorcode = theMove & mask_3;
    theMove >>= 3;
    int from = theMove & mask_6;
    theMove >>= 6;
    int to = theMove & mask_6;
    theMove >>= 6;
    setCastleOrEnpas(nextCastleOrEnpasent());
    zorHash ^= TranspositionTables::getBlackHash();
    if (enpasentCol > -1) {
        zorHash ^= TranspositionTables::getEnpasentHash(enpasentCol);
        enpasentCol = -1;
    }
    if (theMove & 1) {
        halfMoveClock = 0;
        int oppcolor = getOppColor(colorcode);
        theMove >>= 1;
        int takepos = theMove & mask_6;
        theMove >>= 6;
        int takecode = theMove & mask_3;
        theMove >>= 3;
        specTakePiece(takecode, oppcolor, takepos);
        zorHash ^= TranspositionTables::getSquareHash(takepos, takecode, oppcolor);
        materialEval = materialEval - getMultiplyColor(oppcolor)*Evaluation::getPosScore(takecode, oppcolor, takepos, stageOfGame);
    } else {
        theMove >>= 10;
    }
    if (theMove & 1) {
        halfMoveClock = 0;
        theMove >>= 1;
        int procode = theMove & mask_3;
        theMove >>= 3;
        promotePawn(colorcode, from, to, procode);
        zorHash ^= TranspositionTables::getSquareHash(from, 0, colorcode);
        zorHash ^= TranspositionTables::getSquareHash(to, procode, colorcode);
        materialEval = materialEval - getMultiplyColor(colorcode)*Evaluation::getPosScore(code, colorcode, from, stageOfGame);
        materialEval = materialEval + getMultiplyColor(colorcode)*Evaluation::getPosScore(procode, colorcode, to, stageOfGame);
    } else {
        theMove >>= 4;
        makeMove(code, colorcode, from, to);
        zorHash ^= TranspositionTables::getSquareHash(from, code, colorcode);
        zorHash ^= TranspositionTables::getSquareHash(to, code, colorcode);
        materialEval = materialEval - getMultiplyColor(colorcode)*Evaluation::getPosScore(code, colorcode, from, stageOfGame);
        materialEval = materialEval + getMultiplyColor(colorcode)*Evaluation::getPosScore(code, colorcode, to, stageOfGame);
    }
    if (theMove & 1) {
        theMove >>= 1;
        int rookfrom = theMove & mask_6;
        theMove >>= 6;
        int rookto = theMove & mask_6;
        theMove >>= 6;
        makeMove(1, colorcode, rookfrom, rookto);
        zorHash ^= TranspositionTables::getSquareHash(rookfrom, 1, colorcode);
        zorHash ^= TranspositionTables::getSquareHash(rookto, 1, colorcode);
        materialEval = materialEval - getMultiplyColor(colorcode)*Evaluation::getPosScore(1, colorcode, rookfrom, stageOfGame);
        materialEval = materialEval + getMultiplyColor(colorcode)*Evaluation::getPosScore(1, colorcode, rookto, stageOfGame);
    }
    if (code == KING_CODE) {
        if (colorcode == WHITE_CODE) {
            if (checkbit(getCastleOrEnpasent(), 0)) {
                zorHash ^= TranspositionTables::getCastleHash(0);
                setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 0));
            }
            if (checkbit(getCastleOrEnpasent(), 7)) {
                zorHash ^= TranspositionTables::getCastleHash(1);
                setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 7));
            }
        } else {
            if (checkbit(getCastleOrEnpasent(), 56)) {
                zorHash ^= TranspositionTables::getCastleHash(2);
                setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 56));
            }
            if (checkbit(getCastleOrEnpasent(), 63)) {
                zorHash ^= TranspositionTables::getCastleHash(3);
                setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 63));
            }
        }
    }
    if (code == ROOK_CODE) { //rook moving = no castle
        if (checkbit(getCastleOrEnpasent(), from)) {
            zorHash ^= TranspositionTables::getCastleSquare(from);
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), from));
        }
    }
    if (code == PAWN_CODE) { //enpasent
        halfMoveClock = 0;
        if (to - from == 16) { //moving out 2 at first
            setCastleOrEnpas(setbit(getCastleOrEnpasent(), to - 8));
            enpasentCol = to % 8;
        } else if (to - from == -16) {
            setCastleOrEnpas(setbit(getCastleOrEnpasent(), to + 8));
            enpasentCol = to % 8;
        }
    }
    if (enpasentCol > -1) {
        zorHash ^= TranspositionTables::getEnpasentHash(enpasentCol);
    }
}

void Board::unMakeMov(u64 theMove, u64 oldCastleOrEnpas, int lastEnpasent, u64 oldHash, int halfMoveNumber)
{
    zorHash = oldHash;
    setCastleOrEnpas(oldCastleOrEnpas);
    enpasentCol = lastEnpasent;
    halfMoveClock = halfMoveNumber;
    int code = theMove & mask_3;
    theMove >>= 3;
    int colorcode = theMove & mask_3;
    theMove >>= 3;
    int from = theMove & mask_6;
    theMove >>= 6;
    int to = theMove & mask_6;
    theMove >>= 6;
    bool take = theMove & 1;
    theMove >>= 1;
    int takepos = theMove & mask_6;
    theMove >>= 6;
    int takecode = theMove & mask_3;
    theMove >>= 3;
    if (theMove & 1) {
        theMove >>= 1;
        int procode = theMove & mask_3;
        theMove >>= 3;
        specTakePiece(procode, colorcode, to);
        putPiece(PAWN_CODE, colorcode, from);
        materialEval = materialEval - getMultiplyColor(colorcode)*Evaluation::getPosScore(procode, colorcode, to, stageOfGame);
        materialEval = materialEval + getMultiplyColor(colorcode)*Evaluation::getPosScore(code, colorcode, from, stageOfGame);
    } else {
        theMove >>= 4;
        makeMove(code, colorcode, to, from);
        materialEval = materialEval - getMultiplyColor(colorcode)*Evaluation::getPosScore(code, colorcode, to, stageOfGame);
        materialEval = materialEval + getMultiplyColor(colorcode)*Evaluation::getPosScore(code, colorcode, from, stageOfGame);
    }
    if (take) {
        int oppcolor = getOppColor(colorcode);
        putPiece(takecode, oppcolor, takepos);
        materialEval = materialEval + getMultiplyColor(oppcolor)*Evaluation::getPosScore(takecode, oppcolor, takepos, stageOfGame);
    }
    if (theMove & 1) {
        theMove >>= 1;
        int rookfrom = theMove & mask_6;
        theMove >>= 6;
        int rookto = theMove & mask_6;
        makeMove(ROOK_CODE, colorcode, rookto, rookfrom);
        materialEval = materialEval - getMultiplyColor(colorcode)*Evaluation::getPosScore(1, colorcode, rookto, stageOfGame);
        materialEval = materialEval + getMultiplyColor(colorcode)*Evaluation::getPosScore(1, colorcode, rookfrom, stageOfGame);
    }
}

void Board::setEvaluation(int eval)
{
    materialEval = eval;
}

int Board::getEvaluation()
{
    return materialEval;
}

void Board::setEnpasentCol(int x)
{
    enpasentCol = x;
}

int Board::getEnpasentCol()
{
    return enpasentCol;
}

void Board::setZorHash(u64 x)
{
    zorHash = x;
}

u64 Board::getZorHash()
{
    return zorHash;
}
