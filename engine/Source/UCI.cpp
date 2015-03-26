#include "UCI.hpp"
#include "Search.hpp"
#include "Evaluation.hpp"
#include "TranspositionTables.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

void outbitboard(u64 n);

bool UCI::quit = false;
Board UCI::currentBoard = Board();
int UCI::currentColor = 1;
bool UCI::killSearch = false;


bool UCI::waitForInput()
{
    string input;
    getline(cin, input);
    string command = input.substr(0, input.find(" "));
    if (command == "uci"){
        identification();
    } else if (command == "isready"){
        cout << "readyok" << endl;
    } else if (command == "quit"){
        quit = true;
    } else if (command == "position"){
        killSearch = true;
        sentPosition(input);
    } else if (command == "ucinewgame"){
        killSearch = true;
        currentBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        currentColor = 1;
    } else if (command == "go"){
        killSearch = true;
        std::thread t1(&startCalculating, input);
        std::thread t2(waitForInput);
        t1.join();
        t2.join();
    } else if (command == "stop"){
        killSearch = true;
    } else if (command == "ponderhit"){
        //used for pondering, not being implemented this sprint
    } else if (command == "debug"){
        //used for possible implementation of debug mode
    } else if (command == "setoption"){
        //used to change options associated with the engine
    } else if (command == "register"){
        //used for engine registration
    } else {
        //command not recognised
        return false;
    }
    return true;
}

void UCI::outputBestMove(string moveString)
{
    cout << "bestmove " << moveString << endl;
}

void UCI::identification()
{
    cout << "id name Saruman" << endl;
    cout << "id author Terry Bolt, Darragh Griffin, Conor Griffin" << endl;
    cout << "uciok" << endl;
}

bool UCI::sentPosition(string input)
{
    stringstream ss(input);
    string command;
    string fen;
    string moves;
    string moveIn;
    getline(ss, command, ' ');
    getline(ss, fen, ' ');
    if (fen != "startpos"){
        fen = "";
        getline(ss, fen, ' ');
        for (int i = 0; i < 5; i++){
            string tempFen;
            getline(ss, tempFen, ' ');
            if ((tempFen == "moves") or (tempFen == "")){
                //fen not valid
                return 0;
            }
            if (tempFen == "w"){
                currentColor = 1;
            } else if (tempFen == "b"){
                currentColor = -1;
            }
            fen = fen + " " + tempFen;
        }
        currentBoard = Board(fen);
    } else {
        currentBoard = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        currentColor = 1;
    }
    getline(ss, moves, ' ');

    if (moves == "moves"){
        while (getline(ss, moveIn, ' ')){
            pair<int, int> startPosition = make_pair(moveIn[0] - 'a', moveIn[1] - '1');
            pair<int, int> endPosition = make_pair(moveIn[2] - 'a', moveIn[3] - '1');
            char promote = ' ';
            if (moveIn.length() == 5){
                promote = moveIn[4];
            }
            Board lastBoard = currentBoard;
            bool moveMade = currentBoard.simpleMakeMove(startPosition, endPosition, promote);
            //outbitboard(currentBoard.getPieces());
            if (moveMade == false){ //move not valid
                return false;
            }
            currentColor = currentColor*-1;
        }
    } else {
        return false;
    }
    return true;
}

