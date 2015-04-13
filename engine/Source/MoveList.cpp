#include "MoveList.hpp"
#include "Search.hpp"

#include <iostream>
#include <algorithm>

void outbitboard(u64 n);
bool checkbit(u64 bitboard, int pos);
u64 setbit(u64 bitboard, int pos);
u64 unsetbit(u64 bitboard, int pos);
int getpos(int x, int y);

MoveList::MoveList(Board& gameBoard, int colorcode, u64 bestMove, u64 killerMove)
{
    done = false;
    position = 0;
    kingTake = false;
    generateMoves(gameBoard, colorcode);
    scoreMoves(bestMove, killerMove);
}

MoveList::MoveList(Board& gameBoard, int colorcode, bool dontScore)
{
    done = false;
    position = 0;
    kingTake = false;
    generateMoves(gameBoard, colorcode);
}

MoveList::MoveList(Board& gameBoard, int colorcode, std::vector<std::string> restrictedMoves)
{
    done = false;
    position = 0;
    kingTake = false;
    generateMoves(gameBoard, colorcode);
    std::sort(restrictedMoves.begin(), restrictedMoves.begin() + restrictedMoves.size());
    std::vector<u64> newMoves(moves);
    moves.clear();
    for (unsigned int i = 0; i < newMoves.size(); i++) {
        if (std::binary_search(restrictedMoves.begin(), restrictedMoves.end(), getMoveCode(newMoves[i]))) {
            moves.push_back(newMoves[i]);
        }
    }
}

MoveList::MoveList()
{

}

int pieceScore[] = {1, 5, 3, 3, 9, 2};

const u64 dmask_3 = 0b111;
const u64 dmask_6 = 0b111111;

void MoveList::scoreMoves(u64 bestMove, u64 killerMove)
{
    for (unsigned int i = 0; i < moves.size(); i++) {
        if (moves[i] == bestMove) {
            scores[i] = scores[i] + 5000;
        }
        if (moves[i] == killerMove) {
            scores[i] = scores[i] + 20;
        }
    }
}

char promotecodes[] = {' ', 'r', 'n', 'b', 'q', ' '};

std::string MoveList::getMoveCode(u64 x)
{
    int from = (x >> 6) & dmask_6;
    int to = (x >> 12) & dmask_6;
    std::string ret = "";
    ret = ret + char('a' + (7 - (from % 8))) + char('1' + from/8) + char('a' + (7 - (to % 8))) + char('1' + to/8);
    if (checkbit(x, 28)) {
        ret = ret + promotecodes[(x >> 29) & dmask_3];
    }
    return ret;
}

void MoveList::addMove(int code, int colorcode, int from, int to)
{
    u64 newMove = to;
    newMove <<= 6;
    newMove |= from;
    newMove <<= 3;
    newMove |= colorcode;
    newMove <<= 3;
    newMove |= code;
    moves.push_back(newMove);
    scores.push_back(10);
}

void MoveList::addMoveTake(int code, int colorcode, int from, int to, int takecode)
{
    u64 newMove = takecode;
    newMove <<= 6;
    newMove |= to;
    newMove <<= 1;
    newMove |= 1;
    newMove <<= 6;
    newMove |= to;
    newMove <<= 6;
    newMove |= from;
    newMove <<= 3;
    newMove |= colorcode;
    newMove <<= 3;
    newMove |= code;
    if (takecode == KING_CODE) {
        kingTake = true;
    }
    scores.push_back(pieceScore[takecode]*20 - pieceScore[code]);
    moves.push_back(newMove);
}

void MoveList::addMovePro(int code, int colorcode, int from, int to, int procode)
{
    u64 newMove = procode;
    newMove <<= 1;
    newMove |= 1;
    newMove <<= 16;
    newMove |= to;
    newMove <<= 6;
    newMove |= from;
    newMove <<= 3;
    newMove |= colorcode;
    newMove <<= 3;
    newMove |= code;
    scores.push_back(pieceScore[procode]*20);
    moves.push_back(newMove);
}

