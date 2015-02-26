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

using namespace std;

BOOST_AUTO_TEST_CASE(UCI_sentPosition)
{
	cout << "Testing sentPosition function" << endl;
	UCI uciObject;
	BOOST_CHECK(uciObject.sentPosition("position startpos moves e3e2") == false);
	BOOST_CHECK(uciObject.sentPosition("position startpos moves e2e4") == true);
	BOOST_CHECK(uciObject.sentPosition("position startpos") == false);
	BOOST_CHECK(uciObject.sentPosition("position fen not a fen") == false);
}

BOOST_AUTO_TEST_CASE(board_evaluateBoard)
{
	cout << "Testing evaluateBoard function" << endl;
	Board testBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	BOOST_CHECK(testBoard.evaluateBoard() == 0); //evaluation should be 0 for initial board
	testBoard = Board("7k/8/8/8/8/8/8/7K w - - 0 1");
	BOOST_CHECK(testBoard.evaluateBoard() == 0); //no piece difference, symmetrical position
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

BOOST_AUTO_TEST_CASE(board_getMoves)
{
	cout << "Testing getMoves function" << endl;
    Board testBoard = Board("k5R1/7R/8/8/8/8/8/K7 b - - 0 1");
    BOOST_CHECK(testBoard.getBoards(7).size() == 0); //black is check mated
    testBoard = Board("k7/8/8/8/8/8/8/K7 w - - 0 1");
    BOOST_CHECK(testBoard.getBoards(6).size() == 3); //only piece is king in corner
    testBoard = Board("k7/8/8/8/4K3/8/8/8 w - - 0 1");
    BOOST_CHECK(testBoard.getBoards(6).size() == 8); //only piece is king in center
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

BOOST_AUTO_TEST_CASE(search_RootAlphaBeta)
{
    cout << "Testing search" << endl;
    Board testBoard = Board("k7/pppp4/8/8/8/8/8/K4R2 w - - 0 1");
    Search searchClass;
    BOOST_CHECK(searchClass.RootAlphaBeta(testBoard, 1, 4) == "f1f8"); //it should find the checkmate for white
}



