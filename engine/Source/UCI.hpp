#ifndef UCI_H
#define UCI_H

#include "Board.hpp"

#include <string>

using namespace std;

/**
 * @class UCI
 * @brief Handles the UCI communication protocol
 *
 */

class UCI
{
    public:
        static Board currentBoard; /**< The board set-up with the position command */
        static int currentColor; /**< The player who is next to move. 1 for white, -1 for black */
        static bool quit; /**< A flag to be set to true when the quit command is received */
        static bool killSearch; /**< A flag to be set to kill the current search */
        static string ponderMove; /**< The last move sent to the engine as the move the engine wants to ponder on */
        static bool ponderHit; /**< A flag to be set to true if the move currently being pondered on is the move made by the opponent */
        /**
		 * @fn WaitForInput
		 * @brief Waits to receive input via stdin
		 * @return bool Returns true if the command is recognised, false if not
		 */
        static bool waitForInput();
        /**
		 * @fn outputBestMove
		 * @brief Outputs the best move
		 * @param moveString A string giving the best move in algebraic notation
		 * @return void
		 */
        static void outputBestMove(string moveString); //string like e2e4
        /**
		 * @fn identification
		 * @brief Outputs the name of the engine and the authors
		 * @return void
		 */
        static void identification();
        /**
		 * @fn sentPosition
		 * @brief Sets up a position in response to the position UCI command
		 * @param input A string containing the input from stdin
		 * @return bool Returns true if the position can be set up and false if not
		 */
        static bool sentPosition(string input);
        /**
		 * @fn startCalculating
		 * @brief Starts searching for the best move at the current position
		 * @param input A string containing the input from stdin
		 * @return bool Returns true if the command is valid and false if not
		 */
        static bool startCalculating(string input);
        /**
		 * @fn sendInfo
		 * @brief Used for outputting information during calculation
		 * @param info A string containing the information to output
		 * @return void
		 */
        static void sendInfo(string info);
};

#endif // UCI_H