void MoveList::addMoveEnpas(int code, int colorcode, int from, int to, int takepos)
{
    u64 newMove = 1;
    newMove <<= 27;
    newMove |= takepos;
    newMove <<= 1;
    newMove |= 1;
    newMove <<= 6;
    newMove |= to;
    newMove <<= 6;
    newMove |= from;
    newMove <<= 3;
    newMove |= colorcode;
    newMove <<= 3;
    newMove |= code;
    scores.push_back(pieceScore[PAWN_CODE]*20 - pieceScore[PAWN_CODE]);
    moves.push_back(newMove);
}

void MoveList::addMoveTakePro(int code, int colorcode, int from, int to, int takecode, int procode)
{
    u64 newMove = procode;
    newMove <<= 1;
    newMove |= 1;
    newMove <<= 3;
    newMove |= takecode;
    newMove <<= 6;
    newMove |= to;
    newMove <<= 1;
    newMove |= 1;
    newMove <<= 6;
    newMove |= to;
    newMove <<= 6;
    newMove |= from;
    newMove <<= 3;
    newMove |= colorcode;
    newMove <<= 3;
    newMove |= code;
    if (takecode == KING_CODE) {
        kingTake = true;
    }
    scores.push_back(pieceScore[takecode]*20 + pieceScore[procode]*20 - pieceScore[code]);
    moves.push_back(newMove);
}

void MoveList::addMoveCastle(int code, int colorcode, int from, int to, int rookfrom, int rookto)
{
    u64 newMove = rookto;
    newMove <<= 6;
    newMove |= rookfrom;
    newMove <<= 1;
    newMove |= 1;
    newMove <<= 20;
    newMove |= to;
    newMove <<= 6;
    newMove |= from;
    newMove <<= 3;
    newMove |= colorcode;
    newMove <<= 3;
    newMove |= code;
    scores.push_back(10);
    moves.push_back(newMove);
}

