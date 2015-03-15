/*#include <iostream>
#include <cmath>
#include <vector>
#include "board.hpp"
#include "MoveList.hpp"
#include "Evaluation.hpp"
#include "TranspositionTables.hpp"

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
    //Board MyBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    //Board TestBoard("r1bq3r/1p1k1ppp/p1np4/3Q4/8/5N2/PPP2PPP/2KR1B1R b - - 0 14");
    Board MyBoard("8/8/8/3pP3/8/8/8/8 b - d6 0 14"); //pawn promotion test
    Board LastBoard = MyBoard;
    //MyBoard.simpleMakeMove(make_pair(0, 6), make_pair(0, 7), 'q'); //simple make move test
    //return 0;
    //outbitboard(MyBoard.getPieces());
    TranspositionTables::initZobrist();
    return 0;
} // /*/


#include <iostream>
#include "UCI.hpp"
#include "Search.hpp"
#include "MoveList.hpp"
#include "TranspositionTables.hpp"

using namespace std;

void initMasks();
int bitScanForward(u64 bb);

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
    initMasks();
    UCI::currentBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Evaluation::initpopCountOfByte();
    TranspositionTables::initZobrist();
    //TranspositionTables::initEntryCount();
    while (true){
        UCI::waitForInput();
        if (UCI::quit == true){
            break;
        }
    }
    return 0;
}
