//The board class

#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED

#include <string>
#include <vector>
#include <utility>

#define ILLEGAL_MOVE 10001108
#define NEGATIVE_INFINITY -10000000
#define MATE_SCORE 1000000

#define PAWN_CODE 0
#define ROOK_CODE 1
#define KNIGHT_CODE 2
#define BISHOP_CODE 3
#define QUEEN_CODE 4
#define KING_CODE 5
#define WHITE_CODE 6
#define BLACK_CODE 7

#define CUT_NODE 0
#define ALL_NODE 1
#define PV_NODE 2

#define CASTLE_OR_ENPASENT_BASIC 0b1000000100000000000000000000000000000000000000000000000010000001

#define MAX_DEPTH 20


typedef unsigned long long u64;

//Move representation :
//first 3 = code
//next 3 = colorcode
//next 6 = from
//next 6 = to
//next 1 = takebool
//next 6 = takepos
//next 3 = takecode
//next 1 = promote
//next 3 = promotecode
//next 1 = castle
//next 6 = castle from
//next 5 = castle to
//next 1 = enPasent flag

/**
 * @class Board
 * @brief The board representation and methods
 *
 * The class represents a given position
 * The class also handles move generation
 */

class Board
{
    private:
        u64 castleorenpasent; /**< Represents ability to castle and take enpasent */
        u64 pieceBB[8]; /**< Indexes 0 - 5 represent pawns, rooks, knights, bishops, queens and kings respectively. 6 represents white pieces, 7 black. */
        int materialEval; /**< The current materialEvaluation for the board */
        u64 zorHash; /**< The current zobrist hash of the board */
        int enpasentCol; /**< The column where enpasant is available */

