#include "Search.hpp"
#include "Board.hpp"
#include "MoveList.hpp"
#include "UCI.hpp"
#include "TranspositionTables.hpp"
#include <iostream>
#define illegal_move 10001108

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
    //cout << "Depth : " << remainingDepth << endl;
    gameBoard.setEvaluation(Evaluation::evaluateBoard(gameBoard));
    gameBoard.setZorHash(TranspositionTables::getBoardHash(gameBoard, getRealColor(playerColor)));
    MoveList possibleMoves(gameBoard, getRealColor(playerColor), TranspositionTables::getBest(gameBoard.getZorHash()));
    mov curBestMove = possibleMoves.getMovN(0);
    int maxScore = -10000000;
    int score;
    u64 castle = gameBoard.getCastleOrEnpasent();
    u64 lastHash = gameBoard.getZorHash();
    int enpasCol = gameBoard.getEnpasentCol();
    while (true) {
        pair<bool, mov> get = possibleMoves.getNextMove();
        if (get.first) {
            gameBoard.makeMov(get.second);
            score = -AlphaBeta(gameBoard, -10000000, -maxScore, remainingDepth - 1, playerColor*-1);
            //score = -AlphaBeta(gameBoard, -10000000, 10000000, remainingDepth - 1, playerColor*-1);
            //cout << "Move is " << possibleMoves.getMoveCode(get.second) << " and score is " << score << std::endl;
            if (score != illegal_move) {
                if (score > maxScore){
                    maxScore = score;
                    curBestMove = get.second;
                }
            }
            gameBoard.unMakeMov(get.second, castle, enpasCol, lastHash);
        } else {
            break;
        }
    }
    if ((UCI::quit) or (UCI::killSearch)){
        return "";
    }
    TranspositionTables::setEntry(gameBoard.getZorHash(), curBestMove, remainingDepth, maxScore);
    return possibleMoves.getMoveCode(curBestMove);
}

int Search::AlphaBeta(Board& gameBoard, int alpha, int beta, int remainingDepth, int playerColor)
{
    if ((UCI::quit) or (UCI::killSearch)){
        return 0;
    }
    if (remainingDepth == 0){
        if (gameBoard.inCheck(getRealColor(playerColor*-1))) {
            return -illegal_move;
        }
        return gameBoard.getEvaluation()*playerColor;
    }
    //entry best = TranspositionTables::getBest(gameBoard.getZorHash());
    //if (best.depth >= remainingDepth) {
    //    return best.score;
    //}
    MoveList possibleMoves = MoveList(gameBoard, getRealColor(playerColor), TranspositionTables::getBest(gameBoard.getZorHash()));
    if (possibleMoves.kingTake) {
        return -illegal_move;
    }
    int score;
    u64 castle = gameBoard.getCastleOrEnpasent();
    u64 lastHash = gameBoard.getZorHash();
    int enpasCol = gameBoard.getEnpasentCol();
    int maxScore = -10000000;
    bool canMove = false;
    mov bestOne;
    while (true) {
        pair<bool, mov> get = possibleMoves.getNextMove();
        if (get.first) {
            gameBoard.makeMov(get.second);
            score = -AlphaBeta(gameBoard, -beta, -alpha, remainingDepth - 1, playerColor*-1);
            if (score != illegal_move) {
                canMove = true;
                if (score >= beta){
                    gameBoard.unMakeMov(get.second, castle, enpasCol, lastHash);
                    TranspositionTables::setEntry(gameBoard.getZorHash(), get.second, remainingDepth, beta);
                    return beta;
                }
                if (score > alpha){
                    alpha = score;
                    bestOne = get.second;
                    maxScore = score;
                } else if (score > maxScore) {
                    maxScore = score;
                    bestOne = get.second;
                }
            }
            gameBoard.unMakeMov(get.second, castle, enpasCol, lastHash);
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
    TranspositionTables::setEntry(gameBoard.getZorHash(), bestOne, remainingDepth, alpha);
    return alpha;
}
