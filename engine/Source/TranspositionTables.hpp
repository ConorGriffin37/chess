#ifndef Transposition_HPP_INCLUDED
#define Transposition_HPP_INCLUDED

#include "Board.hpp"

#include <string>
#include <vector>
#include <utility>
#include <string>

#define tab_size 1048576 //4194304
#define tab_mask 0b11111111111111111111 //1048576 in binary

struct entry
{
    u64 signature;
    u64 best;
    int depth;
    int score;
    bool ancient = false;
};

/**
 * @class TranspositionTables
 * @brief Adds and removes positions to the transposition table
 *
 */


class TranspositionTables
{
    private:
        static u64 zobrist[781]; /**< The randomly generated numbers for getting the zobrist hash of a position */
        static entry Table[tab_size]; /**< The transposition table */

    public:
        /**
		 * @fn initZobrist
		 * @brief Generates the random u64s for zobrist hashing
		 * @return void
		 */
        static void initZobrist();
        /**
		 * @fn getSquareHash
		 * @brief Returns the hash for a given piece in a given square
		 * @param pos An integer representing the position of the piece on the chess board
		 * @param code The code of the piece, 0 - 5, pawn, rook, knight, bishop, queen king
		 * @param playerColor 6 for white, 7 for black
		 * @return u64 The zobrist hash for a given piece in a given square
		 */
        static u64 getSquareHash(int pos, int code, int playerColor);
        /**
		 * @fn getBlackHash
		 * @brief Returns the hash for black to move
		 * @return u64 The zobrist hash for black to move
		 */
        static u64 getBlackHash();
        /**
		 * @fn getCastleHash
		 * @brief Returns the hash for one of four castling options
		 * @param n 0, 1, 2 or 3. An integer indicating what castling scenario is open
		 * @return u64 The zobrist hash for the given castling option
		 */
        static u64 getCastleHash(int n);
        /**
		 * @fn getEnpasentHash
		 * @brief Returns the hash for the enpasent hash for the given column
		 * @param x An integer indicating in which column enpasent is available 0-7
		 * @return u64 The zobrist hash for the given enpasant
		 */
        static u64 getEnpasentHash(int x);
        /**
		 * @fn getBoardHash
		 * @brief Returns the hash for the given board
		 * @param gameBoard The board to calculate a hash for
		 * @param playerColor 6 for white, 7 for black
		 * @return u64 The zobrist hash for the given Board
		 */
        static u64 getBoardHash(Board& gameBoard, int playerColor);
        /**
		 * @fn getCastleSquare
		 * @brief Returns the hash for the given castling square
		 * @param pos The position of the castling square
		 * @return u64 The zobrist hash for the given castling square
		 */
        static u64 getCastleSquare(int pos);
        /**
		 * @fn getBest
		 * @brief Returns the best move the last time this board was evaluated
		 * @param signature The hash of the board to check
		 * @return mov The best move the last time this board was evaluated
		 */
        static u64 getBest(u64 signature);
        /**
		 * @fn setEntry
		 * @brief Adds a new entry to the transposition table
		 * @param signature The hash of the entry to add
		 * @param bestmove The best move found at this position
		 * @param depth The depth this position was evaluated to
		 * @param score The score of this position
		 * @return void
		 */
        static void setEntry(u64 signature, u64 bestmove, int depth, int score);
        /**
		 * @fn setOld
		 * @brief Marks which entries in the transposition table are from a previous search
		 * @return void
		 */
        static void setOld();
        /**
		 * @fn getPrincipalVariation
		 * @brief Retrievs the principal variation from a given position from the transposition table
		 * @param gameBoard The Board to find the principal variation from
		 * @param depth The number of plies of the PV to retrieve
		 * @return void
		 */
        static std::string getPrincipalVariation(Board gameBoard, int depth);
        static void initEntryCount();
        static int getEntryCount();
};


#endif // Transposition_HPP_INCLUDED
