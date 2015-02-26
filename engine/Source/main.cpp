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
}