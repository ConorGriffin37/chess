#include "board.hpp"
#include <iostream>

void outbitboard(u64 n);

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
    u64 ans = 0;
    for (int i = 0; i < 6; i++) {
        ans |= pieceBB[i];
    }
    return ans;
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

void Board::makemove(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to)
{
    u64 ex = pieceBB[0];
    u64 shift = 1;
    int ammount = from.second*8 + (7 - from.first);
    if (ammount > 31) {
        shift <<= 31;
        shift <<= ammount - 31;
    } else {
        shift <<= ammount;
    }
    pieceBB[code] &= ~(shift);
    pieceBB[colorcode] &= ~(shift);
    ammount = to.second*8 + (7 - to.first);
    shift = 1;
    if (ammount > 31) {
        shift <<= 31;
        shift <<= ammount - 31;
    } else {
        shift <<= ammount;
    }
    pieceBB[code] |= shift;
    pieceBB[colorcode] |= shift;
}

void Board::promotepawn(int colorcode, std::pair<int, int> from, std::pair<int, int> to, int code)
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

void Board::takepiece(std::pair<int, int> position)
{
    u64 shift = 1;
    int ammount = position.second*8 + (7 - position.first);
    if (ammount > 31) {
        shift <<= 32;
        shift <<= ammount - 32;
    } else {
        shift <<= ammount;
    }
    for (int i = 0; i < 8; i++) {
        pieceBB[i] &= ~(shift);
    }
}

int scores[5] = {100, 500, 310, 320, 900};
//pawn, rook, knight, bishop, queen
int positionalScores[2][5][64] =  {
                                {{ //white pawn positional table
                                    0,  0,  0,  0,  0,  0,  0,  0,
                                    5, 10, 10,-20,-20, 10, 10,  5,
                                    5, -5,-10,  0,  0,-10, -5,  5,
                                    0,  0,  0, 20, 20,  0,  0,  0,
                                    5,  5, 10, 25, 25, 10,  5,  5,
                                    10, 10, 20, 30, 30, 20, 10, 10,
                                    50, 50, 50, 50, 50, 50, 50, 50,
                                    0,  0,  0,  0,  0,  0,  0,  0
                                },
                                { //white rook positional table
                                    0,  0,  0,  5,  5,  0,  0,  0,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                    5, 10, 10, 10, 10, 10, 10,  5,
                                    0,  0,  0,  0,  0,  0,  0,  0,
                                },
                                { //white knight positional table
                                    -50,-40,-30,-30,-30,-30,-40,-50,
                                    -40,-20,  0,  5,  5,  0,-20,-40,
                                    -30,  5, 10, 15, 15, 10,  5,-30,
                                    -30,  0, 15, 20, 20, 15,  0,-30,
                                    -30,  5, 15, 20, 20, 15,  5,-30,
                                    -30,  0, 10, 15, 15, 10,  0,-30,
                                    -40,-20,  0,  0,  0,  0,-20,-40,
                                    -50,-40,-30,-30,-30,-30,-40,-50,
                                },
                                { //white bishop positional table
                                    -20,-10,-10,-10,-10,-10,-10,-20,
                                    -10,  5,  0,  0,  0,  0,  5,-10,
                                    -10, 10, 10, 10, 10, 10, 10,-10,
                                    -10,  0, 10, 10, 10, 10,  0,-10,
                                    -10,  5,  5, 10, 10,  5,  5,-10,
                                    -10,  0,  5, 10, 10,  5,  0,-10,
                                    -10,  0,  0,  0,  0,  0,  0,-10,
                                    -20,-10,-10,-10,-10,-10,-10,-20,
                                },
                                { //white queen positional table
                                    -20,-10,-10, -5, -5,-10,-10,-20,
                                    -10,  0,  5,  0,  0,  0,  0,-10,
                                    -10,  5,  5,  5,  5,  5,  0,-10,
                                     0,  0,  5,  5,  5,  5,  0, -5,
                                    -5,  0,  5,  5,  5,  5,  0, -5,
                                    -10,  0,  5,  5,  5,  5,  0,-10,
                                    -10,  0,  0,  0,  0,  0,  0,-10,
                                    -20,-10,-10, -5, -5,-10,-10,-20,
                                }},
                                {{ //black pawn positional table
                                    0,  0,  0,  0,  0,  0,  0,  0,
                                    50, 50, 50, 50, 50, 50, 50, 50,
                                    10, 10, 20, 30, 30, 20, 10, 10,
                                     5,  5, 10, 25, 25, 10,  5,  5,
                                     0,  0,  0, 20, 20,  0,  0,  0,
                                     5, -5,-10,  0,  0,-10, -5,  5,
                                     5, 10, 10,-20,-20, 10, 10,  5,
                                     0,  0,  0,  0,  0,  0,  0,  0
                                },
                                { //black rook positional table
                                    0,  0,  0,  0,  0,  0,  0,  0,
                                    5, 10, 10, 10, 10, 10, 10,  5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                    0,  0,  0,  5,  5,  0,  0,  0,
                                },
                                { //black knight positional table
                                    -50,-40,-30,-30,-30,-30,-40,-50,
                                    -40,-20,  0,  0,  0,  0,-20,-40,
                                    -30,  0, 10, 15, 15, 10,  0,-30,
                                    -30,  5, 15, 20, 20, 15,  5,-30,
                                    -30,  0, 15, 20, 20, 15,  0,-30,
                                    -30,  5, 10, 15, 15, 10,  5,-30,
                                    -40,-20,  0,  5,  5,  0,-20,-40,
                                    -50,-40,-30,-30,-30,-30,-40,-50,
                                },
                                { //black bishop positional table
                                    -20,-10,-10,-10,-10,-10,-10,-20,
                                    -10,  0,  0,  0,  0,  0,  0,-10,
                                    -10,  0,  5, 10, 10,  5,  0,-10,
                                    -10,  5,  5, 10, 10,  5,  5,-10,
                                    -10,  0, 10, 10, 10, 10,  0,-10,
                                    -10, 10, 10, 10, 10, 10, 10,-10,
                                    -10,  5,  0,  0,  0,  0,  5,-10,
                                    -20,-10,-10,-10,-10,-10,-10,-20,
                                },
                                { //black queen positional table
                                    -20,-10,-10, -5, -5,-10,-10,-20,
                                    -10,  0,  0,  0,  0,  0,  0,-10,
                                    -10,  0,  5,  5,  5,  5,  0,-10,
                                     -5,  0,  5,  5,  5,  5,  0, -5,
                                      0,  0,  5,  5,  5,  5,  0, -5,
                                    -10,  5,  5,  5,  5,  5,  0,-10,
                                    -10,  0,  5,  0,  0,  0,  0,-10,
                                    -20,-10,-10, -5, -5,-10,-10,-20,
                                }}};

