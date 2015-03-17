#include "CaptureList.hpp"

CaptureList::CaptureList(Board& gameBoard, int colorcode)
{
    timesCalled = 0;
    position = 0;
    kingTake = false;
    generateMoves(gameBoard, colorcode);
}

