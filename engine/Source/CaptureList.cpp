#include "CaptureList.hpp"

CaptureList::CaptureList(Board& gameBoard, int colorcode)
{
    done = false;
    position = 0;
    kingTake = false;
    generateMoves(gameBoard, colorcode);
}

