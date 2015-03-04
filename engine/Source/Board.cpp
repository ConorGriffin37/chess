#include "Board.hpp"
#include "Evaluation.hpp"
#include <iostream>

void outbitboard(u64 n);

Board::Board()
{

}

Board::Board(std::string fen)
{
    if (fen == "") {
        return;
    }
    castleorenpasent = 0;
    for (int i = 0; i < 8; i++) {
        pieceBB[i] = 0;
    }
    int pos = 63;
    int section = 0;
    int lastone = 0;
    for (int i = 0; i < fen.length(); i++) {
        if (fen[i] == ' ') {
            section++;
        } else {
            if (section == 0) {
                int x = getPieceCode(fen[i]);
                if (x == -1) {
                    if (isdigit(fen[i])) {
                        pos = pos - (fen[i] - '0');
                    }
                } else {
                    u64 shift = 1;
                    if (pos > 31) {
                        shift <<= 31;
                        shift <<= pos - 31;
                    } else {
                        shift <<= pos;
                    }
                    pieceBB[x] |= shift;
                    pieceBB[getColorCode(fen[i])] |= shift;
                    pos--;
                }
            } else if (section == 2) {
                if (fen[i] == 'K') {
                    castleorenpasent |= 1 << 7;
                } else if (fen[i] == 'Q') {
                    castleorenpasent |= 1;
                } else if (fen[i] == 'k') {
                    u64 shift = 1;
                    shift <<= 31;
                    shift <<= 56 - 31;
                    castleorenpasent |= shift;
                } else if (fen[i] == 'q') {
                    u64 shift = 1;
                    shift <<= 31;
                    shift <<= 63 - 31;
                    castleorenpasent |= shift;
                }
            } else if (section == 3) {
                if (isdigit(fen[i])) {
                    if ((8*(fen[i] - '1') + lastone) <= 32) {
                        castleorenpasent |= 1 << (8*(fen[i] - '1') + lastone);
                    } else {
                        u64 shift = 1;
                        shift <<= 31;
                        shift <<= (8*(fen[i] - '1') + lastone) - 31;
                        castleorenpasent |= shift;
                    }
                } else {
                    lastone = ('h' - fen[i]);
                }
            }
        }
    }
}

int Board::getColorCode(char p)
{
    if (p > 'a') {
        return 7;
    }
    return 6;
}

int Board::getPieceCode(char p)
{
    if (p > 'a' and p < 'z') {
        p = 'A' + (p - 'a');
    }
    if (p == 'P') {
        return 0;
    } else if (p == 'R') {
        return 1;
    } else if (p == 'N') {
        return 2;
    } else if (p == 'B') {
        return 3;
    } else if (p == 'Q') {
        return 4;
    } else if (p == 'K') {
        return 5;
    }
    return -1;
}

