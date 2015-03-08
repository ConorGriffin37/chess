#ifndef MOVELIST_HPP_INCLUDED
#define MOVELIST_HPP_INCLUDED

#include <string>
#include <vector>
#include <utility>
#include "Board.hpp"


class MoveList
{
    private:
        std::vector<mov> moves;
        int timesCalled;
        int position;

    public:
        bool kingTake;
        MoveList(Board &gameBoard, int colorcode, mov bestMove);
        void scoreMoves(mov bestMove);
        void scoreMoves();
        void addMove(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to);
        void addMoveTake(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to, int takecode);
        void addMovePro(int code, int colorcode, std::pair<int, int> form, std::pair<int, int> to, int procode);
        void addMoveEnpas(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to, std::pair<int, int> takepos);
        void addMoveTakePro(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to, int takecode, int procode);
        void addMoveCastle(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to, std::pair<int, int> rookfrom, std::pair<int, int> rookto);
        void generateMoves(Board &gameBoard, int colorcode);
        void getPawnMoves(Board &gameBoard, int pos, int colorcode);
        void getRookMoves(Board &gameBoard, int pos, int code, int colorcode); //rook moves need code as queen can make rook moves
        void getKnightMoves(Board &gameBoard, int pos, int colorcode);
        void getBishopMoves(Board &gameBoard, int pos, int code, int colorcode); //bishop moves can be made by queen
        void getKingMoves(Board &gameBoard, int pos, int colorcode);
        std::pair<bool, mov> getNextMove();
        mov getMovN(int n);
        std::string getMoveCode(mov x);
};


#endif // MOVELIST_HPP_INCLUDED
