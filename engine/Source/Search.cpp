#include "Search.hpp"
#include "Board.hpp"
#include "MoveList.hpp"
#include "UCI.hpp"
#include <iostream>

void outbitboard(u64 n);

int getRealColor(int x)
{
    if (x == 1) {
        return 6;
    }
    return 7;
}



string Search::RootAlphaBeta(Board gameBoard, int playerColor, int remainingDepth)
{
    UCI::killSearch = false;
    MoveList possibleMoves(gameBoard, getRealColor(playerColor));
    gameBoard.setEvaluation(Evaluation::evaluateBoard(gameBoard));
    mov curBestMove = possibleMoves.getMovN(0);
    int maxScore = -10000000;
    int score;
    u64 castle = gameBoard.getCastleOrEnpasent();
    while (true) {
        pair<bool, mov> get = possibleMoves.getNextMove();
        if (get.first) {
            gameBoard.makeMov(get.second);
            if (gameBoard.inCheck(getRealColor(playerColor)) == false) {
                score = -AlphaBeta(gameBoard, -10000000, -maxScore, remainingDepth - 1, playerColor*-1);
                //score = -AlphaBeta(gameBoard, -10000000, 10000000, remainingDepth - 1, playerColor*-1);
                //cout << "Move is " << possibleMoves.getMoveCode(get.second) << " and score is " << score << std::endl;
                if (score > maxScore){
                    maxScore = score;
                    curBestMove = get.second;
                }
            }
            gameBoard.unMakeMov(get.second, castle);
        } else {
            break;
        }
    }
    if ((UCI::quit) or (UCI::killSearch)){
        return "";
    }
    return possibleMoves.getMoveCode(curBestMove);
}

int Search::AlphaBeta(Board& gameBoard, int alpha, int beta, int remainingDepth, int playerColor)
{
    if ((UCI::quit) or (UCI::killSearch)){
        return 0;
    }
    if (remainingDepth == 0){
        //return gameBoard.getEvaluation()*playerColor;
        return Evaluation::evaluateBoard(gameBoard)*playerColor;

    }
    int score;
    MoveList possibleMoves = MoveList(gameBoard, getRealColor(playerColor));
    u64 castle = gameBoard.getCastleOrEnpasent();
    bool canMove = false;
    while (true) {
        pair<bool, mov> get = possibleMoves.getNextMove();
        if (get.first) {
            gameBoard.makeMov(get.second);
            if (gameBoard.inCheck(getRealColor(playerColor)) == false) {
                canMove = true;
                score = -AlphaBeta(gameBoard, -beta, -alpha, remainingDepth - 1, playerColor*-1);
                if (score >= beta){
                    gameBoard.unMakeMov(get.second, castle);
                    return beta;
                }
                if (score > alpha){
                    alpha = score;
                }
            }
            gameBoard.unMakeMov(get.second, castle);
        } else {
            break;
        }
    }
    if (canMove == false) {
        if (gameBoard.inCheck(getRealColor(playerColor))) {
            return (-1000000 - remainingDepth);
        }
        return 0;
    }
    return alpha;
}