void MoveList::getPawnMoves(Board &gameBoard, int pos, int colorcode)
{
    int x = 7 - (pos % 8);
    int y = pos/8;
    u64 occupied = gameBoard.getPieces();
    u64 whiteocc = gameBoard.getPieceColor(WHITE_CODE);
    u64 blackocc = gameBoard.getPieceColor(BLACK_CODE);
    if (colorcode == WHITE_CODE) { //white pawn
        if (checkbit(occupied, pos + 8) == false) {
            if (y + 1 < 7) {
                addMove(PAWN_CODE, colorcode, pos, pos + 8);
            } else {
                addMovePro(PAWN_CODE, colorcode, pos, pos + 8, 4);
                addMovePro(PAWN_CODE, colorcode, pos, pos + 8, 2);
                addMovePro(PAWN_CODE, colorcode, pos, pos + 8, 1);
                addMovePro(PAWN_CODE, colorcode, pos, pos + 8, 3);
            }
            if (y == 1) {
                if (checkbit(occupied, pos + 16) == false) {
                    addMove(PAWN_CODE, colorcode, pos, pos + 16);
                }
            }
        }
        if (x > 0) {
            if (checkbit(blackocc, pos + 9)) {
                if (y + 1 < 7) {
                    addMoveTake(PAWN_CODE, colorcode, pos, pos + 9, gameBoard.getPieceFromPos(pos + 9));
                } else {
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos + 9, gameBoard.getPieceFromPos(pos + 9), 4);
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos + 9, gameBoard.getPieceFromPos(pos + 9), 2);
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos + 9, gameBoard.getPieceFromPos(pos + 9), 1);
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos + 9, gameBoard.getPieceFromPos(pos + 9), 3);
                }
            } else if (checkbit(gameBoard.getCastleOrEnpasent(), pos + 9)) {
                addMoveEnpas(PAWN_CODE, colorcode, pos, pos + 9, pos + 1);
            }
        }
        if (x < 7) {
            if (checkbit(blackocc, pos + 7)) {
                if (y + 1 < 7) {
                    addMoveTake(PAWN_CODE, colorcode, pos, pos + 7, gameBoard.getPieceFromPos(pos + 7));
                } else {  //taking to promotion
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos + 7, gameBoard.getPieceFromPos(pos + 7), 4);
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos + 7, gameBoard.getPieceFromPos(pos + 7), 2);
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos + 7, gameBoard.getPieceFromPos(pos + 7), 1);
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos + 7, gameBoard.getPieceFromPos(pos + 7), 3);
                }
            } else if (checkbit(gameBoard.getCastleOrEnpasent(), pos + 7)) {
                addMoveEnpas(PAWN_CODE, colorcode, pos, pos + 7, pos - 1);
            }
        }
    } else if (colorcode == BLACK_CODE) { //black pawn
        if (checkbit(occupied, pos - 8) == false) {
            if (y - 1 > 0) {
                addMove(PAWN_CODE, colorcode, pos, pos - 8);
            } else {
                addMovePro(PAWN_CODE, colorcode, pos, pos - 8, 4);
                addMovePro(PAWN_CODE, colorcode, pos, pos - 8, 2);
                addMovePro(PAWN_CODE, colorcode, pos, pos - 8, 1);
                addMovePro(PAWN_CODE, colorcode, pos, pos - 8, 3);
            }
            if (y == 6) {
                if (checkbit(occupied, pos - 16) == false) {
                     addMove(PAWN_CODE, colorcode, pos, pos - 16);
                }
            }
        }
        if (x > 0) {
            if (checkbit(whiteocc, pos - 7)) {
                if (y - 1 > 0) {
                    addMoveTake(PAWN_CODE, colorcode, pos, pos - 7, gameBoard.getPieceFromPos(pos - 7));
                } else {
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos - 7, gameBoard.getPieceFromPos(pos - 7), 4);
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos - 7, gameBoard.getPieceFromPos(pos - 7), 2);
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos - 7, gameBoard.getPieceFromPos(pos - 7), 1);
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos - 7, gameBoard.getPieceFromPos(pos - 7), 3);
                }
            } else if (checkbit(gameBoard.getCastleOrEnpasent(), pos - 7)) {
                addMoveEnpas(PAWN_CODE, colorcode, pos, pos - 7, pos + 1);
            }
        }
        if (x < 7) {
            if (checkbit(whiteocc, pos - 9)) {
                if (y - 1 > 0) {
                    addMoveTake(PAWN_CODE, colorcode, pos, pos - 9, gameBoard.getPieceFromPos(pos - 9));
                } else {  //taking to promotion
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos - 9, gameBoard.getPieceFromPos(pos - 9), 4);
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos - 9, gameBoard.getPieceFromPos(pos - 9), 2);
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos - 9, gameBoard.getPieceFromPos(pos - 9), 1);
                    addMoveTakePro(PAWN_CODE, colorcode, pos, pos - 9, gameBoard.getPieceFromPos(pos - 9), 3);
                }
            } else if (checkbit(gameBoard.getCastleOrEnpasent(), pos - 9)) {
                addMoveEnpas(PAWN_CODE, colorcode, pos, pos - 9, pos - 1);
            }
        }
    }
}

