#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE api_tests
#define BOOST_TEST_MAIN
#define BOOST_TEST_LOG_LEVEL all

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "../Source/UCI.hpp"
#include "../Source/Search.hpp"
#include "../Source/Board.hpp"
#include "../Source/Evaluation.hpp"
#include "../Source/MoveList.hpp"

using namespace std;

BOOST_AUTO_TEST_CASE(UCI_sentPosition)
{
    cout << "Testing sentPosition function" << endl;
    BOOST_CHECK(UCI::sentPosition("position startpos moves e3e2") == false);
    BOOST_CHECK(UCI::sentPosition("position startpos moves e2e4") == true);
    BOOST_CHECK(UCI::sentPosition("position startpos") == false);
    BOOST_CHECK(UCI::sentPosition("position fen not a fen") == false);
}

BOOST_AUTO_TEST_CASE(evaluation_evaluateBoard)
{
    cout << "Testing evaluateBoard function" << endl;
    Board testBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    BOOST_CHECK(Evaluation::evaluateBoard(testBoard) == 0); //evaluation should be 0 for initial board
    testBoard = Board("7k/8/8/8/8/8/8/7K w - - 0 1");
    BOOST_CHECK(Evaluation::evaluateBoard(testBoard) == 0); //no piece difference, symmetrical position
}

BOOST_AUTO_TEST_CASE(evaluation_CheckForDoublePawns)
{
    Evaluation::initpopCountOfByte(); //initialising look-up table
    cout << "Testing CheckForDoublePawns function" << endl;
    Board testBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    BOOST_CHECK(Evaluation::CheckForDoublePawns(6, testBoard) == 0); //No double pawns for white
    BOOST_CHECK(Evaluation::CheckForDoublePawns(7, testBoard) == 0); //No double pawns for black
    testBoard = Board("8/8/4kP2/8/5P2/PP1P2P1/PPP1P2P/3K4 w - - 0 1");
    BOOST_CHECK(Evaluation::CheckForDoublePawns(6, testBoard) == 3); //3 sets of white double pawns
    testBoard = Board("8/8/4kP2/8/5P2/PPPPP1PP/PPP1P2P/3K4 w - - 0 1");
    BOOST_CHECK(Evaluation::CheckForDoublePawns(6, testBoard) == 6); //6 sets of white double pawns
    testBoard = Board("4k3/pppppppp/pppppppp/2b2b2/P5P1/P1qK1PP1/P4PP1/6q1 w - - 0 1");
    BOOST_CHECK(Evaluation::CheckForDoublePawns(7, testBoard) == 8); //8 sets of black double pawns
}

BOOST_AUTO_TEST_CASE(evaluation_rooksOnOpenFile)
{
    cout << "Testing rooksOnOpenFile function" << endl;
    Board testBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    int temp = Evaluation::CheckForDoublePawns(6, testBoard); //check for double pawns initialises array of open files
    temp = Evaluation::CheckForDoublePawns(7, testBoard);
    BOOST_CHECK(Evaluation::rooksOnOpenFile(6, testBoard) == 0); //no rooks on open file for white
    BOOST_CHECK(Evaluation::rooksOnOpenFile(7, testBoard) == 0); //no rooks on open file for black
    testBoard = Board("r2k1r2/pp2p1pp/8/5P2/7P/8/1P6/RRRKRRRR w - - 0 1");
    temp = Evaluation::CheckForDoublePawns(6, testBoard);
    temp = Evaluation::CheckForDoublePawns(7, testBoard);
    BOOST_CHECK(Evaluation::rooksOnOpenFile(6, testBoard) == 4); //4 rooks on open file for white
    BOOST_CHECK(Evaluation::rooksOnOpenFile(7, testBoard) == 1); //1 rook on open file for black
}

BOOST_AUTO_TEST_CASE(board_inCheck)
{
    cout << "Testing inCheck function" << endl;
    Board testBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    BOOST_CHECK(testBoard.inCheck(6) == false); //white isn't in check
    BOOST_CHECK(testBoard.inCheck(7) == false); //black isn't in check
    testBoard = Board("4k3/8/4r3/8/8/8/8/4K3 w - - 0 1");
    BOOST_CHECK(testBoard.inCheck(6) == true); //white is in check
    BOOST_CHECK(testBoard.inCheck(7) == false); //black isn't in check
    testBoard = Board("k1r5/8/1N6/8/8/8/8/4K3 w - - 0 1");
    BOOST_CHECK(testBoard.inCheck(7) == true); //black is in check
    BOOST_CHECK(testBoard.inCheck(6) == false); //white isn't in check
}


BOOST_AUTO_TEST_CASE(board_simpleMakeMove)
{
    cout << "Testing simpleMakeMove function" << endl;
    Board testBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    pair<int, int> startPosition = make_pair('e' - 'a', '2' - '1');
    pair<int, int> endPosition = make_pair('e' - 'a', '4' - '1');
    testBoard.simpleMakeMove(startPosition, endPosition, ' ');
    BOOST_CHECK(testBoard.getPieceFromPos(4, 3) == 0); //pawn at e4
}

BOOST_AUTO_TEST_CASE(board_takePiece)
{
    cout << "Testing takePiece function" << endl;
    Board testBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    testBoard.takePiece(make_pair(0, 0));
    BOOST_CHECK(testBoard.getPieceFromPos(0, 0) == -1); //piece has been removed
}

