//The board class

#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED

#include <string>
#include <vector>
#include <utility>

typedef unsigned long long u64;

/**
 * @class Board
 * @brief The board representation and methods
 *
 * The class represents a given position
 * The class also handles move generation and board evaluation
 */

class Board
{
    private:
        u64 castleorenpasent; /**< Represents ability to castle and take enpasent */
        u64 pieceBB[8]; /**< Indexes 0 - 5 represent pawns, rooks, knights, bishops, queens and kings respectively. 6 represents white pieces, 7 black. */

    public:
        Board(std::string fen);
        Board();
        /**
		 * @fn getPieces
		 * @brief Returns bitboard of all piece locations
		 * @return u64 representing ocupied spaces
		 */
        u64 getPieces();
        /**
		 * @fn getPieceColor
		 * @brief Returns bitboard of all piece locations or a certain color
		 * @param colorcode  6 for white, 7 for white
		 * @return u64 representing ocupied spaces of color
		 */
        u64 getPieceColor(int colorcode);
        /**
		 * @fn getPiece
		 * @brief Returns bitboard of all piece locations of a certain type
		 * @param code 0 - 5 represent pawns, rooks, knights, bishops, queens and kings respectively.
		 * @return u64 representing the locations of all of a certain piece
		 */
        u64 getPiece(int code);
        /**
		 * @fn getPieceAndColor
		 * @brief Returns bitboard of all piece locations of a certain type and color
		 * @param code 0 - 5 represent pawns, rooks, knights, bishops, queens and kings respectively.
		 * @param colorcode 6 for white, 7 for black
		 * @return u64 representing the locations of all of a certain piece of a certain color
		 */
        u64 getPieceAndColor(int code, int colorcode);
        u64 getCastleOrEnpasent();
        /**
		 * @fn nextCastleOrEnpasent
		 * @brief Removes enpasent availablity, as that can only be done one turn after the pawn moves
		 * @return updated castleOrEnpasent bitboard
		 */
        u64 nextCastleOrEnpasent();
        /**
		 * @fn getAttacked
		 * @brief Returns all attacked squares for a certain color
		 * @param colorcode 6 for white, 7 for black
		 * @return u64 bitboard representing all attacked squares
		 */
        u64 getAttacked(int colorcode);
        /**
		 * @fn getPieceCode
		 * @brief get the piece code from the FEN char notation
		 * @param p FEN char notation
		 * @return 0 - 5, 0 for pawns ect.
		 */
        int getPieceCode(char p);
        /**
		 * @fn getColorCode
		 * @brief get the color code from the FEN char notation
		 * @param p FEN char notation
		 * @return 6 for white, 7 for black
		 */
        int getColorCode(char p);
        /**
		 * @fn getPieceFromPos
		 * @brief get the piece code of the piece in a given positon
		 * @param x (0 - 7) x coordinate
         * @param y (0 - 7) y coordinate
		 * @return 0 - 5, 0 for pawns ect.
		 */
        int getPieceFromPos(int x, int y);
        /**
		 * @fn setBB
		 * @brief Set pieceBB[code] to value
		 * @param code Index to set
		 * @param value Value to set it to
		 */
        void setBB(int code, u64 value);
        void setCastleOrEnpas(u64 value);
        /**
		 * @fn makeMove
		 * @brief Moves a piece from place to place, doesn't include taking/promotion/castling.
		 * @param code Type of piece
		 * @param colorcode Piece color
		 * @param from Position the piece is in
		 * @param to Position to move the piece to
		 */
        void makeMove(int code, int colorcode, std::pair <int, int> from, std::pair <int, int> to);
        /**
		 * @fn simpleMakeMove
		 * @brief Less effecient make move that requires less info.
		 * @param from Position the piece is in
		 * @param to Position to move the piece to
		 * @param promote ' ' for no promotion, otherwise piece code if a pawn is promoting
		 * @return true for success, false for failure
		 */
        bool simpleMakeMove(std::pair <int, int> from, std::pair <int, int> to, char promote);
        /**
		 * @fn getMove
		 * @brief gets the move that was made to advance to the given board
		 * @param nextboard The move after the move was made
		 * @return The move code, e.g "e4e5".
		 */
        std::string getMove(Board nextboard);
        /**
		 * @fn promotePawn
		 * @brief Handles pawn promotion
		 * @param colorcode pawn color
		 * @param from Position the pawn was in
		 * @param to Position where the pawn is promoting
		 * @param code Code for the piece the pawn is promoting to
		 */
        void promotePawn(int colorcode, std::pair <int, int> from, std::pair <int, int> to, int code);
        /**
		 * @fn takePiece
		 * @brief Removes a piece from the given position
		 * @param position Position to remove piece from
		 */
        void takePiece(std::pair<int, int> position);
        /**
		 * @fn evaluateBoard
		 * @brief Evaluates a board based on material and piece position
		 * @return Evaluation, + means white is ahead, - means black is ahead. 100 = value of a pawn
		 */
        int evaluateBoard();
        /**
		 * @fn getBoards
		 * @brief Gets all the resulting boards after 1 move
		 * @param colorcode Color whose turn it is to move
		 * @return Vector of the boards
		 */
        std::vector<Board> getBoards(int colorcode);
        /**
		 * @fn getMoves
		 * @brief Gets all the resulting boards from a single pieces moves
		 * @param position Position of the piece (in bits)
		 * @param code Type of piece
		 * @param colorcode Color whose turn it is to move
		 * @return Vector of the boards
		 */
        std::vector<Board> getMoves(int positon, int code, int colorcode);
        /**
		 * @fn inCheck
		 * @brief Checks if the king of a given color is in check
		 * @param colorcode Check if the king of this color is in check
		 * @return True for incheck, false otherwise.
		 */
        bool inCheck(int colorcode); //checks if the piece of a given color is in check
};

#endif // BOARD_HPP_INCLUDED
