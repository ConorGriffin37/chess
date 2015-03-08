#include "MoveList.hpp"
#include <iostream>

bool checkbit(u64 bitboard, int pos);
u64 setbit(u64 bitboard, int pos);
u64 unsetbit(u64 bitboard, int pos);
int getpos(int x, int y);

MoveList::MoveList(Board& gameBoard, int colorcode, mov bestMove)
{
    timesCalled = 0;
    position = 0;
    kingTake = false;
    generateMoves(gameBoard, colorcode);
    if (bestMove.code == -1) {
        scoreMoves();
    } else {
        scoreMoves(bestMove);
    }
}

MoveList::MoveList(Board& gameBoard, int colorcode, bool dontScore)
{
    timesCalled = 0;
    position = 0;
    kingTake = false;
    generateMoves(gameBoard, colorcode);
}

MoveList::MoveList()
{

}

int pieceScore[] = {1, 5, 3, 3, 9, 2};

bool getEqual(mov x, mov y)
{
    if (x.take != y.take) {
        return false;
    }
    if (x.promote != y.promote) {
        return false;
    }
    if (x.enPas != y.enPas) {
        return false;
    }
    if (x.castle != y.castle) {
        return false;
    }
    if (x.code != y.code) {
        return false;
    }
    if (x.colorcode != y.colorcode) {
        return false;
    }
    if (x.procode != y.procode) {
        return false;
    }
    if (x.takecode != y.takecode) {
        return false;
    }
    if (x.from != y.from) {
        return false;
    }
    if (x.to != y.to) {
        return false;
    }
    if (x.takepos != y.takepos) {
        return false;
    }
    if (x.rookfrom != y.rookfrom) {
        return false;
    }
    if (x.rookto != y.rookto) {
        return false;
    }
    return true;
}

void MoveList::scoreMoves(mov bestMove)
{
    for (int i = 0; i < moves.size(); i++) {
        int score = 50;
        if (getEqual(moves[i], bestMove)) {
            score = score + 500;
        }
        if (moves[i].take) {
            score = score + 1 + (pieceScore[moves[i].takecode]*10 - pieceScore[moves[i].code]);
        }
        if (moves[i].promote) {
            score = score + pieceScore[moves[i].procode]*10;
        }
        moves[i].score = score;
    }
}

void MoveList::scoreMoves()
{
    for (int i = 0; i < moves.size(); i++) {
        int score = 50;
        if (moves[i].take) {
            score = score + 1 + (pieceScore[moves[i].takecode]*10 - pieceScore[moves[i].code]);
        }
        if (moves[i].promote) {
            score = score + pieceScore[moves[i].procode]*10;
        }
        moves[i].score = score;
    }
}

char promotecodes[] = {' ', 'r', 'n', 'b', 'q', ' '};

std::string MoveList::getMoveCode(mov x)
{
    std::string ret = "";
    ret = ret + char('a' + x.from.first) + char('1' + x.from.second) + char('a' + x.to.first) + char('1' + x.to.second);
    if (x.promote) {
        ret = ret + promotecodes[x.procode];
    }
    return ret;
}

void MoveList::addMove(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to)
{
    mov newMove;
    newMove.code = code;
    newMove.colorcode = colorcode;
    newMove.from = from;
    newMove.to = to;
    moves.push_back(newMove);
}

void MoveList::addMoveTake(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to, int takecode)
{
    mov newMove;
    newMove.code = code;
    newMove.colorcode = colorcode;
    newMove.from = from;
    newMove.to = to;
    newMove.take = true;
    if (takecode == 5) {
        kingTake = true;
    }
    newMove.takepos = to;
    newMove.takecode = takecode;
    moves.push_back(newMove);
}

void MoveList::addMovePro(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to, int procode)
{
    mov newMove;
    newMove.code = code;
    newMove.colorcode = colorcode;
    newMove.from = from;
    newMove.to = to;
    newMove.promote = true;
    newMove.procode = procode;
    moves.push_back(newMove);
}