int Board::evalutateBoard()
{
    int whitescore = 0;
    int blackscore = 0;
    u64 bittest = 1;
    u64 colorboard = getPieceColor(6);
    for (int i = 0; i < 64; i++) {
        for (int x = 0; x < 5; x++) {
            if (getPiece(x) & bittest) {
                if (colorboard & bittest) {
                    whitescore = whitescore + scores[x];
                    whitescore = whitescore + positionalScores[0][x][63 - i];
                    break;
                } else {
                    blackscore = blackscore + scores[x];
                    blackscore = blackscore + positionalScores[1][x][63 - i];
                    break;
                }
            }
        }
        bittest <<= 1;
    }
    return whitescore - blackscore;
}

bool checkbit(u64 bitboard, int pos)
{
    if (pos > 31) {
        u64 shift32 = 1;
        shift32 <<= 31;
        shift32 <<= pos - 31;
        return (bitboard & shift32);
    }
    u64 shift32 = 1;
    shift32 <<= pos;
    return (bitboard & shift32);
}

u64 setbit(u64 bitboard, int pos)
{
    if (pos > 31) {
        u64 shift32 = 1;
        shift32 <<= 31;
        shift32 <<= pos - 31;
        bitboard |= shift32;
        return bitboard;
    }
    u64 shift32 = 1;
    shift32 <<= pos;
    bitboard |= shift32;
    return bitboard;
}

u64 unsetbit(u64 bitboard, int pos)
{
    u64 shift = 1;
    if (pos > 31) {
        shift <<= 31;
        shift <<= pos - 31;
    } else {
        shift <<= pos;
    }
    bitboard &= ~(shift);
    return bitboard;
}