    public:
        int halfMoveClock; /**< A counter used to indicate the number of halfmoves since the last irreversible move for 50 move rule*/
        int stageOfGame; /**< Indicates whether the game is in middlegame or endgame. 0 for middlegame, 1 for endgame */
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
		 * @param colorcode  6 for white, 7 for black
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
        /**
		 * @fn getCastleOrEnpasent
		 * @brief Returns the u64 indicating castling and enpasent availability
		 * @return u64 indicating castling and enPasent availability
		 */
        u64 getCastleOrEnpasent();
        /**
		 * @fn nextCastleOrEnpasent
		 * @brief Removes enpasent availablity, as that can only be done one turn after the pawn moves
		 * @return u64 updated castleOrEnpasent bitboard
		 */
        u64 nextCastleOrEnpasent();
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
        int getPieceFromPos(int pos);
        /**
		 * @fn setBB
		 * @brief Set pieceBB[code] to value
		 * @param code Index to set
		 * @param value Value to set it to
		 */
        void setBB(int code, u64 value);
        /**
		 * @fn setCastleOrEnpas
		 * @brief Sets the CastleOrEnpasent u64 to a given value
		 * @param value The value to set it to
		 * @return void
		 */
        void setCastleOrEnpas(u64 value);
        /**
		 * @fn makeMove
		 * @brief Moves a piece from place to place, doesn't include taking/promotion/castling.
		 * @param code Type of piece
		 * @param colorcode Piece color
		 * @param from Position the piece is in
		 * @param to Position to move the piece to
		 */
        void makeMove(int code, int colorcode, int from, int to);
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
		 * @fn promotePawn
		 * @brief Handles pawn promotion
		 * @param colorcode pawn color
		 * @param from Position the pawn was in
		 * @param to Position where the pawn is promoting
		 * @param code Code for the piece the pawn is promoting to
		 */
        void promotePawn(int colorcode, int from, int to, int code);
        /**
		 * @fn takePiece
		 * @brief Removes a piece from the given position
		 * @param position Position to remove piece from
		 */
        void takePiece(std::pair<int, int> position);
        /**
		 * @fn inCheck
		 * @brief Checks if the king of a given color is in check
		 * @param colorcode Check if the king of this color is in check
		 * @return True for incheck, false otherwise.
		 */
        bool inCheck(int colorcode);
        /**
		 * @fn getAttacked
		 * @brief Returns all the squares attacked by a certain color
		 * @param colorcode Attacking color
		 * @return bitboard of attacked squares
		 */
        u64 getAttacked(int colorcode);
        /**
		 * @fn getAttackedPawn
		 * @brief If the given square is attacked by a pawn
		 * @param colorcode Defending color
		 * @return true/false
		 */
        bool getAttackedPawn(int colorcode, int pos, u64 oppcolorboard);
        /**
		 * @fn getAttackedKing
		 * @brief If the given square is attacked by a King
		 * @param colorcode Defending color
		 * @return true/false
		 */
        bool getAttackedKing(int pos, u64 oppcolorboard);
        /**
		 * @fn getAttackedKnight
		 * @brief If the given square is attacked by a Knight
		 * @param colorcode Defending color
		 * @return true/false
		 */
        bool getAttackedKnight(int pos, u64 oppcolorboard);
        /**
		 * @fn getAttackedBishopQueen
		 * @brief If the given square is attacked by a Bishop or Queen
		 * @param colorcode Defending color
		 * @return true/false
		 */
        bool getAttackedBishopQueen(int pos, u64 oppbisqueen);
        /**
		 * @fn getAttackedRookQueen
		 * @brief If the given square is attacked by a Rook or Queen
		 * @param colorcode Defending color
		 * @return true/false
		 */
        bool getAttackedRookQueen(int pos, u64 opprookqueen);
        /**
		 * @fn putPiece
		 * @brief Puts a given piece in a given position
		 * @param code The piece code of the piece to place on the board
		 * @param colorcode The color code of the piece to place on the board
		 * @param position A pair of integers idicating where to place the piece
		 * @return void
		 */
        void putPiece(int code, int colorcode, int position);
        /**
		 * @fn specTakePiece
		 * @brief Removes a given piece from a given position
		 * @param code The piece code of the piece to remove from the board
		 * @param colorcode The color code of the piece to remove from the board
		 * @param position A pair of integers idicating the position of the piece
		 * @return void
		 */
        void specTakePiece(int code, int colorcode, int position);
        /**
		 * @fn makeMov
		 * @brief Makes a move given a mov struct
		 * @param theMove A mov struct representing the given move
		 * @return void
		 */
        void makeMov(u64 theMove);
        /**
		 * @fn unMakeMov
		 * @brief UnMakes a move given a mov struct and the non reversible elements of a position
		 * @param theMove A mov struct representing the given move
		 * @param oldCastleOrEnpas The CastleOrEnpasent value before the move was made
		 * @param lastEnpasent The column where an enpasant was available or -1
		 * @param oldHash The zobrist hash before the move was made
		 * @return void
		 */
        void unMakeMov(u64 theMove, u64 oldCastleOrEnpas, int lastEnpasent, u64 oldHash, int halfMoveNumber);
        /**
		 * @fn setEvaluation
		 * @brief Sets the materialEval of the board to a given number
		 * @param eval The value to set materialEval to
		 * @return void
		 */
        void setEvaluation(int eval);
        /**
		 * @fn getEvaluation
		 * @brief Returns the materialEval of the board
		 * @return int The materialEval of the board (not including the mobility score)
		 */
        int getEvaluation();
        /**
		 * @fn setEnpasentCol
		 * @brief Sets enpasent available for a given column
		 * @param x The column in which enpasent is available
		 * @return void
		 */
        void setEnpasentCol(int x);
        /**
		 * @fn getEnpasentCol
		 * @brief Returns the column for which enpasent is available
		 * @return int The column in which enpasent is available
		 */
        int getEnpasentCol();
        /**
		 * @fn setZorHash
		 * @brief Sets the Zobrist hash of the board to a given number
		 * @param x The value to set the hash to
		 * @return void
		 */
        void setZorHash(u64 x);
        /**
		 * @fn getZorHash
		 * @brief Returns the Zobrist hash of the board
		 * @return u64 The Zobrist hash of the board
		 */
        u64 getZorHash();
};

#endif // BOARD_HPP_INCLUDED
