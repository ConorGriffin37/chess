#ifndef CAPTURELIST_HPP_INCLUDED
#define CAPTURELIST_HPP_INCLUDED

#include "Board.hpp"
#include "MoveList.hpp"

/**
 * @class CaptureList
 * @brief A class for generating captures from a given board
 *
 * Generates all possible captures from a given board and given color to move
 */

class CaptureList: public MoveList
{
    public:
        CaptureList(Board &gameBoard, int colorcode);
        void addMove(int code, int colorcode, int from, int to) override {}
        void addMovePro(int code, int colorcode, int from, int to, int procode) override {}
        void addMoveCastle(int code, int colorcode, int from, int to, int rookfrom, int rookto) override {}
};


#endif // CAPTURELIST_HPP_INCLUDED