void MoveList::addMoveEnpas(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to, std::pair<int, int> takepos)
{
    mov newMove;
    newMove.code = code;
    newMove.colorcode = colorcode;
    newMove.from = from;
    newMove.to = to;
    newMove.take = true;
    newMove.enPas = true;
    newMove.takepos = takepos;
    newMove.takecode = 0;
    moves.push_back(newMove);
}

void MoveList::addMoveTakePro(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to, int takecode, int procode)
{
    mov newMove;
    newMove.code = code;
    newMove.colorcode = colorcode;
    newMove.from = from;
    newMove.to = to;
    newMove.promote = true;
    newMove.procode = procode;
    newMove.take = true;
    if (takecode == 5) {
        kingTake = true;
    }
    newMove.takepos = to;
    newMove.takecode = takecode;
    moves.push_back(newMove);
}

void MoveList::addMoveCastle(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to, std::pair<int, int> rookfrom, std::pair<int, int> rookto)
{
    mov newMove;
    newMove.code = code;
    newMove.colorcode = colorcode;
    newMove.from = from;
    newMove.to = to;
    newMove.castle = true;
    newMove.rookfrom = rookfrom;
    newMove.rookto = rookto;
    moves.push_back(newMove);
}

void MoveList::getPawnMoves(Board &gameBoard, int pos, int colorcode)
{
    int x = 7 - (pos % 8);
    int y = pos/8;
    u64 occupied = gameBoard.getPieces();
    u64 whiteocc = gameBoard.getPieceColor(6);
    u64 blackocc = gameBoard.getPieceColor(7);
    if (colorcode == 6) { //white pawn
        if (checkbit(occupied, getpos(x, y + 1)) == false) {
            if (y + 1 < 7) {
                addMove(0, colorcode, std::make_pair(x, y), std::make_pair(x, y + 1));
            } else {
                addMovePro(0, colorcode, std::make_pair(x, y), std::make_pair(x, y + 1), 4);
                addMovePro(0, colorcode, std::make_pair(x, y), std::make_pair(x, y + 1), 2);
                addMovePro(0, colorcode, std::make_pair(x, y), std::make_pair(x, y + 1), 1);
                addMovePro(0, colorcode, std::make_pair(x, y), std::make_pair(x, y + 1), 3);
            }
            if (y == 1) {
                if (checkbit(occupied, getpos(x, y + 2)) == false) {
                    addMove(0, colorcode, std::make_pair(x, y), std::make_pair(x, y + 2));
                }
            }
        }
        if (x > 0) {
            if (checkbit(blackocc, getpos(x - 1, y + 1))) {
                if (y + 1 < 7) {
                    addMoveTake(0, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 1), gameBoard.getPieceFromPos(x - 1, y + 1));
                } else {
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 1), gameBoard.getPieceFromPos(x - 1, y + 1), 4);
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 1), gameBoard.getPieceFromPos(x - 1, y + 1), 2);
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 1), gameBoard.getPieceFromPos(x - 1, y + 1), 1);
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 1), gameBoard.getPieceFromPos(x - 1, y + 1), 3);
                }
            } else if (checkbit(gameBoard.getCastleOrEnpasent(), getpos(x - 1, y + 1))) {
                addMoveEnpas(0, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 1), std::make_pair(x - 1, y));
            }
        }
        if (x < 7) {
            if (checkbit(blackocc, getpos(x + 1, y + 1))) {
                if (y + 1 < 7) {
                    addMoveTake(0, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 1), gameBoard.getPieceFromPos(x + 1, y + 1));
                } else {  //taking to promotion
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 1), gameBoard.getPieceFromPos(x + 1, y + 1), 4);
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 1), gameBoard.getPieceFromPos(x + 1, y + 1), 2);
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 1), gameBoard.getPieceFromPos(x + 1, y + 1), 1);
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 1), gameBoard.getPieceFromPos(x + 1, y + 1), 3);
                }
            } else if (checkbit(gameBoard.getCastleOrEnpasent(), getpos(x + 1, y + 1))) {
                addMoveEnpas(0, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 1), std::make_pair(x + 1, y));
            }
        }
    } else if (colorcode == 7) { //black pawn
        if (checkbit(occupied, getpos(x, y - 1)) == false) {
            if (y - 1 > 0) {
                addMove(0, colorcode, std::make_pair(x, y), std::make_pair(x, y - 1));
            } else {
                addMovePro(0, colorcode, std::make_pair(x, y), std::make_pair(x, y - 1), 4);
                addMovePro(0, colorcode, std::make_pair(x, y), std::make_pair(x, y - 1), 2);
                addMovePro(0, colorcode, std::make_pair(x, y), std::make_pair(x, y - 1), 1);
                addMovePro(0, colorcode, std::make_pair(x, y), std::make_pair(x, y - 1), 3);
            }
            if (y == 6) {
                if (checkbit(occupied, getpos(x, y - 2)) == false) {
                     addMove(0, colorcode, std::make_pair(x, y), std::make_pair(x, y - 2));
                }
            }
        }
        if (x > 0) {
            if (checkbit(whiteocc, getpos(x - 1, y - 1))) {
                if (y - 1 > 0) {
                    addMoveTake(0, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 1), gameBoard.getPieceFromPos(x - 1, y - 1));
                } else {
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 1), gameBoard.getPieceFromPos(x - 1, y - 1), 4);
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 1), gameBoard.getPieceFromPos(x - 1, y - 1), 2);
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 1), gameBoard.getPieceFromPos(x - 1, y - 1), 1);
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 1), gameBoard.getPieceFromPos(x - 1, y - 1), 3);
                }
            } else if (checkbit(gameBoard.getCastleOrEnpasent(), getpos(x - 1, y - 1))) {
                addMoveEnpas(0, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 1), std::make_pair(x - 1, y));
            }
        }
        if (x < 7) {
            if (checkbit(whiteocc, getpos(x + 1, y - 1))) {
                if (y - 1 > 0) {
                    addMoveTake(0, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 1), gameBoard.getPieceFromPos(x + 1, y - 1));
                } else {  //taking to promotion
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 1), gameBoard.getPieceFromPos(x + 1, y - 1), 4);
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 1), gameBoard.getPieceFromPos(x + 1, y - 1), 2);
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 1), gameBoard.getPieceFromPos(x + 1, y - 1), 1);
                    addMoveTakePro(0, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 1), gameBoard.getPieceFromPos(x + 1, y - 1), 3);
                }
            } else if (checkbit(gameBoard.getCastleOrEnpasent(), getpos(x + 1, y - 1))) {
                addMoveEnpas(0, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 1), std::make_pair(x + 1, y));
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
    if (colorcode == 6) {
        oppcolorboard = gameBoard.getPieceColor(7);
    } else {
        oppcolorboard = gameBoard.getPieceColor(6);
    }
    for (int j = x + 1; j < 8; j++) {
        if (checkbit(colorboard, getpos(j, y))) {
            break;
        }
        if (checkbit(oppcolorboard, getpos(j, y))) {
            addMoveTake(code, colorcode, std::make_pair(x, y), std::make_pair(j, y), gameBoard.getPieceFromPos(j, y));
            break;
        }
        addMove(code, colorcode, std::make_pair(x, y), std::make_pair(j, y));
    }
    for (int j = x - 1; j >= 0; j--) {
        if (checkbit(colorboard, getpos(j, y))) {
            break;
        }
        if (checkbit(oppcolorboard, getpos(j, y))) {
            addMoveTake(code, colorcode, std::make_pair(x, y), std::make_pair(j, y), gameBoard.getPieceFromPos(j, y));
            break;
        }
        addMove(code, colorcode, std::make_pair(x, y), std::make_pair(j, y));
    }
    for (int j = y + 1; j < 8; j++) {
        if (checkbit(colorboard, getpos(x, j))) {
            break;
        }
        if (checkbit(oppcolorboard, getpos(x, j))) {
            addMoveTake(code, colorcode, std::make_pair(x, y), std::make_pair(x, j), gameBoard.getPieceFromPos(x, j));
            break;
        }
        addMove(code, colorcode, std::make_pair(x, y), std::make_pair(x, j));
    }
    for (int j = y - 1; j >= 0; j--) {
        if (checkbit(colorboard, getpos(x, j))) {
            break;
        }
        if (checkbit(oppcolorboard, getpos(x, j))) {
            addMoveTake(code, colorcode, std::make_pair(x, y), std::make_pair(x, j), gameBoard.getPieceFromPos(x, j));
            break;
        }
        addMove(code, colorcode, std::make_pair(x, y), std::make_pair(x, j));
    }
}