int getpos(int x, int y)
{
    return y*8 + (7 - x);
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

std::vector<Board> Board::getMoves(int position, int code, int colorcode)
{
    int x = 7 - (position % 8);
    int y = position/8;
    u64 occupied = getPieces();
    u64 whiteocc = getPieceColor(6);
    u64 blackocc = getPieceColor(7);
    u64 colorboard = getPieceColor(colorcode);
    u64 oppcolorboard;
    if (colorcode == 6) {
        oppcolorboard = getPieceColor(7);
    } else {
        oppcolorboard = getPieceColor(6);
    }
    Board test("");
    for (int i = 0; i < 8; i++) {
        test.setBB(i, getPiece(i));
    }
    test.setCastleorenpas(nextCastleOrEnpasent());
    std::vector<Board> boards;
    if (code == 0 and colorcode == 6) { //white pawn
        if (checkbit(occupied, getpos(x, y + 1)) == false) {
            boards.push_back(test);
            if (y + 1 < 7) {
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x, y + 1));
            } else {
                boards.push_back(test);
                boards[boards.size() - 2].promotepawn(colorcode, std::make_pair(x, y), std::make_pair(x, y + 1), 4);
                boards[boards.size() - 1].promotepawn(colorcode, std::make_pair(x, y), std::make_pair(x, y + 1), 2);
            }
            if (y == 1) {
                if (checkbit(occupied, getpos(x, y + 2)) == false) {
                    boards.push_back(test);
                    boards[boards.size() - 1].setCastleorenpas(setbit(boards[boards.size() - 1].getCastleOrEnpasent(), getpos(x, y + 1)));
                    boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x, y + 2));
                }
            }
        }
        if (x > 0) {
            if (checkbit(blackocc, getpos(x - 1, y + 1))) {
                boards.push_back(test);
                if (y + 1 < 7) {
                    boards[boards.size() - 1].takepiece(std::make_pair(x - 1, y + 1));
                    boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 1));
                } else {
                    boards.push_back(test);
                    boards[boards.size() - 2].takepiece(std::make_pair(x - 1, y + 1));
                    boards[boards.size() - 1].takepiece(std::make_pair(x - 1, y + 1));
                    boards[boards.size() - 2].promotepawn(colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 1), 4);
                    boards[boards.size() - 1].promotepawn(colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 1), 2);
                }
            } else if (checkbit(getCastleOrEnpasent(), getpos(x - 1, y + 1))) {
                boards.push_back(test);
                boards[boards.size() - 1].takepiece(std::make_pair(x - 1, y));
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 1));
            }
        }
        if (x < 7) {
            if (checkbit(blackocc, getpos(x + 1, y + 1))) {
                boards.push_back(test);
                if (y + 1 < 7) {
                    boards[boards.size() - 1].takepiece(std::make_pair(x + 1, y + 1));
                    boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 1));
                } else {  //taking to promotion
                    boards.push_back(test);
                    boards[boards.size() - 2].takepiece(std::make_pair(x + 1, y + 1));
                    boards[boards.size() - 1].takepiece(std::make_pair(x + 1, y + 1));
                    boards[boards.size() - 2].promotepawn(colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 1), 4);
                    boards[boards.size() - 1].promotepawn(colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 1), 2);
                }
            } else if (checkbit(getCastleOrEnpasent(), getpos(x + 1, y + 1))) {
                boards.push_back(test);
                boards[boards.size() - 1].takepiece(std::make_pair(x + 1, y));
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 1));
            }
        }
    } else if (code == 0 and colorcode == 7) { //black pawn
        if (checkbit(occupied, getpos(x, y - 1)) == false) {
            boards.push_back(test);
            if (y - 1 > 0) {
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x, y - 1));
            } else {
                boards.push_back(test);
                boards[boards.size() - 2].promotepawn(colorcode, std::make_pair(x, y), std::make_pair(x, y - 1), 4);
                boards[boards.size() - 1].promotepawn(colorcode, std::make_pair(x, y), std::make_pair(x, y - 1), 2);
            }
            if (y == 6) {
                if (checkbit(occupied, getpos(x, y - 2)) == false) {
                    boards.push_back(test);
                    boards[boards.size() - 1].setCastleorenpas(setbit(boards[boards.size() - 1].getCastleOrEnpasent(), getpos(x, y + 1)));
                    boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x, y - 2));
                }
            }
        }
        if (x > 0) {
            if (checkbit(whiteocc, getpos(x - 1, y - 1))) {
                boards.push_back(test);
                if (y - 1 > 0) {
                    boards[boards.size() - 1].takepiece(std::make_pair(x - 1, y - 1));
                    boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 1));
                } else {
                    boards.push_back(test);
                    boards[boards.size() - 2].takepiece(std::make_pair(x - 1, y - 1));
                    boards[boards.size() - 1].takepiece(std::make_pair(x - 1, y - 1));
                    boards[boards.size() - 2].promotepawn(colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 1), 4);
                    boards[boards.size() - 1].promotepawn(colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 1), 2);
                }
            } else if (checkbit(getCastleOrEnpasent(), getpos(x - 1, y - 1))) {
                boards.push_back(test);
                boards[boards.size() - 1].takepiece(std::make_pair(x - 1, y));
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 1));
            }
        }
        if (x < 7) {
            if (checkbit(whiteocc, getpos(x + 1, y - 1))) {
                boards.push_back(test);
                if (y - 1 > 0) {
                    boards[boards.size() - 1].takepiece(std::make_pair(x + 1, y - 1));
                    boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 1));
                } else {  //taking to promotion
                    boards.push_back(test);
                    boards[boards.size() - 2].takepiece(std::make_pair(x + 1, y - 1));
                    boards[boards.size() - 1].takepiece(std::make_pair(x + 1, y - 1));
                    boards[boards.size() - 2].promotepawn(colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 1), 4);
                    boards[boards.size() - 1].promotepawn(colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 1), 2);
                }
            } else if (checkbit(getCastleOrEnpasent(), getpos(x + 1, y - 1))) {
                boards.push_back(test);
                boards[boards.size() - 1].takepiece(std::make_pair(x + 1, y));
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 1));
            }
        }
    } else if (code == 1 or code == 4) { //rook and partial queen
        Board test2("");   //if the rook moves, castling is off
        for (int i = 0; i < 8; i++) {
            test2.setBB(i, getPiece(i));
        }
        if (y == 0 or y == 7) {
            test2.setCastleorenpas(unsetbit(nextCastleOrEnpasent(), getpos(x, y)));
        } else {
            test2.setCastleorenpas(nextCastleOrEnpasent());
        }
        for (int j = x + 1; j < 8; j++) {
            if (checkbit(colorboard, getpos(j, y))) {
                break;
            }
            if (checkbit(oppcolorboard, getpos(j, y))) {
                boards.push_back(test2);
                boards[boards.size() - 1].takepiece(std::make_pair(j, y));
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(j, y));
                break;
            }
            boards.push_back(test2);
            boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(j, y));
        }
        for (int j = x - 1; j >= 0; j--) {
            if (checkbit(colorboard, getpos(j, y))) {
                break;
            }
            if (checkbit(oppcolorboard, getpos(j, y))) {
                boards.push_back(test2);
                boards[boards.size() - 1].takepiece(std::make_pair(j, y));
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(j, y));
                break;
            }
            boards.push_back(test2);
            boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(j, y));
        }
        for (int j = y + 1; j < 8; j++) {
            if (checkbit(colorboard, getpos(x, j))) {
                break;
            }
            if (checkbit(oppcolorboard, getpos(x, j))) {
                boards.push_back(test2);
                boards[boards.size() - 1].takepiece(std::make_pair(x, j));
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x, j));
                break;
            }
            boards.push_back(test2);
            boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x, j));
        }
        for (int j = y - 1; j >= 0; j--) {
            if (checkbit(colorboard, getpos(x, j))) {
                break;
            }
            if (checkbit(oppcolorboard, getpos(x, j))) {
                boards.push_back(test2);
                boards[boards.size() - 1].takepiece(std::make_pair(x, j));
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x, j));
                break;
            }
            boards.push_back(test2);
            boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x, j));
        }
    } else if (code == 2) { //knight
        if (x < 6) {
            if (y < 7) {
                if (checkbit(colorboard, getpos(x + 2, y + 1)) == false) {
                    boards.push_back(test);
                    if (checkbit(oppcolorboard, getpos(x + 2, y + 1))) {
                        boards[boards.size() - 1].takepiece(std::make_pair(x + 2, y + 1));
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 2, y + 1));
                    } else {
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 2, y + 1));
                    }
                }
            }
            if (y > 0) {
                if (checkbit(colorboard, getpos(x + 2, y - 1)) == false) {
                    boards.push_back(test);
                    if (checkbit(oppcolorboard, getpos(x + 2, y - 1))) {
                        boards[boards.size() - 1].takepiece(std::make_pair(x + 2, y - 1));
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 2, y - 1));
                    } else {
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 2, y - 1));
                    }
                }
            }
        }
        if (x > 1) {
            if (y < 7) {
                if (checkbit(colorboard, getpos(x - 2, y + 1)) == false) {
                    boards.push_back(test);
                    if (checkbit(oppcolorboard, getpos(x - 2, y + 1))) {
                        boards[boards.size() - 1].takepiece(std::make_pair(x - 2, y + 1));
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 2, y + 1));
                    } else {
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 2, y + 1));
                    }
                }
            }
            if (y > 0) {
                if (checkbit(colorboard, getpos(x - 2, y - 1)) == false) {
                    boards.push_back(test);
                    if (checkbit(oppcolorboard, getpos(x - 2, y - 1))) {
                        boards[boards.size() - 1].takepiece(std::make_pair(x - 2, y - 1));
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 2, y - 1));
                    } else {
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 2, y - 1));
                    }
                }
            }
        }
        if (y < 6) {
            if (x < 7) {
                if (checkbit(colorboard, getpos(x + 1, y + 2)) == false) {
                    boards.push_back(test);
                    if (checkbit(oppcolorboard, getpos(x + 1, y + 2))) {
                        boards[boards.size() - 1].takepiece(std::make_pair(x + 1, y + 2));
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 2));
                    } else {
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 2));
                    }
                }
            }
            if (x > 0) {
                if (checkbit(colorboard, getpos(x - 1, y + 2)) == false) {
                    boards.push_back(test);
                    if (checkbit(oppcolorboard, getpos(x - 1, y + 2))) {
                        boards[boards.size() - 1].takepiece(std::make_pair(x - 1, y + 2));
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 2));
                    } else {
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 2));
                    }
                }
            }
        }
        if (y > 1) {
            if (x < 7) {
                if (checkbit(colorboard, getpos(x + 1, y - 2)) == false) {
                    boards.push_back(test);
                    if (checkbit(oppcolorboard, getpos(x + 1, y - 2))) {
                        boards[boards.size() - 1].takepiece(std::make_pair(x + 1, y - 2));
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 2));
                    } else {
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 2));
                    }
                }
            }
            if (x > 0) {
                if (checkbit(colorboard, getpos(x - 1, y - 2)) == false) {
                    boards.push_back(test);
                    if (checkbit(oppcolorboard, getpos(x - 1, y - 2))) {
                        boards[boards.size() - 1].takepiece(std::make_pair(x - 1, y - 2));
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 2));
                    } else {
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 2));
                    }
                }
            }
        }
    } else if (code == 5) { //king
        Board test3("");   //if the king moves, castling is off
        for (int i = 0; i < 8; i++) {
            test3.setBB(i, getPiece(i));
        }
        if (colorcode == 6) {
            test3.setCastleorenpas(unsetbit(nextCastleOrEnpasent(), 0));
            test3.setCastleorenpas(unsetbit(nextCastleOrEnpasent(), 7));
        } else {
            test3.setCastleorenpas(unsetbit(nextCastleOrEnpasent(), 56));
            test3.setCastleorenpas(unsetbit(nextCastleOrEnpasent(), 63));
        }
        if (x < 7) {
            if (checkbit(colorboard, getpos(x + 1, y)) == false) {
                boards.push_back(test3);
                if (checkbit(oppcolorboard, getpos(x + 1, y))) {
                     boards[boards.size() - 1].takepiece(std::make_pair(x + 1, y));
                }
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y));
            }
            if (y < 7) {
                if (checkbit(colorboard, getpos(x + 1, y + 1)) == false) {
                    boards.push_back(test3);
                    if (checkbit(oppcolorboard, getpos(x + 1, y + 1))) {
                         boards[boards.size() - 1].takepiece(std::make_pair(x + 1, y + 1));
                    }
                    boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 1));
                }
            }
            if (y > 0) {
                if (checkbit(colorboard, getpos(x + 1, y - 1)) == false) {
                    boards.push_back(test3);
                    if (checkbit(oppcolorboard, getpos(x + 1, y - 1))) {
                         boards[boards.size() - 1].takepiece(std::make_pair(x + 1, y - 1));
                    }
                    boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 1));
                }
            }
        }
        if (y < 7) {
            if (checkbit(colorboard, getpos(x, y + 1)) == false) {
                boards.push_back(test3);
                if (checkbit(oppcolorboard, getpos(x, y + 1))) {
                     boards[boards.size() - 1].takepiece(std::make_pair(x, y + 1));
                }
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x, y + 1));
            }
        }
        if (y > 0) {
            if (checkbit(colorboard, getpos(x, y - 1)) == false) {
                boards.push_back(test3);
                if (checkbit(oppcolorboard, getpos(x, y - 1))) {
                     boards[boards.size() - 1].takepiece(std::make_pair(x, y - 1));
                }
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x, y - 1));
            }
        }
        if (x > 0) {
            if (checkbit(colorboard, getpos(x - 1, y)) == false) {
                boards.push_back(test3);
                if (checkbit(oppcolorboard, getpos(x - 1, y))) {
                     boards[boards.size() - 1].takepiece(std::make_pair(x - 1, y));
                }
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y));
            }
            if (y < 7) {
                if (checkbit(colorboard, getpos(x - 1, y + 1)) == false) {
                    boards.push_back(test3);
                    if (checkbit(oppcolorboard, getpos(x - 1, y + 1))) {
                         boards[boards.size() - 1].takepiece(std::make_pair(x - 1, y + 1));
                    }
                    boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 1));
                }
            }
            if (y > 0) {
                if (checkbit(colorboard, getpos(x - 1, y - 1)) == false) {
                    boards.push_back(test3);
                    if (checkbit(oppcolorboard, getpos(x - 1, y - 1))) {
                         boards[boards.size() - 1].takepiece(std::make_pair(x - 1, y - 1));
                    }
                    boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 1));
                }
            }
        }
        if (colorcode == 6) { //white
            u64 attacked = getAttacked(7);
            if (checkbit(getCastleOrEnpasent(), 0)) {
                if (checkbit(occupied, 2) or checkbit(occupied, 1)) {
                    if (checkbit(attacked, 3) or checkbit(attacked, 2) or checkbit(attacked, 1)) {
                        boards.push_back(test3);
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(4, 0), std::make_pair(6, 0));
                        boards[boards.size() - 1].makemove(1, colorcode, std::make_pair(7, 0), std::make_pair(5, 0));
                    }
                }
            }
            if (checkbit(getCastleOrEnpasent(), 7)) {
                if (checkbit(occupied, 4) or checkbit(occupied, 5) or checkbit(occupied, 6)) {
                    if (checkbit(attacked, 3) or checkbit(attacked, 4) or checkbit(attacked, 5)) {
                        boards.push_back(test3);
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(4, 0), std::make_pair(3, 0));
                        boards[boards.size() - 1].makemove(1, colorcode, std::make_pair(0, 0), std::make_pair(2, 0));
                    }
                }
            }
        } else { //black
            u64 attacked = getAttacked(6);
            if (checkbit(getCastleOrEnpasent(), 56)) {
                if (not checkbit(occupied, 57) and not checkbit(occupied, 58)) {
                    if (not checkbit(attacked, 59) and not checkbit(attacked, 58) and not checkbit(attacked, 57)) {
                        boards.push_back(test3);
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(4, 7), std::make_pair(6, 7));
                        boards[boards.size() - 1].makemove(1, colorcode, std::make_pair(7, 7), std::make_pair(5, 7));
                    }
                }
            }
            if (checkbit(getCastleOrEnpasent(), 63)) {
                if (not checkbit(occupied, 60) and not checkbit(occupied, 61) and not checkbit(occupied, 62)) {
                    if (not checkbit(attacked, 59) and not checkbit(attacked, 60) and not checkbit(attacked, 61)) {
                        boards.push_back(test3);
                        boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(4, 7), std::make_pair(2, 7));
                        boards[boards.size() - 1].makemove(1, colorcode, std::make_pair(0, 7), std::make_pair(3, 7));
                    }
                }
            }
        }
    }
    if (code == 3 or code == 4) { //bishop and partial queen
        for (int j = 1; j <= std::min(7 - x, 7 - y); j++) {
            if (checkbit(colorboard, getpos(x + j, y + j))) {
                break;
            }
            if (checkbit(oppcolorboard, getpos(x + j, y + j))) {
                boards.push_back(test);
                boards[boards.size() - 1].takepiece(std::make_pair(x + j, y + j));
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + j, y + j));
                break;
            }
            boards.push_back(test);
            boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + j, y + j));
        }
        for (int j = 1; j <= std::min(7 - x, y); j++) {
            if (checkbit(colorboard, getpos(x + j, y - j))) {
                break;
            }
            if (checkbit(oppcolorboard, getpos(x + j, y - j))) {
                boards.push_back(test);
                boards[boards.size() - 1].takepiece(std::make_pair(x + j, y - j));
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + j, y - j));
                break;
            }
            boards.push_back(test);
            boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x + j, y - j));
        }
        for (int j = 1; j <= std::min(x, y); j++) {
            if (checkbit(colorboard, getpos(x - j, y - j))) {
                break;
            }
            if (checkbit(oppcolorboard, getpos(x - j, y - j))) {
                boards.push_back(test);
                boards[boards.size() - 1].takepiece(std::make_pair(x - j, y - j));
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - j, y - j));
                break;
            }
            boards.push_back(test);
            boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - j, y - j));
        }
        for (int j = 1; j <= std::min(x, 7 - y); j++) {
            if (checkbit(colorboard, getpos(x - j, y + j))) {
                break;
            }
            if (checkbit(oppcolorboard, getpos(x - j, y + j))) {
                boards.push_back(test);
                boards[boards.size() - 1].takepiece(std::make_pair(x - j, y + j));
                boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - j, y + j));
                break;
            }
            boards.push_back(test);
            boards[boards.size() - 1].makemove(code, colorcode, std::make_pair(x, y), std::make_pair(x - j, y + j));
        }
    }
    return boards;
}

