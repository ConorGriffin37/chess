#include "Search.hpp"
#include "Board.hpp"
#include "MoveList.hpp"
#include "UCI.hpp"
#include "TranspositionTables.hpp"

#include <iostream>

void outbitboard(u64 n);
u64 Search::nodes = 0;


pair<string, int> Search::RootAlphaBeta(Board gameBoard, int playerColor, int remainingDepth)
{
    MoveList possibleMoves(gameBoard, ((playerColor == 1) ? WHITE_CODE : BLACK_CODE), TranspositionTables::getBest(gameBoard.getZorHash()));
    nodes++;

    mov curBestMove = possibleMoves.getMovN(0);
    int currMoveNumber = 0;
    int maxScore = NEGATIVE_INFINITY;
    int score;

    u64 castle = gameBoard.getCastleOrEnpasent();
    u64 lastHash = gameBoard.getZorHash();
    int enpasCol = gameBoard.getEnpasentCol();

    while (true) {
        pair<bool, mov> nextMove = possibleMoves.getNextMove();
        if (nextMove.first) {
            UCI::sendInfo("currmove " + possibleMoves.getMoveCode(nextMove.second) + " currmovenumber " + to_string(++currMoveNumber));
            gameBoard.makeMov(nextMove.second);
            score = -AlphaBeta(gameBoard, NEGATIVE_INFINITY, -maxScore, remainingDepth - 1, playerColor*-1);
            gameBoard.unMakeMov(nextMove.second, castle, enpasCol, lastHash);
            //score = -AlphaBeta(gameBoard, NEGATIVE_INFINITY, INFINITY, remainingDepth - 1, playerColor*-1);
            //cout << "Move is " << possibleMoves.getMoveCode(nextMove.second) << " and score is " << score << std::endl;
            if (score != ILLEGAL_MOVE) {
                if (score > maxScore){
                    maxScore = score;
                    curBestMove = nextMove.second;
                }
            }
        } else {
            break;
        }
    }

    if (currMoveNumber == 0) {
        UCI::killSearch = true;
    }

    if ((UCI::quit) or (UCI::killSearch)){
        return make_pair("", 0);
    }

    TranspositionTables::setEntry(gameBoard.getZorHash(), curBestMove, remainingDepth, maxScore);
    return make_pair(possibleMoves.getMoveCode(curBestMove), maxScore);
}

int Search::AlphaBeta(Board& gameBoard, int alpha, int beta, int remainingDepth, int playerColor)
{
    nodes++;
    if ((UCI::quit) or (UCI::killSearch)){
        return 0;
    }

    if (remainingDepth == 0){
        if (gameBoard.inCheck(((playerColor*-1 == 1) ? WHITE_CODE : BLACK_CODE))) {
            return -ILLEGAL_MOVE;
        }
        return gameBoard.getEvaluation()*playerColor;
    }

    MoveList possibleMoves = MoveList(gameBoard, ((playerColor == 1) ? WHITE_CODE : BLACK_CODE), TranspositionTables::getBest(gameBoard.getZorHash()));
    if (possibleMoves.kingTake) {
        return -ILLEGAL_MOVE;
    }

    int score;
    u64 castle = gameBoard.getCastleOrEnpasent();
    u64 lastHash = gameBoard.getZorHash();
    int enpasCol = gameBoard.getEnpasentCol();
    int maxScore = NEGATIVE_INFINITY;
    bool canMove = false;
    mov bestMove;

    while (true) {
        pair<bool, mov> nextMove = possibleMoves.getNextMove();
        if (nextMove.first) {
            gameBoard.makeMov(nextMove.second);
            score = -AlphaBeta(gameBoard, -beta, -alpha, remainingDepth - 1, playerColor*-1);
            gameBoard.unMakeMov(nextMove.second, castle, enpasCol, lastHash);
            if (score != ILLEGAL_MOVE) {
                canMove = true;
                if (score >= beta){
                    TranspositionTables::setEntry(gameBoard.getZorHash(), nextMove.second, remainingDepth, beta);
                    return beta;
                }
                if (score > alpha){
                    alpha = score;
                    bestMove = nextMove.second;
                    maxScore = score;
                } else if (score > maxScore) {
                    maxScore = score;
                    bestMove = nextMove.second;
                }
            }
        } else {
            break;
        }
    }

    if (canMove == false) {
        if (gameBoard.inCheck(((playerColor == 1) ? WHITE_CODE : BLACK_CODE))) {
            return (-MATE_SCORE - remainingDepth);
        }
        return 0;
    }

    TranspositionTables::setEntry(gameBoard.getZorHash(), bestMove, remainingDepth, alpha);
    return alpha;
}
