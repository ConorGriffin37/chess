#ifndef EVALUATION_H
#define EVALUATION_H

#include "Board.hpp"

/**
 * @class Evaluation
 * @brief Evaluates a Board
 *
 * Evaluates a board based on material difference and position
 */

class Evaluation
{
 public:
        /**
		 * @fn CheckForDoublePawns
		 * @brief Returns the number of double pawns found for a certain color
		 * @param colorCode integer indicating the color to check, 6 for white, 7 for black
		 * @param evalBoard The board to evaluate
		 * @return An int indicating the number of double pawns on the board for the given color
		 */
        static int CheckForDoublePawns(int colorCode, Board& evalBoard);
        /**
		 * @fn rooksOnOpenFile
		 * @brief Returns the number of rooks on an open file for a given color
		 * @param colorCode integer indicating the color to check, 6 for white, 7 for black
		 * @param evalBoard The board to evaluate
		 * @return An int indicating the number of rooks on an open file on the board for the given color
		 */
        static int rooksOnOpenFile(int colorCode, Board& evalBoard);
        /**
		 * @fn GetMobilityScore
		 * @brief Returns the mobility score for a given board (white mobility - black mobility)
		 * @param evalBoard The board to evaluate
		 * @return An int indicating the mobility score of the board
		 */
        static int GetMobilityScore(Board& evalBoard);
        /**
		 * @fn initpopCountOfByte
		 * @brief initialises the table for quickly computing the population count of a byte
		 * @return void
		 */
        static void initpopCountOfByte();
        /**
		 * @fn popCount
		 * @brief Quickly computes the population count of a bitboard
		 * @param x The bitboard to check
		 * @return int representing the population count of the bitboard
		 */
        static int popCount(u64 x);
        /**
		 * @fn evaluateBoard
		 * @brief Evaluates a given board
		 * @param boardToEvaluate The board to check
		 * @return int The score of the board
		 */
        static int evaluateBoard(Board& boardToEvaluate);
        /**
		 * @fn getPosScore
		 * @brief Returns the position score for a given piece
		 * @param code The piece code
		 * @param colorCode the colorCode of the piece
		 * @param position The position of the piece on the board
		 * @return int The score of the piece at the given position
		 */
        static int getPosScore(int code, int colorCode, int position, int gameStage);
        /**
		 * @fn stageOfGame
		 * @brief Returns whether a game is in middlegame or endgame
		 * @param evalBoard The board to check
		 * @return int 0 for middlegame, 1 for endgame
		 */
        static int stageOfGame(Board& evalBoard);
        /**
		 * @fn numMinorPieces
		 * @brief Returns the number of minor pieces on a given board for a certain color
		 * @param evalBoard The board to check
		 * @return int The number of rooks, knights and bishops for a certain color
		 */
        static int numMinorPieces(int colorCode, Board& evalBoard);
};

#endif // EVALUATION_H