BOOST_AUTO_TEST_CASE(board_getPieceCode)
{
    cout << "Testing getPieceCode function" << endl;
    Board testBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    BOOST_CHECK(testBoard.getPieceCode('p') == 0); //pawn
    BOOST_CHECK(testBoard.getPieceCode('P') == 0); //pawn
    BOOST_CHECK(testBoard.getPieceCode('r') == 1); //rook
    BOOST_CHECK(testBoard.getPieceCode('R') == 1); //rook
    BOOST_CHECK(testBoard.getPieceCode('n') == 2); //knight
    BOOST_CHECK(testBoard.getPieceCode('N') == 2); //knight
    BOOST_CHECK(testBoard.getPieceCode('b') == 3); //bishop
    BOOST_CHECK(testBoard.getPieceCode('B') == 3); //bishop
    BOOST_CHECK(testBoard.getPieceCode('q') == 4); //queen
    BOOST_CHECK(testBoard.getPieceCode('Q') == 4); //queen
    BOOST_CHECK(testBoard.getPieceCode('k') == 5); //king
    BOOST_CHECK(testBoard.getPieceCode('K') == 5); //king
}


int getLegalMoves(Board testBoard, int playerColor) {
    mov bad;
    bad.code = -1;
	MoveList possibleMoves = MoveList(testBoard, ((playerColor == 1) ? 6 : 7), bad);
	u64 castle = testBoard.getCastleOrEnpasent();
    u64 lastHash = testBoard.getZorHash();
    int enpasCol = testBoard.getEnpasentCol();
	int legalMoves = 0;

	while (true) {
		pair<bool, mov> get = possibleMoves.getNextMove();
		if (get.first) {
			testBoard.makeMov(get.second);
			if (testBoard.inCheck(((playerColor == 1) ? 6 : 7)) == false) {
                legalMoves++;
			}
			testBoard.unMakeMov(get.second, castle, enpasCol, lastHash);
		} else {
			break;
		}
	}
	return legalMoves;
}

BOOST_AUTO_TEST_CASE(MoveList_generateMoves)
{
	cout << "Testing MoveList class" << endl;
	Board testBoard = Board("k5R1/7R/8/8/8/8/8/K7 b - - 0 1");
	BOOST_CHECK(getLegalMoves(testBoard, -1) == 0); //black is check mated
	testBoard = Board("k7/8/8/8/8/8/8/K7 w - - 0 1");
	BOOST_CHECK(getLegalMoves(testBoard, 1) == 3); //only piece is king in corner
	testBoard = Board("k7/8/8/8/4K3/8/8/8 w - - 0 1");
	BOOST_CHECK(getLegalMoves(testBoard, 1) == 8); //only piece is king in center
}

BOOST_AUTO_TEST_CASE(search_RootAlphaBeta)
{
	cout << "Testing search" << endl;
	Board testBoard = Board("k7/pppp4/8/8/8/8/8/K4R2 w - - 0 1");
	BOOST_CHECK(Search::RootAlphaBeta(testBoard, 1, 4).first == "f1f8"); //it should find the checkmate for white
}

//Perft is good for verifying the correctness of the move generation, generate all the leaf nodes at a given depth from a known position.
//This also shows how fast the move generation works in relation to other engines (slow, magic bitboards would speed up move generation)
u64 Perft(int depth, Board& gameBoard, int playerColor)
{
    if (depth == 0) {
        if (gameBoard.inCheck(((playerColor == 1) ? 7 : 6)) == true) {
            return 0;
        }
        return 1;
    }
 
    u64 nodes = 0;
    u64 castle = gameBoard.getCastleOrEnpasent();
    u64 lastHash = gameBoard.getZorHash();
    int enpasCol = gameBoard.getEnpasentCol();
 
    MoveList possibleMoves(gameBoard, ((playerColor == 1) ? 6 : 7), true);
 
    if (possibleMoves.kingTake) {
        return 0;
    }
 
    int movNumber = possibleMoves.getMoveNumber();
 
    for (int i = 0; i < movNumber; i++) {
        u64 theMove = possibleMoves.getMovN(i);
        gameBoard.makeMov(theMove);
 
        //if (gameBoard.inCheck(((playerColor == 1) ? 6 : 7)) == false) {
            nodes += Perft(depth - 1, gameBoard, playerColor*-1);
        //}
        gameBoard.unMakeMov(theMove, castle, enpasCol, lastHash);
    }
 
    return nodes;
}

BOOST_AUTO_TEST_CASE(perft_Test)
{
    cout << "Testing move generation (perft), this is slow" << endl;
    Board testBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); //initial position
    BOOST_CHECK(Perft(4, testBoard, 1) == 197281); //Known value from the chess programming wiki http://chessprogramming.wikispaces.com/Perft+Results
    testBoard = Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    BOOST_CHECK(Perft(4, testBoard, 1) == 4085603);
    testBoard = Board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
    BOOST_CHECK(Perft(5, testBoard, 1) == 674624);
    testBoard = Board("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");
    BOOST_CHECK(Perft(4, testBoard, -1) == 422333);
    testBoard = Board("rnbqkb1r/pp1p1ppp/2p5/4P3/2B5/8/PPP1NnPP/RNBQK2R w KQkq - 0 6");
    BOOST_CHECK(Perft(3, testBoard, 1) == 53392);
    testBoard = Board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    BOOST_CHECK(Perft(4, testBoard, 1) == 3894594);
}