void MoveList::getRookMoves(Board &gameBoard, int pos, int code, int colorcode)
{
    int x = 7 - (pos % 8);
    int y = pos/8;
    u64 colorboard = gameBoard.getPieceColor(colorcode);
    u64 oppcolorboard;
    if (colorcode == WHITE_CODE) {
        oppcolorboard = gameBoard.getPieceColor(BLACK_CODE);
    } else {
        oppcolorboard = gameBoard.getPieceColor(WHITE_CODE);
    }
    for (int j = 1; x + j < 8; j++) {
        if (checkbit(colorboard, pos - j)) {
            break;
        }
        if (checkbit(oppcolorboard, pos - j)) {
            addMoveTake(code, colorcode, pos, pos - j, gameBoard.getPieceFromPos(pos - j));
            break;
        }
        addMove(code, colorcode, pos, pos - j);
    }
    for (int j = 1; x - j >= 0; j++) {
        if (checkbit(colorboard, pos + j)) {
            break;
        }
        if (checkbit(oppcolorboard, pos + j)) {
            addMoveTake(code, colorcode, pos, pos + j, gameBoard.getPieceFromPos(pos + j));
            break;
        }
        addMove(code, colorcode, pos, pos + j);
    }
    for (int j = 1; y + j < 8; j++) {
        if (checkbit(colorboard, pos + j*8)) {
            break;
        }
        if (checkbit(oppcolorboard, pos + j*8)) {
            addMoveTake(code, colorcode, pos, pos + j*8, gameBoard.getPieceFromPos(pos + j*8));
            break;
        }
        addMove(code, colorcode, pos, pos + j*8);
    }
    for (int j = 1; y - j >= 0; j++) {
        if (checkbit(colorboard, pos - j*8)) {
            break;
        }
        if (checkbit(oppcolorboard, pos - j*8)) {
            addMoveTake(code, colorcode, pos, pos - j*8, gameBoard.getPieceFromPos(pos - j*8));
            break;
        }
        addMove(code, colorcode, pos, pos - j*8);
    }
}

void MoveList::getKnightMoves(Board &gameBoard, int pos, int colorcode)
{
    int x = 7 - (pos % 8);
    int y = pos/8;
    u64 colorboard = gameBoard.getPieceColor(colorcode);
    u64 oppcolorboard;
    if (colorcode == WHITE_CODE) {
        oppcolorboard = gameBoard.getPieceColor(BLACK_CODE);
    } else {
        oppcolorboard = gameBoard.getPieceColor(WHITE_CODE);
    }
    if (x < 6) {
        if (y < 7) {
            if (checkbit(colorboard, pos + 6) == false) {
                if (checkbit(oppcolorboard, pos + 6)) {
                    addMoveTake(KNIGHT_CODE, colorcode, pos, pos + 6, gameBoard.getPieceFromPos(pos + 6));
                } else {
                    addMove(KNIGHT_CODE, colorcode, pos, pos + 6);
                }
            }
        }
        if (y > 0) {
            if (checkbit(colorboard, pos - 10) == false) {
                if (checkbit(oppcolorboard, pos - 10)) {
                    addMoveTake(KNIGHT_CODE, colorcode, pos, pos - 10, gameBoard.getPieceFromPos(pos - 10));
                } else {
                    addMove(KNIGHT_CODE, colorcode, pos, pos - 10);
                }
            }
        }
    }
    if (x > 1) {
        if (y < 7) {
            if (checkbit(colorboard, pos + 10) == false) {
                if (checkbit(oppcolorboard, pos + 10)) {
                    addMoveTake(KNIGHT_CODE, colorcode, pos, pos + 10, gameBoard.getPieceFromPos(pos + 10));
                } else {
                    addMove(KNIGHT_CODE, colorcode, pos, pos + 10);
                }
            }
        }
        if (y > 0) {
            if (checkbit(colorboard, pos - 6) == false) {
                if (checkbit(oppcolorboard, pos - 6)) {
                    addMoveTake(KNIGHT_CODE, colorcode, pos, pos - 6, gameBoard.getPieceFromPos(pos - 6));
                } else {
                    addMove(KNIGHT_CODE, colorcode, pos, pos - 6);
                }
            }
        }
    }
    if (y < 6) {
        if (x < 7) {
            if (checkbit(colorboard, pos + 15) == false) {
                if (checkbit(oppcolorboard, pos + 15)) {
                    addMoveTake(KNIGHT_CODE, colorcode, pos, pos + 15, gameBoard.getPieceFromPos(pos + 15));
                } else {
                    addMove(KNIGHT_CODE, colorcode, pos, pos + 15);
                }
            }
        }
        if (x > 0) {
            if (checkbit(colorboard, pos + 17) == false) {
                if (checkbit(oppcolorboard, pos + 17)) {
                    addMoveTake(KNIGHT_CODE, colorcode, pos, pos + 17, gameBoard.getPieceFromPos(pos + 17));
                } else {
                    addMove(KNIGHT_CODE, colorcode, pos, pos + 17);
                }
            }
        }
    }
    if (y > 1) {
        if (x < 7) {
            if (checkbit(colorboard, pos - 17) == false) {
                if (checkbit(oppcolorboard, pos - 17)) {
                    addMoveTake(KNIGHT_CODE, colorcode, pos, pos - 17, gameBoard.getPieceFromPos(pos - 17));
                } else {
                    addMove(KNIGHT_CODE, colorcode, pos, pos - 17);
                }
            }
        }
        if (x > 0) {
            if (checkbit(colorboard, pos - 15) == false) {
                if (checkbit(oppcolorboard, pos - 15)) {
                    addMoveTake(KNIGHT_CODE, colorcode, pos, pos - 15, gameBoard.getPieceFromPos(pos - 15));
                } else {
                    addMove(KNIGHT_CODE, colorcode, pos, pos - 15);
                }
            }
        }
    }
}

