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
    for (unsigned int i = 0; i < possibleMoves.size(); i++) {
        score = -AlphaBeta(possibleMoves[i], -1000000, 1000000, remainingDepth - 1, playerColor*-1);
        if (score > maxScore){
            maxScore = score;
            curBestMove = possibleMoves[i];
        }
    }
    //outbitboard(curBestMove.getPieces());
    return gameBoard.getmove(curBestMove);
}

int Search::AlphaBeta(Board board, int alpha, int beta, int remainingDepth, int playerColor)
{
    if (remainingDepth == 0){
        return board.evalutateBoard()*playerColor;
    }
    vector<Board> possibleMoves = board.getBoards(playerColor);
    int score;
    if (possibleMoves.size() == 0){
        int colorCode = 6;
        if (playerColor == -1){
            colorCode = 7;
        }
        if (board.inCheck(colorCode) == true){
            //checkmate
            return 1000000*playerColor;
        } else {
            //stale mate
            return 0;
        }
    }
    for (unsigned int i = 0; i < possibleMoves.size(); i++){
        score = -AlphaBeta(board, -beta, -alpha, remainingDepth - 1, playerColor*-1);
        if (score >= beta){
            return beta; // fail-hard beta cut off
        }
        if (score > alpha){
            alpha = score;
        }
    }
    return alpha;
}