void MoveList::getKnightMoves(Board &gameBoard, int pos, int colorcode)
{
    int x = 7 - (pos % 8);
    int y = pos/8;
    u64 colorboard = gameBoard.getPieceColor(colorcode);
    u64 oppcolorboard;
    if (colorcode == 6) {
        oppcolorboard = gameBoard.getPieceColor(7);
    } else {
        oppcolorboard = gameBoard.getPieceColor(6);
    }
    if (x < 6) {
        if (y < 7) {
            if (checkbit(colorboard, getpos(x + 2, y + 1)) == false) {
                if (checkbit(oppcolorboard, getpos(x + 2, y + 1))) {
                    addMoveTake(2, colorcode, std::make_pair(x, y), std::make_pair(x + 2, y + 1), gameBoard.getPieceFromPos(x + 2, y + 1));
                } else {
                    addMove(2, colorcode, std::make_pair(x, y), std::make_pair(x + 2, y + 1));
                }
            }
        }
        if (y > 0) {
            if (checkbit(colorboard, getpos(x + 2, y - 1)) == false) {
                if (checkbit(oppcolorboard, getpos(x + 2, y - 1))) {
                    addMoveTake(2, colorcode, std::make_pair(x, y), std::make_pair(x + 2, y - 1), gameBoard.getPieceFromPos(x + 2, y - 1));
                } else {
                    addMove(2, colorcode, std::make_pair(x, y), std::make_pair(x + 2, y - 1));
                }
            }
        }
    }
    if (x > 1) {
        if (y < 7) {
            if (checkbit(colorboard, getpos(x - 2, y + 1)) == false) {
                if (checkbit(oppcolorboard, getpos(x - 2, y + 1))) {
                    addMoveTake(2, colorcode, std::make_pair(x, y), std::make_pair(x - 2, y + 1), gameBoard.getPieceFromPos(x - 2, y + 1));
                } else {
                    addMove(2, colorcode, std::make_pair(x, y), std::make_pair(x - 2, y + 1));
                }
            }
        }
        if (y > 0) {
            if (checkbit(colorboard, getpos(x - 2, y - 1)) == false) {
                if (checkbit(oppcolorboard, getpos(x - 2, y - 1))) {
                    addMoveTake(2, colorcode, std::make_pair(x, y), std::make_pair(x - 2, y - 1), gameBoard.getPieceFromPos(x - 2, y - 1));
                } else {
                    addMove(2, colorcode, std::make_pair(x, y), std::make_pair(x - 2, y - 1));
                }
            }
        }
    }
    if (y < 6) {
        if (x < 7) {
            if (checkbit(colorboard, getpos(x + 1, y + 2)) == false) {
                if (checkbit(oppcolorboard, getpos(x + 1, y + 2))) {
                    addMoveTake(2, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 2), gameBoard.getPieceFromPos(x + 1, y + 2));
                } else {
                    addMove(2, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 2));
                }
            }
        }
        if (x > 0) {
            if (checkbit(colorboard, getpos(x - 1, y + 2)) == false) {
                if (checkbit(oppcolorboard, getpos(x - 1, y + 2))) {
                    addMoveTake(2, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 2), gameBoard.getPieceFromPos(x - 1, y + 2));
                } else {
                    addMove(2, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 2));
                }
            }
        }
    }
    if (y > 1) {
        if (x < 7) {
            if (checkbit(colorboard, getpos(x + 1, y - 2)) == false) {
                if (checkbit(oppcolorboard, getpos(x + 1, y - 2))) {
                    addMoveTake(2, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 2), gameBoard.getPieceFromPos(x + 1, y - 2));
                } else {
                    addMove(2, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 2));
                }
            }
        }
        if (x > 0) {
            if (checkbit(colorboard, getpos(x - 1, y - 2)) == false) {
                if (checkbit(oppcolorboard, getpos(x - 1, y - 2))) {
                    addMoveTake(2, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 2), gameBoard.getPieceFromPos(x - 1, y - 2));
                } else {
                    addMove(2, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 2));
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
    if (colorcode == 6) {
        oppcolorboard = gameBoard.getPieceColor(7);
    } else {
        oppcolorboard = gameBoard.getPieceColor(6);
    }
    for (int j = 1; j <= std::min(7 - x, 7 - y); j++) {
        if (checkbit(colorboard, getpos(x + j, y + j))) {
            break;
        }
        if (checkbit(oppcolorboard, getpos(x + j, y + j))) {
            addMoveTake(code, colorcode, std::make_pair(x, y), std::make_pair(x + j, y + j), gameBoard.getPieceFromPos(x + j, y + j));
            break;
        }
        addMove(code, colorcode, std::make_pair(x, y), std::make_pair(x + j, y + j));
    }
    for (int j = 1; j <= std::min(7 - x, y); j++) {
        if (checkbit(colorboard, getpos(x + j, y - j))) {
            break;
        }
        if (checkbit(oppcolorboard, getpos(x + j, y - j))) {
            addMoveTake(code, colorcode, std::make_pair(x, y), std::make_pair(x + j, y - j), gameBoard.getPieceFromPos(x + j, y - j));
            break;
        }
        addMove(code, colorcode, std::make_pair(x, y), std::make_pair(x + j, y - j));
    }
    for (int j = 1; j <= std::min(x, y); j++) {
        if (checkbit(colorboard, getpos(x - j, y - j))) {
            break;
        }
        if (checkbit(oppcolorboard, getpos(x - j, y - j))) {
            addMoveTake(code, colorcode, std::make_pair(x, y), std::make_pair(x - j, y - j), gameBoard.getPieceFromPos(x - j, y - j));
            break;
        }
        addMove(code, colorcode, std::make_pair(x, y), std::make_pair(x - j, y - j));
    }
    for (int j = 1; j <= std::min(x, 7 - y); j++) {
        if (checkbit(colorboard, getpos(x - j, y + j))) {
            break;
        }
        if (checkbit(oppcolorboard, getpos(x - j, y + j))) {
            addMoveTake(code, colorcode, std::make_pair(x, y), std::make_pair(x - j, y + j), gameBoard.getPieceFromPos(x - j, y + j));
            break;
        }
        addMove(code, colorcode, std::make_pair(x, y), std::make_pair(x - j, y + j));
    }
}