void MoveList::getBishopMoves(Board &gameBoard, int pos, int code, int colorcode)
{
    int x = 7 - (pos % 8);
    int y = pos/8;
    u64 colorboard = gameBoard.getPieceColor(colorcode);
    u64 oppcolorboard;
    if (colorcode == WHITE_CODE) {
        oppcolorboard = gameBoard.getPieceColor(BLACK_CODE);
    } else {
        oppcolorboard = gameBoard.getPieceColor(WHITE_CODE);
    }
    for (int j = 1; j <= std::min(7 - x, 7 - y); j++) {
        if (checkbit(colorboard, pos + 7*j)) {
            break;
        }
        if (checkbit(oppcolorboard, pos + 7*j)) {
            addMoveTake(code, colorcode, pos, pos + 7*j, gameBoard.getPieceFromPos(pos + 7*j));
            break;
        }
        addMove(code, colorcode, pos, pos + 7*j);
    }
    for (int j = 1; j <= std::min(7 - x, y); j++) {
        if (checkbit(colorboard, pos - 9*j)) {
            break;
        }
        if (checkbit(oppcolorboard, pos - 9*j)) {
            addMoveTake(code, colorcode, pos, pos - 9*j, gameBoard.getPieceFromPos(pos - 9*j));
            break;
        }
        addMove(code, colorcode, pos, pos - 9*j);
    }
    for (int j = 1; j <= std::min(x, y); j++) {
        if (checkbit(colorboard, pos - 7*j)) {
            break;
        }
        if (checkbit(oppcolorboard, pos - 7*j)) {
            addMoveTake(code, colorcode, pos, pos - 7*j, gameBoard.getPieceFromPos(pos - 7*j));
            break;
        }
        addMove(code, colorcode, pos, pos - 7*j);
    }
    for (int j = 1; j <= std::min(x, 7 - y); j++) {
        if (checkbit(colorboard, pos + 9*j)) {
            break;
        }
        if (checkbit(oppcolorboard, pos + 9*j)) {
            addMoveTake(code, colorcode, pos, pos + 9*j, gameBoard.getPieceFromPos(pos + 9*j));
            break;
        }
        addMove(code, colorcode, pos, pos + 9*j);
    }
}

