//The board class

#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED

#include <string>
#include <vector>
#include <utility>



typedef unsigned long long u64;

class Board
{
    private:
        u64 castleorenpasent;
        u64 pieceBB[8];

    public:
        Board(std::string fen);
        Board();
        u64 getPieces();  //return all piece locations
        u64 getPieceColor(int colorcode); //return all pices of a certain color, black or white
        u64 getPiece(int code);  //return all of a certain piece type e.g Knights
        u64 getPieceAndColor(int code, int colorcode); //return all of a certain type and color e.g Black Pawns
        u64 getCastleOrEnpasent();
        u64 nextCastleOrEnpasent();
        u64 getAttacked(int colorcode); //gets all the squares attacked by a certain colour
        int getPieceCode(char p); //get the piece code from the FEN char notation
        int getColorCode(char p); //get the color code from the FEN char notation
        int getPieceFromPos(int x, int y); //gets the piece code in a given position
        void setBB(int code, u64 value);
        void setCastleOrEnpas(u64 value);
        void makeMove(int code, int colorcode, std::pair <int, int> from, std::pair <int, int> to); //moves a piece from position to position
        bool simpleMakeMove(std::pair <int, int> from, std::pair <int, int> to, char promote); //makes a move given the positions
        std::string getMove(Board nextboard); //gets the move code (eg. e4e5) from a given finishing position
        void promotePawn(int colorcode, std::pair <int, int> from, std::pair <int, int> to, int code); //color of pawn, column, what to promote it to
        void takePiece(std::pair<int, int> position); //removes the unknown piece from the position
        int evaluateBoard();  //simple evaluation function based on piece values
        std::vector<Board> getBoards(int colorcode); //gets all legal boards from a given board
        std::vector<Board> getMoves(int positon, int code, int colorcode); //makes all moves with a certain piece (does not check for king in check)
        bool inCheck(int colorcode); //checks if the piece of a given color is in check
};

#endif // BOARD_HPP_INCLUDED