u64 Board::getPieces()
{
    return pieceBB[6] | pieceBB[7];
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


void Board::promotePawn(int colorcode, std::pair<int, int> from, std::pair<int, int> to, int code)
{
    u64 shift = 1;
    int ammount = from.second*8 + (7 - from.first);
    if (ammount > 31) {
        shift <<= 31;
        shift <<= ammount - 31;
    } else {
        shift <<= ammount;
    }
    pieceBB[0] &= ~(shift);
    pieceBB[colorcode] &= ~(shift);
    shift = 1;
    ammount = to.second*8 + (7 - to.first);
    if (ammount > 31) {
        shift <<= 31;
        shift <<= ammount - 31;
    } else {
        shift <<= ammount;
    }
    pieceBB[code] |= shift;
    pieceBB[colorcode] |= shift;
}

bool checkbit(u64 bitboard, int pos)
{
    u64 shift = 1;
    shift <<= pos;
    return (bitboard & shift);
}

u64 setbit(u64 bitboard, int pos)
{
    u64 shift32 = 1;
    shift32 <<= pos;
    bitboard |= shift32;
    return bitboard;
}

u64 unsetbit(u64 bitboard, int pos)
{
    u64 shift = 1;
    shift <<= pos;
    bitboard &= ~(shift);
    return bitboard;
}

int getpos(int x, int y)
{
    return y*8 + (7 - x);
}

void Board::takePiece(std::pair<int, int> position)
{
    int ammount = position.second*8 + (7 - position.first);
    for (int i = 0; i < 8; i++) {
        pieceBB[i] = unsetbit(pieceBB[i], ammount);
    }
}

void Board::makeMove(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to)
{
    pieceBB[code] = unsetbit(pieceBB[code], getpos(from.first, from.second));
    pieceBB[colorcode] = unsetbit(pieceBB[colorcode], getpos(from.first, from.second));
    pieceBB[code] = setbit(pieceBB[code], getpos(to.first, to.second));
    pieceBB[colorcode] = setbit(pieceBB[colorcode], getpos(to.first, to.second));
}

u64 Board::getAttacked(int colorcode)
{
    u64 colorboard = getPieceColor(colorcode);
    u64 oppcolorboard;
    if (colorcode == 6) {
        oppcolorboard = getPieceColor(7);
    } else {
        oppcolorboard = getPieceColor(6);
    }
    u64 bitest = 1;
    u64 attacked = 0;
    for (int i = 0; i < 64; i++) {
        int x = 7 - (i % 8);
        int y = i/8;
        if (colorboard & bitest) {
            if (getPiece(0) & bitest) { //pawn
                if (colorcode == 6) { //white
                    if (x < 7) {
                        attacked = setbit(attacked, getpos(x + 1, y + 1));
                    }
                    if (x > 0) {
                        attacked = setbit(attacked, getpos(x - 1, y + 1));
                    }
                } else if (colorcode == 7) { //black
                    if (x < 7) {
                        attacked = setbit(attacked, getpos(x + 1, y - 1));
                    }
                    if (x > 0) {
                        attacked = setbit(attacked, getpos(x - 1, y - 1));
                    }
                }
            } else if (getPiece(1) & bitest or getPiece(4) & bitest) { //rook and queen (partialy)
                for (int j = x + 1; j < 8; j++) {
                    if (checkbit(colorboard, getpos(j, y))) {
                        break;
                    }
                    if (checkbit(oppcolorboard, getpos(j, y))) {
                        attacked = setbit(attacked, getpos(j, y));
                        break;
                    }
                    attacked = setbit(attacked, getpos(j, y));
                }
                for (int j = x - 1; j >= 0; j--) {
                    if (checkbit(colorboard, getpos(j, y))) {
                        break;
                    }
                    if (checkbit(oppcolorboard, getpos(j, y))) {
                        attacked = setbit(attacked, getpos(j, y));
                        break;
                    }
                    attacked = setbit(attacked, getpos(j, y));
                }
                for (int j = y + 1; j < 8; j++) {
                    if (checkbit(colorboard, getpos(x, j))) {
                        break;
                    }
                    if (checkbit(oppcolorboard, getpos(x, j))) {
                        attacked = setbit(attacked, getpos(x, j));
                        break;
                    }
                    attacked = setbit(attacked, getpos(x, j));
                }
                for (int j = y - 1; j >= 0; j--) {
                    if (checkbit(colorboard, getpos(x, j))) {
                        break;
                    }
                    if (checkbit(oppcolorboard, getpos(x, j))) {
                        attacked = setbit(attacked, getpos(x, j));
                        break;
                    }
                    attacked = setbit(attacked, getpos(x, j));
                }
            } else if (getPiece(2) & bitest) { //knight
                if (x < 6) {
                    if (y < 7) {
                        if (checkbit(colorboard, getpos(x + 2, y + 1)) == false) {
                            attacked = setbit(attacked, getpos(x + 2, y + 1));
                        }
                    }
                    if (y > 0) {
                        if (checkbit(colorboard, getpos(x + 2, y - 1)) == false) {
                            attacked = setbit(attacked, getpos(x + 2, y - 1));
                        }
                    }
                }
                if (x > 1) {
                    if (y < 7) {
                        if (checkbit(colorboard, getpos(x - 2, y + 1)) == false) {
                            attacked = setbit(attacked, getpos(x - 2, y + 1));
                        }
                    }
                    if (y > 0) {
                        if (checkbit(colorboard, getpos(x - 2, y - 1)) == false) {
                            attacked = setbit(attacked, getpos(x - 2, y - 1));
                        }
                    }
                }
                if (y < 6) {
                    if (x < 7) {
                        if (checkbit(colorboard, getpos(x + 1, y + 2)) == false) {
                            attacked = setbit(attacked, getpos(x + 1, y + 2));
                        }
                    }
                    if (x > 0) {
                        if (checkbit(colorboard, getpos(x - 1, y + 2)) == false) {
                            attacked = setbit(attacked, getpos(x - 1, y + 2));
                        }
                    }
                }
                if (y > 1) {
                    if (x < 7) {
                        if (checkbit(colorboard, getpos(x + 1, y - 2)) == false) {
                            attacked = setbit(attacked, getpos(x + 1, y - 2));
                        }
                    }
                    if (x > 0) {
                        if (checkbit(colorboard, getpos(x - 1, y - 2)) == false) {
                            attacked = setbit(attacked, getpos(x - 1, y - 2));
                        }
                    }
                }
            } else if (getPiece(5) & bitest) { //king
                if (x < 7) {
                    if (checkbit(colorboard, getpos(x + 1, y)) == false) {
                        attacked = setbit(attacked, getpos(x + 1, y));
                    }
                    if (y < 7) {
                        if (checkbit(colorboard, getpos(x + 1, y + 1)) == false) {
                            attacked = setbit(attacked, getpos(x + 1, y + 1));
                        }
                    }
                    if (y > 0) {
                        if (checkbit(colorboard, getpos(x + 1, y - 1)) == false) {
                            attacked = setbit(attacked, getpos(x + 1, y - 1));
                        }
                    }
                }
                if (y < 7) {
                    if (checkbit(colorboard, getpos(x, y + 1)) == false) {
                        attacked = setbit(attacked, getpos(x, y + 1));
                    }
                }
                if (y > 0) {
                    if (checkbit(colorboard, getpos(x, y - 1)) == false) {
                        attacked = setbit(attacked, getpos(x, y - 1));
                    }
                }
                if (x > 0) {
                    if (checkbit(colorboard, getpos(x - 1, y)) == false) {
                        attacked = setbit(attacked, getpos(x - 1, y));
                    }
                    if (y < 7) {
                        if (checkbit(colorboard, getpos(x - 1, y + 1)) == false) {
                            attacked = setbit(attacked, getpos(x - 1, y + 1));
                        }
                    }
                    if (y > 0) {
                        if (checkbit(colorboard, getpos(x - 1, y - 1)) == false) {
                            attacked = setbit(attacked, getpos(x - 1, y - 1));
                        }
                    }
                }
            }
            if (getPiece(3) & bitest or getPiece(4) & bitest) { //bishop and queen partially
                for (int j = 1; j <= std::min(7 - x, 7 - y); j++) {
                    if (checkbit(colorboard, getpos(x + j, y + j))) {
                        break;
                    }
                    if (checkbit(oppcolorboard, getpos(x + j, y + j))) {
                        attacked = setbit(attacked, getpos(x + j, y + j));
                        break;
                    }
                    attacked = setbit(attacked, getpos(x + j, y + j));
                }
                for (int j = 1; j <= std::min(7 - x, y); j++) {
                    if (checkbit(colorboard, getpos(x + j, y - j))) {
                        break;
                    }
                    if (checkbit(oppcolorboard, getpos(x + j, y - j))) {
                        attacked = setbit(attacked, getpos(x + j, y - j));
                        break;
                    }
                    attacked = setbit(attacked, getpos(x + j, y - j));
                }
                for (int j = 1; j <= std::min(x, y); j++) {
                    if (checkbit(colorboard, getpos(x - j, y - j))) {
                        break;
                    }
                    if (checkbit(oppcolorboard, getpos(x - j, y - j))) {
                        attacked = setbit(attacked, getpos(x - j, y - j));
                        break;
                    }
                    attacked = setbit(attacked, getpos(x - j, y - j));
                }
                for (int j = 1; j <= std::min(x, 7 - y); j++) {
                    if (checkbit(colorboard, getpos(x - j, y + j))) {
                        break;
                    }
                    if (checkbit(oppcolorboard, getpos(x - j, y + j))) {
                        attacked = setbit(attacked, getpos(x - j, y + j));
                        break;
                    }
                    attacked = setbit(attacked, getpos(x - j, y + j));
                }
            }
        }
        bitest <<= 1;
    }
    return attacked;
}


bool Board::inCheck(int colorcode)
{
    if (colorcode == 6) {
        return (getAttacked(7) & getPieceAndColor(5, 6));
    }
    return (getAttacked(6) & getPieceAndColor(5, 7));
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
    u64 last = getCastleOrEnpasent();
    u64 next = 0;
    if (checkbit(last, 63)) {
        next = setbit(next, 63);
    }
    if (checkbit(last, 56)) {
        next = setbit(next, 56);
    }
    if (checkbit(last, 0)) {
        next = setbit(next, 0);
    }
    if (checkbit(last, 7)) {
        next = setbit(next, 7);
    }
    return next;
}

int Board::getPieceFromPos(int x, int y)
{
    for (int i = 0; i < 6; i++) {
        if (checkbit(getPiece(i), getpos(x, y))) {
            return i;
        }
    }
    return -1;
}

int abs(int x)
{
    if (x < 0) {
        return x*-1;
    }
    return x;
}

bool Board::simpleMakeMove(std::pair <int, int> from, std::pair <int, int> to, char promote)
{
    int pcode = getPieceFromPos(from.first, from.second);
    if (pcode == -1) {
        return false;
    }
    setCastleOrEnpas(nextCastleOrEnpasent());
    int colorcode = 6;
    if (checkbit(getPieceColor(7), getpos(from.first, from.second))) {
        colorcode = 7;
    }
    if (pcode == 0 and from.first != to.first) {
        if (checkbit(getPieces(), getpos(to.first, to.second)) == false) { //enpasent
            takePiece(std::make_pair(to.first, from.second));
            makeMove(pcode, colorcode, from, to);
            return true;
        }
    }
    if (pcode == 5) {
        if (colorcode == 6) {
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 0));
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 7));
        } else {
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 56));
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 63));
        }
        if (abs(from.first - to.first) > 1) { //castling
            if (to.first == 6) { //"e1g1" or "e8g8"
                makeMove(pcode, colorcode, from, to);
                makeMove(1, colorcode, std::make_pair(7, from.second), std::make_pair(5, from.second));
                return true;
            } else { //"e1c1" or "e8c8"
                makeMove(pcode, colorcode, from, to);
                makeMove(1, colorcode, std::make_pair(0, from.second), std::make_pair(3, from.second));
                return true;
            }
        }
    }
    if (pcode == 1) { //rook moving = no castle
        if (from.second == 0 or from.second == 7) {
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), getpos(from.first, from.second)));
        }
    }
    if (pcode == 0) { //enpasent
        if (from.second - to.second == 2) { //moving out 2 at first
            setCastleOrEnpas(setbit(getCastleOrEnpasent(), getpos(from.first, from.second - 1)));
        } else if (from.second - to.second == -2) {
            setCastleOrEnpas(setbit(getCastleOrEnpasent(), getpos(from.first, from.second + 1)));
        }
    }
    takePiece(to);
    if (promote == ' ') {
        makeMove(pcode, colorcode, from, to);
    } else {
        promotePawn(colorcode, from, to, getPieceCode(promote));
    }
    return true;
}

