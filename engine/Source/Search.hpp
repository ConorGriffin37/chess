#ifndef SEARCH_H
#define SEARCH_H

using namespace std;

#include <string>
#include "Board.hpp"

class Search
{
    public:
        string RootAlphaBeta(Board gameBoard, int playerColor, int remainingDepth);
        int AlphaBeta(Board board, int alpha, int beta, int remainingDepth, int playerColor);
};

#endif // SEARCH_H