void MoveList::getKingMoves(Board &gameBoard, int pos, int colorcode)
{
    int x = 7 - (pos % 8);
    int y = pos/8;
    u64 colorboard = gameBoard.getPieceColor(colorcode);
    u64 occupied = gameBoard.getPieces();
    u64 whiteocc = gameBoard.getPieceColor(WHITE_CODE);
    u64 blackocc = gameBoard.getPieceColor(BLACK_CODE);
    u64 oppcolorboard;
    if (colorcode == WHITE_CODE) {
        oppcolorboard = gameBoard.getPieceColor(BLACK_CODE);
    } else {
        oppcolorboard = gameBoard.getPieceColor(WHITE_CODE);
    }
    if (x < 7) {
        if (checkbit(colorboard, pos - 1) == false) {
            if (checkbit(oppcolorboard, pos - 1)) {
                addMoveTake(KING_CODE, colorcode, pos, pos - 1, gameBoard.getPieceFromPos(pos - 1));
            } else {
                addMove(KING_CODE, colorcode, pos, pos - 1);
            }
        }
        if (y < 7) {
            if (checkbit(colorboard, pos + 7) == false) {
                if (checkbit(oppcolorboard, pos + 7)) {
                    addMoveTake(KING_CODE, colorcode, pos,pos + 7, gameBoard.getPieceFromPos(pos + 7));
                } else {
                    addMove(KING_CODE, colorcode, pos, pos + 7);
                }
            }
        }
        if (y > 0) {
            if (checkbit(colorboard, pos - 9) == false) {
                if (checkbit(oppcolorboard, pos - 9)) {
                    addMoveTake(KING_CODE, colorcode, pos, pos - 9, gameBoard.getPieceFromPos(pos - 9));
                } else {
                    addMove(KING_CODE, colorcode, pos, pos - 9);
                }
            }
        }
    }
    if (y < 7) {
        if (checkbit(colorboard, pos + 8) == false) {
            if (checkbit(oppcolorboard, pos + 8)) {
                addMoveTake(KING_CODE, colorcode, pos, pos + 8, gameBoard.getPieceFromPos(pos + 8));
            } else {
                addMove(KING_CODE, colorcode, pos, pos + 8);
            }
        }
    }
    if (y > 0) {
        if (checkbit(colorboard, pos - 8) == false) {
            if (checkbit(oppcolorboard, pos - 8)) {
                addMoveTake(KING_CODE, colorcode, pos, pos - 8, gameBoard.getPieceFromPos(pos - 8));
            } else {
                addMove(KING_CODE, colorcode, pos, pos - 8);
            }
        }
    }
    if (x > 0) {
        if (checkbit(colorboard, pos + 1) == false) {
            if (checkbit(oppcolorboard, pos + 1)) {
                addMoveTake(KING_CODE, colorcode, pos, pos + 1, gameBoard.getPieceFromPos(pos + 1));
            } else {
                addMove(KING_CODE, colorcode, pos, pos + 1);
            }
        }
        if (y < 7) {
            if (checkbit(colorboard, pos + 9) == false) {
                if (checkbit(oppcolorboard, pos + 9)) {
                    addMoveTake(KING_CODE, colorcode, pos, pos + 9, gameBoard.getPieceFromPos(pos + 9));
                } else {
                    addMove(KING_CODE, colorcode, pos, pos + 9);
                }
            }
        }
        if (y > 0) {
            if (checkbit(colorboard, pos - 7) == false) {
                if (checkbit(oppcolorboard, pos - 7)) {
                    addMoveTake(KING_CODE, colorcode, pos, pos - 7, gameBoard.getPieceFromPos(pos - 7));
                } else {
                    addMove(KING_CODE, colorcode, pos, pos - 7);
                }
            }
        }
    }
    if (colorcode == WHITE_CODE) { //white
        if (checkbit(gameBoard.getCastleOrEnpasent(), 0)) {
            if (checkbit(whiteocc, 0)) {
                if (not checkbit(occupied, 2) and not checkbit(occupied, 1)) {
                    if (not gameBoard.getAttacked(colorcode, 3) and not gameBoard.getAttacked(colorcode, 2) and not gameBoard.getAttacked(colorcode, 1)) {
                        addMoveCastle(KING_CODE, colorcode, 3, 1, 0, 2);
                    }
                }
            }
        }
        if (checkbit(gameBoard.getCastleOrEnpasent(), 7)) {
            if (checkbit(whiteocc, 7)) {
                if (not checkbit(occupied, 4) and not checkbit(occupied, 5) and not checkbit(occupied, 6)) {
                    if (not gameBoard.getAttacked(colorcode, 3) and not gameBoard.getAttacked(colorcode, 4) and not gameBoard.getAttacked(colorcode, 5)) {
                        addMoveCastle(KING_CODE, colorcode, 3, 5, 7, 4);
                    }
                }
            }
        }
    } else { //black
        if (checkbit(gameBoard.getCastleOrEnpasent(), 56)) {
            if (checkbit(blackocc, 56)) {
                if (not checkbit(occupied, 57) and not checkbit(occupied, 58)) {
                    if (not gameBoard.getAttacked(colorcode, 59) and not gameBoard.getAttacked(colorcode, 58) and not gameBoard.getAttacked(colorcode, 57)) {
                        addMoveCastle(KING_CODE, colorcode, 59, 57, 56, 58);
                    }
                }
            }
        }
        if (checkbit(gameBoard.getCastleOrEnpasent(), 63)) {
            if (checkbit(blackocc, 63)) {
                if (not checkbit(occupied, 60) and not checkbit(occupied, 61) and not checkbit(occupied, 62)) {
                    if (not gameBoard.getAttacked(colorcode, 59) and not gameBoard.getAttacked(colorcode, 60) and not gameBoard.getAttacked(colorcode, 61)) {
                        addMoveCastle(KING_CODE, colorcode, 59, 61, 63, 60);
                    }
                }
            }
        }
    }
}

