#include "Search.h"
#include "board.hpp"
#include <iostream>


void outbitboard(u64 n);

string Search::RootAlphaBeta(Board gameBoard, int playerColor, int remainingDepth)
{
    vector<Board> possibleMoves = gameBoard.getBoards(playerColor);
    Board curBestMove = possibleMoves[0];
    int maxScore = -1000000;
    int score;
    for (unsigned int i = 0; i < possibleMoves.size(); i++){
        score = -AlphaBeta(possibleMoves[i], -1000000, -maxScore, remainingDepth - 1, playerColor*-1);
        cout << "Move is " << gameBoard.getmove(possibleMoves[i]) << " and score is " << score << std::endl;
        if (score > maxScore){
            maxScore = score;
            curBestMove = possibleMoves[i];
        }
    }
    return gameBoard.getmove(curBestMove);
}

int Search::AlphaBeta(Board gameBoard, int alpha, int beta, int remainingDepth, int playerColor)
{
    if (remainingDepth == 0){
        return gameBoard.evaluateBoard()*playerColor;
    }
    int score;
    vector<Board> possibleMoves = gameBoard.getBoards(playerColor);
    if (possibleMoves.size() == 0){
        int colorCode = 6;
        if (playerColor == -1){
            colorCode = 7;
        }
        if (gameBoard.inCheck(colorCode) == true){
            //checkmate
            return (-1000000 - remainingDepth);
        } else {
            //stale mate
            return 0;
        }
    }
    for (unsigned int i = 0; i < possibleMoves.size(); i++){
        score = -AlphaBeta(possibleMoves[i], -beta, -alpha, remainingDepth - 1, playerColor*-1);
        if (score >= beta){
            return beta;
        }
        if (score > alpha){
            alpha = score;
        }
    }
    return alpha;
}