void Board::putPiece(int code, int colorcode, std::pair<int, int> position)
{
    pieceBB[code] = setbit(pieceBB[code], getpos(position.first, position.second));
    pieceBB[colorcode] = setbit(pieceBB[colorcode], getpos(position.first, position.second));
}

void Board::specTakePiece(int code, int colorcode, std::pair<int, int> position)
{
    pieceBB[code] = unsetbit(pieceBB[code], getpos(position.first, position.second));
    pieceBB[colorcode] = unsetbit(pieceBB[colorcode], getpos(position.first, position.second));
}

int getOppColor(int x)
{
    if (x == 6) {
        return 7;
    }
    return 6;
}

int getMultiplyColor(int x)
{
    if (x == 6) {
        return 1;
    }
    return -1;
}

void Board::makeMov(mov theMove)
{
    setCastleOrEnpas(nextCastleOrEnpasent());
    if (theMove.take) {
        int oppcolor = getOppColor(theMove.colorcode);
        specTakePiece(theMove.takecode, oppcolor, theMove.takepos);
        materialEval = materialEval - getMultiplyColor(oppcolor)*Evaluation::getPosScore(theMove.takecode, oppcolor, theMove.takepos);
    }
    if (theMove.promote) {
        promotePawn(theMove.colorcode, theMove.from, theMove.to, theMove.procode);
        materialEval = materialEval - getMultiplyColor(theMove.colorcode)*Evaluation::getPosScore(theMove.code, theMove.colorcode, theMove.from);
        materialEval = materialEval + getMultiplyColor(theMove.colorcode)*Evaluation::getPosScore(theMove.procode, theMove.colorcode, theMove.to);
    } else {
        makeMove(theMove.code, theMove.colorcode, theMove.from, theMove.to);
        materialEval = materialEval - getMultiplyColor(theMove.colorcode)*Evaluation::getPosScore(theMove.code, theMove.colorcode, theMove.from);
        materialEval = materialEval + getMultiplyColor(theMove.colorcode)*Evaluation::getPosScore(theMove.code, theMove.colorcode, theMove.to);
    }
    if (theMove.castle) {
        makeMove(1, theMove.colorcode, theMove.rookfrom, theMove.rookto);
        materialEval = materialEval - getMultiplyColor(theMove.colorcode)*Evaluation::getPosScore(1, theMove.colorcode, theMove.rookfrom);
        materialEval = materialEval + getMultiplyColor(theMove.colorcode)*Evaluation::getPosScore(1, theMove.colorcode, theMove.rookto);
    }
    if (theMove.code == 5) {
        if (theMove.colorcode == 6) {
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 0));
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 7));
        } else {
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 56));
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), 63));
        }
    }
    if (theMove.code == 1) { //rook moving = no castle
        if (theMove.from.second == 0 or theMove.from.second == 7) {
            setCastleOrEnpas(unsetbit(getCastleOrEnpasent(), getpos(theMove.from.first, theMove.from.second)));
        }
    }
    if (theMove.code == 0) { //enpasent
        if (theMove.from.second - theMove.to.second == 2) { //moving out 2 at first
            setCastleOrEnpas(setbit(getCastleOrEnpasent(), getpos(theMove.from.first, theMove.from.second - 1)));
        } else if (theMove.from.second - theMove.to.second == -2) {
            setCastleOrEnpas(setbit(getCastleOrEnpasent(), getpos(theMove.from.first, theMove.from.second + 1)));
        }
    }
}