void MoveList::generateMoves(Board &gameBoard, int colorcode)
{
    if (colorcode == 1) {
        colorcode = WHITE_CODE;
    } else if (colorcode == -1) {
        colorcode = BLACK_CODE;
    }
    u64 colorboard = gameBoard.getPieceColor(colorcode);
    u64 pieceBBs[6];
    for (int i = 0; i < 6; i++) {
        pieceBBs[i] = gameBoard.getPiece(i);
    }
    u64 bitest = 1;
    for (int i = 0; i < 64; i++) {
        if (colorboard & bitest) {
            if (pieceBBs[PAWN_CODE] & bitest) { //pawn
                getPawnMoves(gameBoard, i, colorcode);
            } else if (pieceBBs[ROOK_CODE] & bitest) { //rook
                getRookMoves(gameBoard, i, ROOK_CODE, colorcode);
            } else if (pieceBBs[KNIGHT_CODE] & bitest) { //knight
                getKnightMoves(gameBoard, i, colorcode);
            } else if (pieceBBs[BISHOP_CODE] & bitest) { //bishop
                getBishopMoves(gameBoard, i, BISHOP_CODE, colorcode);
            } else if (pieceBBs[QUEEN_CODE] & bitest) { //queen
                getRookMoves(gameBoard, i, QUEEN_CODE, colorcode);
                getBishopMoves(gameBoard, i, QUEEN_CODE, colorcode);
            } else if (pieceBBs[KING_CODE] & bitest) { //king
                getKingMoves(gameBoard, i, colorcode);
            }
        }
        bitest <<= 1;
    }
}

u64 MoveList::getNextMove()
{
    if (done == false) {
        int bestscore = -1;
        int best = 0;
        for (unsigned int i = 0; i < moves.size(); i++) {
            if (scores[i] > bestscore) {
                bestscore = scores[i];
                best = i;
            }
        }
        if (bestscore == -1) {
            return 0;
        } else {
            if (bestscore == 10) {
                done = true;
            }
            scores[best] = -1;
            return moves[best];
        }
    }
    for (unsigned int i = position; i < moves.size(); i++) {
        position++;
        if (scores[i] > -1) {
            return moves[i];
        }
    }
    return 0;
}

u64 MoveList::getMovN(int n)
{
    return moves[n];
}

int MoveList::getMoveNumber()
{
    return (int)moves.size();
}