bool UCI::startCalculating(string input)
{
    vector<string> searchMoves; //a restricted list of moves to search
    bool ponder = false;        //whether to ponder on the current position or not
    int wtime = -1;             //time on whites clock in mseconds
    int btime = -1;             //time on blacks clock in mseconds
    int winc = -1;              //whites increment per move in mseconds
    int binc = -1;              //blacks increment per move in mseconds
    int movestogo = -1;         //number of moves left until the next time control
    int depth = 10;              //search only to a certain depth
    int nodes = -1;             //number of nodes to search
    int mate = -1;              //search for a mate in x moves
    int movetime = -1;          //time allowed for the move in mseconds
    bool infinite = false;      //search untill given the stop command
    string token;
    stringstream ss(input);
    while (getline(ss, token, ' ')){
        if (token == "searchmoves"){
            while (getline(ss, token, ' ')){
                searchMoves.push_back(token);
            }
        } else if (token == "ponder"){
            ponder = true;
        } else if (token == "infinite"){
            infinite = true;
        } else if (token == "wtime"){
            getline(ss, token, ' ');
            istringstream iss(token);
            iss >> wtime;
        } else if (token == "btime"){
            getline(ss, token, ' ');
            istringstream iss(token);
            iss >> btime;
        } else if (token == "winc"){
            getline(ss, token, ' ');
            istringstream iss(token);
            iss >> winc;
        } else if (token == "binc"){
            getline(ss, token, ' ');
            istringstream iss(token);
            iss >> binc;
        } else if (token == "movestogo"){
            getline(ss, token, ' ');
            istringstream iss(token);
            iss >> movestogo;
        } else if (token == "depth"){
            getline(ss, token, ' ');
            istringstream iss(token);
            iss >> depth;
        } else if (token == "nodes"){
            getline(ss, token, ' ');
            istringstream iss(token);
            iss >> nodes;
        } else if (token == "mate"){
            getline(ss, token, ' ');
            istringstream iss(token);
            iss >> mate;
        } else if (token == "movetime"){
            getline(ss, token, ' ');
            istringstream iss(token);
            iss >> movetime;
        }
    }

    //Set-up before search
    currentBoard.setEvaluation(Evaluation::evaluateBoard(currentBoard));
    currentBoard.stageOfGame = Evaluation::stageOfGame(currentBoard);
    currentBoard.setZorHash(TranspositionTables::getBoardHash(currentBoard, ((currentColor == 1) ? WHITE_CODE : BLACK_CODE)));

    string bestMove;
    int curDepth = min(depth, 2);
    killSearch = false;
    Search::totalNodes = 0;
    chrono::time_point<chrono::system_clock> timeBeforeSearch, timeAfterSearch;
    timeBeforeSearch = chrono::system_clock::now();
    int lastSearched = 0;

    while ((curDepth <= depth) or (infinite)) {
        if (killSearch == true) {
            break;
        }

        timeAfterSearch = chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = timeAfterSearch-timeBeforeSearch;

        Search::nodes = 0;
        pair<string, int> searchResult = Search::RootAlphaBeta(currentBoard, currentColor, curDepth, searchMoves);
        Search::totalNodes = Search::totalNodes + Search::nodes;

        if (searchResult.first != "") {
            int nodesPerSecond = int(double(Search::totalNodes )/elapsed_seconds.count());
            string info = string("depth ") + to_string(curDepth);
            info += " nodes " + to_string(Search::totalNodes);
            info += " nps " + to_string(nodesPerSecond);
            cout << "Branching factor : " << double(Search::nodes)/double(lastSearched) << std::endl;

            if (searchResult.second > MATE_SCORE) {
                searchResult.second -= MATE_SCORE;
                int plyCount = (curDepth - searchResult.second);
                info += " pv " + TranspositionTables::getPrincipalVariation(currentBoard, plyCount);
                info += " score mate " + to_string(plyCount - (plyCount/2));
            } else if (searchResult.second < -MATE_SCORE) {
                searchResult.second += MATE_SCORE;
                int plyCount = (curDepth + searchResult.second);
                info += " pv " + TranspositionTables::getPrincipalVariation(currentBoard, plyCount);
                info += " score mate " + to_string(-1*((plyCount/2) + 1));
            } else {
                info += " pv " + TranspositionTables::getPrincipalVariation(currentBoard, curDepth);
                info += " score cp " + to_string(searchResult.second);
            }
            sendInfo(info);
            bestMove = searchResult.first;
        }
        lastSearched = Search::nodes;
        if ((elapsed_seconds.count() > movetime) and (movetime > 0)) {
            break;
        }

        if ((Search::totalNodes  > nodes) and (nodes > 0)) {
            break;
        }

        curDepth++;
    }

    if (quit == false) {
        TranspositionTables::setOld();
        outputBestMove(bestMove);
    }
    return true;
}

void UCI::sendInfo(string info)
{
    cout << "info " << info << endl;
}