void MoveList::getKingMoves(Board &gameBoard, int pos, int colorcode)
{
    int x = 7 - (pos % 8);
    int y = pos/8;
    u64 colorboard = gameBoard.getPieceColor(colorcode);
    u64 occupied = gameBoard.getPieces();
    u64 whiteocc = gameBoard.getPieceColor(6);
    u64 blackocc = gameBoard.getPieceColor(7);
    u64 oppcolorboard;
    if (colorcode == 6) {
        oppcolorboard = gameBoard.getPieceColor(7);
    } else {
        oppcolorboard = gameBoard.getPieceColor(6);
    }
    if (x < 7) {
        if (checkbit(colorboard, getpos(x + 1, y)) == false) {
            if (checkbit(oppcolorboard, getpos(x + 1, y))) {
                addMoveTake(5, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y), gameBoard.getPieceFromPos(x + 1, y));
            } else {
                addMove(5, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y));
            }
        }
        if (y < 7) {
            if (checkbit(colorboard, getpos(x + 1, y + 1)) == false) {
                if (checkbit(oppcolorboard, getpos(x + 1, y + 1))) {
                    addMoveTake(5, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y  + 1), gameBoard.getPieceFromPos(x + 1, y + 1));
                } else {
                    addMove(5, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y + 1));
                }
            }
        }
        if (y > 0) {
            if (checkbit(colorboard, getpos(x + 1, y - 1)) == false) {
                if (checkbit(oppcolorboard, getpos(x + 1, y - 1))) {
                    addMoveTake(5, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y  - 1), gameBoard.getPieceFromPos(x + 1, y - 1));
                } else {
                    addMove(5, colorcode, std::make_pair(x, y), std::make_pair(x + 1, y - 1));
                }
            }
        }
    }
    if (y < 7) {
        if (checkbit(colorboard, getpos(x, y + 1)) == false) {
            if (checkbit(oppcolorboard, getpos(x, y + 1))) {
                addMoveTake(5, colorcode, std::make_pair(x, y), std::make_pair(x, y + 1), gameBoard.getPieceFromPos(x, y + 1));
            } else {
                addMove(5, colorcode, std::make_pair(x, y), std::make_pair(x, y + 1));
            }
        }
    }
    if (y > 0) {
        if (checkbit(colorboard, getpos(x, y - 1)) == false) {
            if (checkbit(oppcolorboard, getpos(x, y - 1))) {
                addMoveTake(5, colorcode, std::make_pair(x, y), std::make_pair(x, y - 1), gameBoard.getPieceFromPos(x, y - 1));
            } else {
                addMove(5, colorcode, std::make_pair(x, y), std::make_pair(x, y - 1));
            }
        }
    }
    if (x > 0) {
        if (checkbit(colorboard, getpos(x - 1, y)) == false) {
            if (checkbit(oppcolorboard, getpos(x - 1, y))) {
                addMoveTake(5, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y), gameBoard.getPieceFromPos(x - 1, y));
            } else {
                addMove(5, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y));
            }
        }
        if (y < 7) {
            if (checkbit(colorboard, getpos(x - 1, y + 1)) == false) {
                if (checkbit(oppcolorboard, getpos(x - 1, y + 1))) {
                    addMoveTake(5, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 1), gameBoard.getPieceFromPos(x - 1, y + 1));
                } else {
                    addMove(5, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y + 1));
                }
            }
        }
        if (y > 0) {
            if (checkbit(colorboard, getpos(x - 1, y - 1)) == false) {
                if (checkbit(oppcolorboard, getpos(x - 1, y - 1))) {
                    addMoveTake(5, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 1), gameBoard.getPieceFromPos(x - 1, y - 1));
                } else {
                    addMove(5, colorcode, std::make_pair(x, y), std::make_pair(x - 1, y - 1));
                }
            }
        }
    }
    if (colorcode == 6) { //white
        u64 attacked = gameBoard.getAttacked(7);
        if (checkbit(gameBoard.getCastleOrEnpasent(), 0)) {
            if (checkbit(whiteocc, 0)) {
                if (not checkbit(occupied, 2) and not checkbit(occupied, 1)) {
                    if (not checkbit(attacked, 3) and not checkbit(attacked, 2) and not checkbit(attacked, 1)) {
                        addMoveCastle(5, colorcode, std::make_pair(4, 0), std::make_pair(6, 0), std::make_pair(7, 0), std::make_pair(5, 0));
                    }
                }
            }
        }
        if (checkbit(gameBoard.getCastleOrEnpasent(), 7)) {
            if (checkbit(whiteocc, 7)) {
                if (not checkbit(occupied, 4) and not checkbit(occupied, 5) and not checkbit(occupied, 6)) {
                    if (not checkbit(attacked, 3) and not checkbit(attacked, 4) and not checkbit(attacked, 5)) {
                        addMoveCastle(5, colorcode, std::make_pair(4, 0), std::make_pair(2, 0), std::make_pair(0, 0), std::make_pair(3, 0));
                    }
                }
            }
        }
    } else { //black
        u64 attacked = gameBoard.getAttacked(6);
        if (checkbit(gameBoard.getCastleOrEnpasent(), 56)) {
            if (checkbit(blackocc, 56)) {
                if (not checkbit(occupied, 57) and not checkbit(occupied, 58)) {
                    if (not checkbit(attacked, 59) and not checkbit(attacked, 58) and not checkbit(attacked, 57)) {
                        addMoveCastle(5, colorcode, std::make_pair(4, 7), std::make_pair(6, 7), std::make_pair(7, 7), std::make_pair(5, 7));
                    }
                }
            }
        }
        if (checkbit(gameBoard.getCastleOrEnpasent(), 63)) {
            if (checkbit(blackocc, 63)) {
                if (not checkbit(occupied, 60) and not checkbit(occupied, 61) and not checkbit(occupied, 62)) {
                    if (not checkbit(attacked, 59) and not checkbit(attacked, 60) and not checkbit(attacked, 61)) {
                        addMoveCastle(5, colorcode, std::make_pair(4, 7), std::make_pair(2, 7), std::make_pair(0, 7), std::make_pair(3, 7));
                    }
                }
            }
        }
    }
}

