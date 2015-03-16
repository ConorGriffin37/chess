#include <iostream>
#include "UCI.hpp"
#include "Search.hpp"
#include "MoveList.hpp"
#include "CaptureList.hpp"
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
    CaptureList caps = CaptureList(UCI::currentBoard, 6);
    for (int i = 0; i < caps.getMoveNumber(); i++) {
        cout << caps.getMoveCode(caps.getMovN(i)) << endl;
    }
    Evaluation::initpopCountOfByte();
    TranspositionTables::initZobrist();
    //TranspositionTables::initEntryCount();
    while (true){
        UCI::waitForInput();
        if (UCI::quit == true){
            break;
        }
    }
    //std::cout << TranspositionTables::getEntryCount() << std::endl;
    return 0;
}
