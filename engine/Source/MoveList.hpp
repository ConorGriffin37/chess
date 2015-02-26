#ifndef MOVELIST_HPP_INCLUDED
#define MOVELIST_HPP_INCLUDED

#include <string>
#include <vector>
#include <utility>
#include "Board.hpp"

struct mov
{
    int code;
    int colorcode;
    std::pair<int, int> from;
    std::pair<int, int> to;
    bool take = false;
    bool promote = false;
    bool enPas = false;
    bool castle = false;
    std::pair<int, int> rookfrom;
    std::pair<int, int> rookto;
    int procode;
    int takecode;
    std::pair<int, int> takepos;
    int score;
};

class MoveList
{
    private:
        std::vector<mov> moves;
        int timesCalled;
        int position;

    public:
        MoveList(Board &gameBoard, int colorcode);
        std::string getMoveCode(mov);
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
};


#endif // MOVELIST_HPP_INCLUDED
