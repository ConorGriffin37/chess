#ifndef EVALUATION_H
#define EVALUATION_H

#include "Board.hpp"

class Evaluation
{
 public:
        static int CheckForDoublePawns(int colorCode, Board evalBoard);
        static int rooksOnOpenFile(int colorCode, Board evalBoard);
        static int GetMobilityScore(Board evalBoard);
        static void initpopCountOfByte();
        static int popCount(u64 x);
        static int evaluateBoard(Board boardToEvaluate);
};

#endif // EVALUATION_H
