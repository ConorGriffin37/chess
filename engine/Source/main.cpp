/*#include <iostream>
#include <cmath>
#include <vector>
#include "board.hpp"
#include "MoveList.hpp"
#include "Evaluation.hpp"

using namespace std;

char gets(u64 x)
{
    if (x >= 10) {
        return 'a' + (x - 10);
    }
    return '0' + x;
}

void outbitboard(u64 n)
{
    cout << "Board : " << endl;
    string ret = "";
    while (n > 0) {
        ret = gets(n % 2) + ret;
        n = n/2;
    }
    while (ret.length() < 64) {
        ret = "0" + ret;
    }
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0 and i != 0) {
            cout << endl;
        }
        cout << ret[i];
    }
    cout << endl;
}

string moves[] = {"g1f3", "b8c6", "b1c3", "e7e5", "e2e3", "g8f6", "f1b5", "e5e4", "d1e2", "e4f3"};

Board poss[10000];

int main()
{
    //Board MyBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Board TestBoard("r1bq3r/1p1k1ppp/p1np4/3Q4/8/5N2/PPP2PPP/2KR1B1R b - - 0 14");
    string moveIn = "d7e6";
    pair<int, int> startPosition = make_pair(moveIn[0] - 'a', moveIn[1] - '1');
    pair<int, int> endPosition = make_pair(moveIn[2] - 'a', moveIn[3] - '1');
    char promote = ' ';
    TestBoard.simpleMakeMove(startPosition, endPosition, promote);
    cout << TestBoard.inCheck(7) << endl;
    //return 0;
    Board MyBoard("r1bq3r/1p1k1ppp/p1np4/3Q4/8/5N2/PPP2PPP/2KR1B1R b - - 0 14"); //pawn promotion test
    Board LastBoard = MyBoard;
    //MyBoard.simpleMakeMove(make_pair(0, 6), make_pair(0, 7), 'q'); //simple make move test
    //return 0;
    //outbitboard(MyBoard.getPieces());

    //testing simple make move
    MoveList theMoves = MoveList(MyBoard, 7);
    pair<bool, mov> theMove;
    u64 castle = MyBoard.getCastleOrEnpasent();
    MyBoard.setEvaluation(Evaluation::evaluateBoard(MyBoard));
    cout << "Evaluation : " << MyBoard.getEvaluation() << endl;
    while (true) {
        theMove = theMoves.getNextMove();
        if (theMove.first) {
            if (theMoves.getMoveCode(theMove.second) == "d7e6") {
                cout << "Making move : " << theMoves.getMoveCode(theMove.second) << endl;
                MyBoard.makeMov(theMove.second);
                if (MyBoard.inCheck(7)) {
                    cout << "InCheck is true" << endl;
                }
                cout << "Evaluation : " << MyBoard.getEvaluation() << endl;
                cout << "GetMove : " << LastBoard.getMove(MyBoard) << endl;
                MyBoard.unMakeMov(theMove.second, castle);
                cout << "Undone Evaluation : " << MyBoard.getEvaluation() << endl;
            }
        } else {
            break;
        }
    }
    outbitboard(MyBoard.getPieces());
    return 0;
    vector<Board> newBoards = MyBoard.getBoards(6);
    cout << "Old Moves : " << endl;
    for (int i = 0; i < newBoards.size(); i++) {
        cout << MyBoard.getMove(newBoards[i]) << endl;
    }
    //Board MyBoard("r1bqkb1r/pppp1ppp/2n2n2/1B6/8/2N1Pp2/PPPPQPPP/R1B1K2R w KQkq - 0 6");
    //Board MyBoard("rnbqk2r/p2p1Bpp/1pp2n2/2b1p1N1/3PP3/8/PPP2PPP/RNBQK2R b KQkq - 0 6");
    //cout << MyBoard.inCheck(7) << endl; //attacked check
    //outbitboard(MyBoard.getAttacked(6)); //testing attacked
    //outbitboard(MyBoard.getCastleOrEnpasent()); //castle and enpasent places
    //outbitboard(MyBoard.getPieceColor(6)); //black pieces
    //outbitboard(MyBoard.getPieces()); //all pieces
    return 0;
} // /*/


#include <iostream>
#include "UCI.hpp"
#include "Search.hpp"

using namespace std;

char gets(u64 x)
{
    if (x >= 10) {
        return 'a' + (x - 10);
    }
    return '0' + x;
}

void outbitboard(u64 n)
{
    cout << "Board : " << endl;
    string ret = "";
    while (n > 0) {
        ret = gets(n % 2) + ret;
        n = n/2;
    }
    while (ret.length() < 64) {
        ret = "0" + ret;
    }
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0 and i != 0) {
            cout << endl;
        }
        cout << ret[i];
    }
    cout << endl;
}

int main()
{
    Evaluation::initpopCountOfByte();
    while (true){
        UCI::waitForInput();
        if (UCI::quit == true){
            break;
        }
    }
    return 0;
} // */