void MoveList::generateMoves(Board &gameBoard, int colorcode)
{
    if (colorcode == 1) {
        colorcode = 6;
    } else if (colorcode == -1) {
        colorcode = 7;
    }
    u64 colorboard = gameBoard.getPieceColor(colorcode);
    u64 pieceBBs[6];
    for (int i = 0; i < 6; i++) {
        pieceBBs[i] = gameBoard.getPiece(i);
    }
    u64 bitest = 1;
    for (int i = 0; i < 64; i++) {
        if (colorboard & bitest) {
            if (pieceBBs[0] & bitest) { //pawn
                getPawnMoves(gameBoard, i, colorcode);
            } else if (pieceBBs[1] & bitest) { //rook
                getRookMoves(gameBoard, i, 1, colorcode);
            } else if (pieceBBs[2] & bitest) { //knight
                getKnightMoves(gameBoard, i, colorcode);
            } else if (pieceBBs[3] & bitest) { //bishop
                getBishopMoves(gameBoard, i, 3, colorcode);
            } else if (pieceBBs[4] & bitest) { //queen
                getRookMoves(gameBoard, i, 4, colorcode);
                getBishopMoves(gameBoard, i, 4, colorcode);
            } else if (pieceBBs[5] & bitest) { //king
                getKingMoves(gameBoard, i, colorcode);
            }
        }
        bitest <<= 1;
    }
}

std::pair<bool, mov> MoveList::getNextMove()
{
    timesCalled++;
    if (timesCalled <= 5) {
        int bestscore = -1;
        int best;
        for (int i = 0; i < moves.size(); i++) {
            if (moves[i].score > bestscore) {
                bestscore = moves[i].score;
                best = i;
            }
        }
        if (bestscore == -1) {
            mov bad;
            return std::make_pair(false, bad);
        } else {
            moves[best].score = -1;
            return std::make_pair(true, moves[best]);
        }
    }
    for (int i = position; i < moves.size(); i++) {
        position++;
        if (moves[i].score > -1) {
            return std::make_pair(true, moves[i]);
        }
    }
    mov bad;
    return std::make_pair(false, bad);
}

mov MoveList::getMovN(int n)
{
    return moves[n];
}

int MoveList::getMoveNumber()
{
    return (int)moves.size();
}

