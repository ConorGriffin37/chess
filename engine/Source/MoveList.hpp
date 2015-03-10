#ifndef MOVELIST_HPP_INCLUDED
#define MOVELIST_HPP_INCLUDED

#include "Board.hpp"

#include <string>
#include <vector>
#include <utility>

/**
 * @class MoveList
 * @brief A class for generating moves from a given board
 *
 * Generates all possible moves from a given board and given color to move
 * This class also handles MVA/LLA move ordering
 */

class MoveList
{
    private:
        std::vector<mov> moves; /**< A vector containing the moves in the MoveList */
        int timesCalled; /**< The amount of moves that have been retrieved with getNextMove */
        int position; /**< The current position in the moves vector */

    public:
        bool kingTake; /**< Is the king taken by any of the generated moves (last move made was illegal) */
        MoveList(Board &gameBoard, int colorcode, mov bestMove);
        MoveList(Board& gameBoard, int colorcode, bool dontScore);
        MoveList();
        /**
		 * @fn scoreMoves
		 * @brief Scores the moves for move ordering
		 * @param bestMove The hash move from the transposition table
		 * @return void
		 */
        void scoreMoves(mov bestMove);
        void scoreMoves();
        /**
		 * @fn addMove
		 * @brief Adds a move to the moves vector based on the given parameters
		 * @param code The piece that is moving
		 * @param colorcode The color code of the piece that is moving
		 * @param from A pair of ints representing the pieces initial position
		 * @param to A pair of ints representing the pieces final position
		 * @return void
		 */
        void addMove(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to);
        /**
		 * @fn addMoveTake
		 * @brief Adds a taking move to the moves vector based on the given parameters
		 * @param code The piece that is moving
		 * @param colorcode The color code of the piece that is moving
		 * @param from A pair of ints representing the pieces initial position
		 * @param to A pair of ints representing the pieces final position
		 * @param takecode The piece code of the piece being taken
		 * @return void
		 */
        void addMoveTake(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to, int takecode);
        /**
		 * @fn addMovePro
		 * @brief Adds a promotion move to the moves vector based on the given parameters
		 * @param code The piece that is moving
		 * @param colorcode The color code of the piece that is moving
		 * @param from A pair of ints representing the pieces initial position
		 * @param to A pair of ints representing the pieces final position
		 * @param takecode The piece code of the piece being promoted to
		 * @return void
		 */
        void addMovePro(int code, int colorcode, std::pair<int, int> form, std::pair<int, int> to, int procode);
        /**
		 * @fn addMoveEnpas
		 * @brief Adds an enpasent move to the moves vector based on the given parameters
		 * @param code The piece that is moving
		 * @param colorcode The color code of the piece that is moving
		 * @param from A pair of ints representing the pieces initial position
		 * @param to A pair of ints representing the pieces final position
		 * @param takepos The position of the piece being taken en passent
		 * @return void
		 */
        void addMoveEnpas(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to, std::pair<int, int> takepos);
        /**
		 * @fn addMoveTakePro
		 * @brief Adds a taking and promotion move to the moves vector based on the given parameters
		 * @param code The piece that is moving
		 * @param colorcode The color code of the piece that is moving
		 * @param from A pair of ints representing the pieces initial position
		 * @param to A pair of ints representing the pieces final position
		 * @param takecode The piece code of the piece being taken
		 * @param The piece code being promoted to
		 * @return void
		 */
        void addMoveTakePro(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to, int takecode, int procode);
        /**
		 * @fn addMoveCastle
		 * @brief Adds a castling move to the moves vector based on the given parameters
		 * @param code The piece that is moving
		 * @param colorcode The color code of the piece that is moving
		 * @param from A pair of ints representing the kings initial position
		 * @param to A pair of ints representing the kings final position
		 * @param rookfrom A pair of ints representing the rooks initial position
		 * @param rookto A pair of ints representing the rooks final position
		 * @return void
		 */
        void addMoveCastle(int code, int colorcode, std::pair<int, int> from, std::pair<int, int> to, std::pair<int, int> rookfrom, std::pair<int, int> rookto);
        /**
		 * @fn generateMoves
		 * @brief Generates all the moves for a given board and color
		 * @param gameBoard The board to generate the moves for
		 * @param colorcode The color code to generate the moves for
		 * @return void
		 */
        void generateMoves(Board &gameBoard, int colorcode);
        /**
		 * @fn getPawnMoves
		 * @brief Generates all the pawn moves for a given board and color and position
		 * @param gameBoard The board to generate the moves for
		 * @param colorcode The color code to generate the moves for
		 * @param pos The position of the pawn
		 * @return void
		 */
        void getPawnMoves(Board &gameBoard, int pos, int colorcode);
        /**
		 * @fn getRookMoves
		 * @brief Generates all the rook moves for a given board and color and position
		 * @param gameBoard The board to generate the moves for
		 * @param colorcode The color code to generate the moves for
		 * @param code The piece code (queen can also make rook moves)
		 * @param pos The position of the piece
		 * @return void
		 */
        void getRookMoves(Board &gameBoard, int pos, int code, int colorcode); //rook moves need code as queen can make rook moves
        /**
		 * @fn getKnightMoves
		 * @brief Generates all the knight moves for a given board and color and position
		 * @param gameBoard The board to generate the moves for
		 * @param colorcode The color code to generate the moves for
		 * @param pos The position of the knight
		 * @return void
		 */
        void getKnightMoves(Board &gameBoard, int pos, int colorcode);
        /**
		 * @fn getBishopMoves
		 * @brief Generates all the bishop moves for a given board and color and position
		 * @param gameBoard The board to generate the moves for
		 * @param colorcode The color code to generate the moves for
		 * @param code The piece code (queen can also make bishop moves)
		 * @param pos The position of the piece
		 * @return void
		 */
        void getBishopMoves(Board &gameBoard, int pos, int code, int colorcode); //bishop moves can be made by queen
        /**
		 * @fn getKingMoves
		 * @brief Generates all the king moves for a given board and color and position
		 * @param gameBoard The board to generate the moves for
		 * @param colorcode The color code to generate the moves for
		 * @param pos The position of the king
		 * @return void
		 */
        void getKingMoves(Board &gameBoard, int pos, int colorcode);
        /**
		 * @fn getNextMove
		 * @brief Returns the next move to make on the board
		 * @return pair<bool, mov> The mov struct and a bool indicating whether there was a valid move to return
		 */
        std::pair<bool, mov> getNextMove();
        /**
		 * @fn getMovN
		 * @brief Returns the mov at position N in the moves vector
		 * @param n The position in the vector to return the mov from
		 * @return mov The mov at position n in the moves vector
		 */
        mov getMovN(int n);
        /**
		 * @fn getMoveNumber
		 * @brief Returns the number of pseudolegal moves that were generated
		 * @return int The length of the moves vector
		 */
        int getMoveNumber();
        /**
		 * @fn getMoveCode
		 * @brief Returns the move code in algebraic notation for the given mov
		 * @param x The mov to generate the mov code for
		 * @return string The move code for the given mov
		 */
        std::string getMoveCode(mov x);
};


#endif // MOVELIST_HPP_INCLUDED