std::vector<Board> Board::getBoards(int colorcode)
{
    if (colorcode == 1) {
        colorcode = 6;
    } else if (colorcode == -1) {
        colorcode = 7;
    }
    std::vector<Board> boards;
    std::vector<Board> singleBoards;
    u64 colorboard = getPieceColor(colorcode);
    u64 bitest = 1;
    for (int i = 0; i < 64; i++) {
        if (colorboard & bitest) {
            for (int x = 0; x < 6; x++) {
                if (getPiece(x) & bitest) {
                    singleBoards = getMoves(i, x, colorcode);
                    break;
                }
            }
            for (int j = 0; j < singleBoards.size(); j++) {
                if (singleBoards[j].inCheck(colorcode) == false) {
                    boards.push_back(singleBoards[j]);
                }
            }
        }
        bitest <<= 1;
    }
    return boards;
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

void Board::setCastleorenpas(u64 value)
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

std::string promotecode[] = {"r", "n", "b", "q"};

std::string Board::getmove(Board nextboard)
{
    std::string ret = "";
    u64 from = getPieces() & ~(nextboard.getPieces());
    u64 to = nextboard.getPieces() & ~(getPieces());
    u64 bittest = 1;
    u64 whiteboard = getPieceColor(6);
    int colorcode = 0;
    std::string startpos = "";
    std::string endpos = "";
    std::string promote = "";
    bool pro = false;
    int x;
    int y;
    for (int i = 0; i < 64; i++) {
        x = 7 - (i % 8);
        y = i/8;
        if (checkbit(from, i)) {
            if (checkbit(whiteboard, i)) {
                colorcode = 6;
            } else {
                colorcode = 7;
            }
            startpos = startpos + char('a' + x);
            startpos = startpos + char('1' + y);
            if (y == 6) { //check if pawn promoting (white)
                if (checkbit(getPiece(0), getpos(x, y))) {
                    if (checkbit(getPieceColor(6), getpos(x, y))) {
                        pro = true;
                    }
                }
            } else if (y == 1) { //check if pawn promoting (black)
                if (checkbit(getPiece(0), getpos(x, y))) {
                    if (checkbit(getPieceColor(7), getpos(x, y))) {
                        pro = true;
                    }
                }
            }
        }
        if (checkbit(to, i)) {
            endpos = endpos + char('a' + x);
            endpos = endpos + char('1' + y);
            int pcode = nextboard.getPieceFromPos(x, y);
            if (pcode > 0 and pcode < 5) {
                promote = promotecode[pcode - 1];
            }
        }
        bittest <<= 1;
    }
    if (startpos.length() > 2) { //castling, enpasent
        if (endpos.length() > 2) { //castling
            if (startpos.find("e1") != std::string::npos) {
                if (startpos.find("h1") != std::string::npos) {
                    startpos = "e1";
                    endpos = "g1";
                } else {
                    startpos = "e1";
                    endpos = "c1";
                }
            } else {
                if (startpos.find("h8") != std::string::npos) {
                    startpos = "e8";
                    endpos = "g8";
                } else {
                    startpos = "e8";
                    endpos = "c8";
                }
            }
        } else { //enpasent
            if (startpos[0] == endpos[0]) {
                startpos = startpos.substr(2, 2);
            } else if (startpos[2] == endpos[0]) {
                startpos = startpos.substr(0, 2);
            }
        }
    }
    if (endpos == "") { //taking
        to = nextboard.getPieceColor(colorcode) & ~(getPieceColor(colorcode));
        bittest = 1;
        for (int i = 0; i < 64; i++) {
            x = 7 - (i % 8);
            y = i/8;
            if (checkbit(to, i)) {
                endpos = endpos + char('a' + x);
                endpos = endpos + char('1' + y);
                int pcode = nextboard.getPieceFromPos(x, y);
                if (pcode > 0 and pcode < 5) {
                    promote = promotecode[pcode - 1];
                }
            }
            bittest <<= 1;
        }
    }
    ret = ret + startpos;
    ret = ret + endpos;
    if (pro) {
        ret = ret + promote;
    }
    return ret;
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
    setCastleorenpas(nextCastleOrEnpasent());
    int colorcode = 6;
    if (checkbit(getPieceColor(7), getpos(from.first, from.second))) {
        colorcode = 7;
    }
    if (pcode == 0 and from.first != to.first) {
        if (checkbit(getPieces(), getpos(to.first, to.second)) == false) { //enpasent
            takepiece(std::make_pair(to.first, from.second));
            makemove(pcode, colorcode, from, to);
            return true;
        }
    }
    if (pcode == 5) {
        if (colorcode == 6) {
            setCastleorenpas(unsetbit(getCastleOrEnpasent(), 0));
            setCastleorenpas(unsetbit(getCastleOrEnpasent(), 7));
        } else {
            setCastleorenpas(unsetbit(getCastleOrEnpasent(), 56));
            setCastleorenpas(unsetbit(getCastleOrEnpasent(), 63));
        }
        if (abs(from.first - to.first) > 1) { //castling
            if (to.first == 6) { //"e1g1" or "e8g8"
                makemove(pcode, colorcode, from, to);
                makemove(1, colorcode, std::make_pair(7, from.second), std::make_pair(5, from.second));
                return true;
            } else { //"e1c1" or "e8c8"
                makemove(pcode, colorcode, from, to);
                makemove(1, colorcode, std::make_pair(0, from.second), std::make_pair(3, from.second));
                return true;
            }
        }
    }
    if (pcode == 1) { //rook moving = no castle
        if (from.second == 0) {
            setCastleorenpas(unsetbit(getCastleOrEnpasent(), getpos(from.first, from.second)));
        }
    }
    if (pcode == 0) { //enpasent
        if (from.second - to.second == 2) { //moving out 2 at first
            setCastleorenpas(setbit(getCastleOrEnpasent(), getpos(from.first, from.second + 1)));
        } else if (from.second - to.second == -2) {
            setCastleorenpas(setbit(getCastleOrEnpasent(), getpos(from.first, from.second - 1)));
        }
    }
    takepiece(to);
    if (promote == ' ') {
        makemove(pcode, colorcode, from, to);
    } else {
        promotepawn(colorcode, from, to, getPieceCode(promote));
    }
    return true;
}