void Board::unMakeMov(mov theMove, u64 oldCastleOrEnpas)
{
    setCastleOrEnpas(oldCastleOrEnpas);
    if (theMove.promote) {
        specTakePiece(theMove.procode, theMove.colorcode, theMove.to);
        putPiece(0, theMove.colorcode, theMove.from);
        materialEval = materialEval - getMultiplyColor(theMove.colorcode)*Evaluation::getPosScore(theMove.procode, theMove.colorcode, theMove.to);
        materialEval = materialEval + getMultiplyColor(theMove.colorcode)*Evaluation::getPosScore(theMove.code, theMove.colorcode, theMove.from);
    } else {
        makeMove(theMove.code, theMove.colorcode, theMove.to, theMove.from);
        materialEval = materialEval - getMultiplyColor(theMove.colorcode)*Evaluation::getPosScore(theMove.code, theMove.colorcode, theMove.to);
        materialEval = materialEval + getMultiplyColor(theMove.colorcode)*Evaluation::getPosScore(theMove.code, theMove.colorcode, theMove.from);
    }
    if (theMove.take) {
        int oppcolor = getOppColor(theMove.colorcode);
        putPiece(theMove.takecode, oppcolor, theMove.takepos);
        materialEval = materialEval + getMultiplyColor(oppcolor)*Evaluation::getPosScore(theMove.takecode, oppcolor, theMove.takepos);
    }
    if (theMove.castle) {
        makeMove(1, theMove.colorcode, theMove.rookto, theMove.rookfrom);
        materialEval = materialEval - getMultiplyColor(theMove.colorcode)*Evaluation::getPosScore(1, theMove.colorcode, theMove.rookto);
        materialEval = materialEval + getMultiplyColor(theMove.colorcode)*Evaluation::getPosScore(1, theMove.colorcode, theMove.rookfrom);
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
