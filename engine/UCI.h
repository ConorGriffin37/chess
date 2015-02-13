#ifndef UCI_H
#define UCI_H

#include <string>
#include "Board.h"

using namespace std;

class UCI
{
    private:
        Board currentBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        int currentColor = 1;
    public:
        bool quit = 0;
        bool waitForInput();
        void outputBestMove(string moveString); //string like e2e4
        void identification();
        bool sentPosition(string input);
        bool startCalculating(string input);
        void sendInfo(string info);
};

#endif // UCI_H
