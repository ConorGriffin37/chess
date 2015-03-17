#include "Evaluation.hpp"
#include "Board.hpp"

//pawn, rook, knight, bishop, queen
int scores[5] = {100, 500, 310, 320, 900};

int positionalScores[2][5][64] =  {
                                {{ //white pawn positional table
                                    0,  0,  0,  0,  0,  0,  0,  0,
                                    5, 10, 10,-20,-20, 10, 10,  5,
                                    5, -5,-10,  0,  0,-10, -5,  5,
                                    0,  0,  0, 20, 20,  0,  0,  0,
                                    5,  5, 10, 25, 25, 10,  5,  5,
                                    10, 10, 20, 30, 30, 20, 10, 10,
                                    50, 50, 50, 50, 50, 50, 50, 50,
                                    0,  0,  0,  0,  0,  0,  0,  0
                                },
                                { //white rook positional table
                                    0,  0,  0,  5,  5,  0,  0,  0,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                    5, 10, 10, 10, 10, 10, 10,  5,
                                    0,  0,  0,  0,  0,  0,  0,  0,
                                },
                                { //white knight positional table
                                    -50,-40,-30,-30,-30,-30,-40,-50,
                                    -40,-20,  0,  5,  5,  0,-20,-40,
                                    -30,  5, 10, 15, 15, 10,  5,-30,
                                    -30,  0, 15, 20, 20, 15,  0,-30,
                                    -30,  5, 15, 20, 20, 15,  5,-30,
                                    -30,  0, 10, 15, 15, 10,  0,-30,
                                    -40,-20,  0,  0,  0,  0,-20,-40,
                                    -50,-40,-30,-30,-30,-30,-40,-50,
                                },
                                { //white bishop positional table
                                    -20,-10,-10,-10,-10,-10,-10,-20,
                                    -10,  5,  0,  0,  0,  0,  5,-10,
                                    -10, 10, 10, 10, 10, 10, 10,-10,
                                    -10,  0, 10, 10, 10, 10,  0,-10,
                                    -10,  5,  5, 10, 10,  5,  5,-10,
                                    -10,  0,  5, 10, 10,  5,  0,-10,
                                    -10,  0,  0,  0,  0,  0,  0,-10,
                                    -20,-10,-10,-10,-10,-10,-10,-20,
                                },
                                { //white queen positional table
                                    -20,-10,-10, -5, -5,-10,-10,-20,
                                    -10,  0,  5,  0,  0,  0,  0,-10,
                                    -10,  5,  5,  5,  5,  5,  0,-10,
                                     0,  0,  5,  5,  5,  5,  0, -5,
                                    -5,  0,  5,  5,  5,  5,  0, -5,
                                    -10,  0,  5,  5,  5,  5,  0,-10,
                                    -10,  0,  0,  0,  0,  0,  0,-10,
                                    -20,-10,-10, -5, -5,-10,-10,-20,
                                }},
                                {{ //black pawn positional table
                                    0,  0,  0,  0,  0,  0,  0,  0,
                                    50, 50, 50, 50, 50, 50, 50, 50,
                                    10, 10, 20, 30, 30, 20, 10, 10,
                                     5,  5, 10, 25, 25, 10,  5,  5,
                                     0,  0,  0, 20, 20,  0,  0,  0,
                                     5, -5,-10,  0,  0,-10, -5,  5,
                                     5, 10, 10,-20,-20, 10, 10,  5,
                                     0,  0,  0,  0,  0,  0,  0,  0
                                },
                                { //black rook positional table
                                    0,  0,  0,  0,  0,  0,  0,  0,
                                    5, 10, 10, 10, 10, 10, 10,  5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                   -5,  0,  0,  0,  0,  0,  0, -5,
                                    0,  0,  0,  5,  5,  0,  0,  0,
                                },
                                { //black knight positional table
                                    -50,-40,-30,-30,-30,-30,-40,-50,
                                    -40,-20,  0,  0,  0,  0,-20,-40,
                                    -30,  0, 10, 15, 15, 10,  0,-30,
                                    -30,  5, 15, 20, 20, 15,  5,-30,
                                    -30,  0, 15, 20, 20, 15,  0,-30,
                                    -30,  5, 10, 15, 15, 10,  5,-30,
                                    -40,-20,  0,  5,  5,  0,-20,-40,
                                    -50,-40,-30,-30,-30,-30,-40,-50,
                                },
                                { //black bishop positional table
                                    -20,-10,-10,-10,-10,-10,-10,-20,
                                    -10,  0,  0,  0,  0,  0,  0,-10,
                                    -10,  0,  5, 10, 10,  5,  0,-10,
                                    -10,  5,  5, 10, 10,  5,  5,-10,
                                    -10,  0, 10, 10, 10, 10,  0,-10,
                                    -10, 10, 10, 10, 10, 10, 10,-10,
                                    -10,  5,  0,  0,  0,  0,  5,-10,
                                    -20,-10,-10,-10,-10,-10,-10,-20,
                                },
                                { //black queen positional table
                                    -20,-10,-10, -5, -5,-10,-10,-20,
                                    -10,  0,  0,  0,  0,  0,  0,-10,
                                    -10,  0,  5,  5,  5,  5,  0,-10,
                                     -5,  0,  5,  5,  5,  5,  0, -5,
                                      0,  0,  5,  5,  5,  5,  0, -5,
                                    -10,  5,  5,  5,  5,  5,  0,-10,
                                    -10,  0,  5,  0,  0,  0,  0,-10,
                                    -20,-10,-10, -5, -5,-10,-10,-20,
                                }}};

int kingPositionalScores[2][2][64] = {  //White King
                                        {{//White king middlegame
                                        -30,-40,-40,-50,-50,-40,-40,-30,
                                        -30,-40,-40,-50,-50,-40,-40,-30,
                                        -30,-40,-40,-50,-50,-40,-40,-30,
                                        -30,-40,-40,-50,-50,-40,-40,-30,
                                        -20,-30,-30,-40,-40,-30,-30,-20,
                                        -10,-20,-20,-20,-20,-20,-20,-10,
                                         20, 20,  0,  0,  0,  0, 20, 20,
                                         20, 30, 10,  0,  0, 10, 30, 20
                                        },
                                        { //White king endgame
                                        -50,-40,-30,-20,-20,-30,-40,-50,
                                        -30,-20,-10,  0,  0,-10,-20,-30,
                                        -30,-10, 20, 30, 30, 20,-10,-30,
                                        -30,-10, 30, 40, 40, 30,-10,-30,
                                        -30,-10, 30, 40, 40, 30,-10,-30,
                                        -30,-10, 20, 30, 30, 20,-10,-30,
                                        -30,-30,  0,  0,  0,  0,-30,-30,
                                        -50,-30,-30,-30,-30,-30,-30,-50
                                        }},
                                        //Black King
                                        {{//Black king middlegame
                                         20, 30, 10,  0,  0, 10, 30, 20,
                                         20, 20,  0,  0,  0,  0, 20, 20,
                                        -10,-20,-20,-20,-20,-20,-20,-10,
                                        -20,-30,-30,-40,-40,-30,-30,-20,
                                        -30,-40,-40,-50,-50,-40,-40,-30,
                                        -30,-40,-40,-50,-50,-40,-40,-30,
                                        -30,-40,-40,-50,-50,-40,-40,-30,
                                        -30,-40,-40,-50,-50,-40,-40,-30
                                        },
                                        { //Black king endgame
                                        -50,-30,-30,-30,-30,-30,-30,-50,
                                        -30,-30,  0,  0,  0,  0,-30,-30,
                                        -30,-10, 20, 30, 30, 20,-10,-30,
                                        -30,-10, 30, 40, 40, 30,-10,-30,
                                        -30,-10, 30, 40, 40, 30,-10,-30,
                                        -30,-10, 20, 30, 30, 20,-10,-30,
                                        -30,-20,-10,  0,  0,-10,-20,-30,
                                        -50,-40,-30,-20,-20,-30,-40,-50
                                        }}};

unsigned char popCountOfByte[256];

void Evaluation::initpopCountOfByte()
{
   popCountOfByte[0] = 0;
   for (int i = 1; i < 256; i++){
      popCountOfByte[i] = popCountOfByte[i / 2] + (i & 1);
   }
}

int Evaluation::popCount(u64 x) {
   unsigned char * p = (unsigned char *) & x;
   return popCountOfByte[p[0]] +
          popCountOfByte[p[1]] +
          popCountOfByte[p[2]] +
          popCountOfByte[p[3]] +
          popCountOfByte[p[4]] +
          popCountOfByte[p[5]] +
          popCountOfByte[p[6]] +
          popCountOfByte[p[7]];
}

u64 RemoveRanks[8] = {  18374403900871474942,  //1111111011111110111111101111111011111110111111101111111011111110
                        18302063728033398269,  //1111110111111101111111011111110111111101111111011111110111111101
                        18157383382357244923,  //1111101111111011111110111111101111111011111110111111101111111011
                        17868022691004938231,  //1111011111110111111101111111011111110111111101111111011111110111
                        17289301308300324847,  //1110111111101111111011111110111111101111111011111110111111101111
                        16131858542891098079,  //1101111111011111110111111101111111011111110111111101111111011111
                        13816973012072644543,  //1011111110111111101111111011111110111111101111111011111110111111
                        9187201950435737471 }; //0111111101111111011111110111111101111111011111110111111101111111

bool filesOpen[2][8] = { {false} };

int Evaluation::CheckForDoublePawns(int colorCode, Board& evalBoard)
{
    u64 pawns = evalBoard.getPieceAndColor(PAWN_CODE, colorCode);
    int numPawns = popCount(pawns);
    int doublePawns = 0;
    for (int i = 0; i < 8; i++){
        filesOpen[colorCode - WHITE_CODE][i] = false;
        pawns = pawns & RemoveRanks[i];
        int change = numPawns - popCount(pawns);
        if (change > 1){
            doublePawns++;
        } else if (change == 0){
            filesOpen[colorCode - WHITE_CODE][i] = true;
        }
        numPawns = numPawns - change;
    }
    return doublePawns;
}

int Evaluation::rooksOnOpenFile(int colorCode, Board& evalBoard)
{
    u64 rooks = evalBoard.getPieceAndColor(ROOK_CODE, colorCode);
    int numRooks = popCount(rooks);
    int rooksOpen = 0;
    if (numRooks > 0){
        for (int i = 0; i < 8; i++){
            rooks &= RemoveRanks[i];
            int change = numRooks - popCount(rooks);
            if ((change > 0) and (filesOpen[colorCode - WHITE_CODE][i] == true)){
                rooksOpen++;
            }
            numRooks = numRooks - change;
        }
    }
    return rooksOpen;
}

int Evaluation::GetMobilityScore(Board& evalBoard)
{
    int whiteDoublePawns = CheckForDoublePawns(WHITE_CODE, evalBoard);
    int blackDoublePawns = CheckForDoublePawns(BLACK_CODE, evalBoard);
    int whiteRooksOpen = rooksOnOpenFile(WHITE_CODE, evalBoard);
    int blackRooksOpen = rooksOnOpenFile(BLACK_CODE, evalBoard);
    return (-15*(whiteDoublePawns-blackDoublePawns) + 40*(whiteRooksOpen-blackRooksOpen));
}



int Evaluation::evaluateBoard(Board& boardToEvaluate)
{
    int whitescore = 0;
    int blackscore = 0;
    u64 bittest = 1;
    u64 colorboard = boardToEvaluate.getPieceColor(WHITE_CODE);
    u64 piecesBoards[6] = { boardToEvaluate.getPiece(PAWN_CODE),
                            boardToEvaluate.getPiece(ROOK_CODE),
                            boardToEvaluate.getPiece(KNIGHT_CODE),
                            boardToEvaluate.getPiece(BISHOP_CODE),
                            boardToEvaluate.getPiece(QUEEN_CODE),
                            boardToEvaluate.getPiece(KING_CODE) };

    for (int i = 0; i < 64; i++) {
        for (int x = PAWN_CODE; x < KING_CODE; x++) {
            if (piecesBoards[x] & bittest) {
                if (colorboard & bittest) {
                    whitescore = whitescore + scores[x];
                    whitescore = whitescore + positionalScores[0][x][i];
                    break;
                } else {
                    blackscore = blackscore + scores[x];
                    blackscore = blackscore + positionalScores[1][x][i];
                    break;
                }
            }
        }
        if (piecesBoards[KING_CODE] & bittest) {
            if (colorboard & bittest) {
                whitescore = whitescore + kingPositionalScores[0][stageOfGame(boardToEvaluate)][i];
            } else {
                blackscore = blackscore + kingPositionalScores[1][stageOfGame(boardToEvaluate)][i];
            }
        }
        bittest <<= 1;
    }
    return (whitescore - blackscore);
}

int Evaluation::numMinorPieces(int colorCode, Board& evalBoard)
{
    int num = 0;
    for (int i = ROOK_CODE; i < QUEEN_CODE; i++){
        num += popCount(evalBoard.getPieceAndColor(i, colorCode));
    }
    return num;
}

int Evaluation::stageOfGame(Board& evalBoard)
{
    int numWhiteQueens = popCount(evalBoard.getPieceAndColor(QUEEN_CODE, WHITE_CODE));
    int numBlackQueens = popCount(evalBoard.getPieceAndColor(QUEEN_CODE, BLACK_CODE));
    if ((!numWhiteQueens) and (!numBlackQueens)){
        return 1;
    } else if ((numWhiteQueens) and (!numBlackQueens)){
        if (numMinorPieces(WHITE_CODE, evalBoard) <= 1){
            return 1;
        }
    } else if ((!numWhiteQueens) and (numBlackQueens)){
        if (numMinorPieces(BLACK_CODE, evalBoard) <= 1){
            return 1;
        }
    } else {
        if ((numMinorPieces(WHITE_CODE, evalBoard) <= 1) and (numMinorPieces(BLACK_CODE, evalBoard) <= 1)){
            return 1;
        }
    }
    return 0;
}

int Evaluation::getPosScore(int code, int colorCode, int position, int gameStage)
{
    if (code == KING_CODE){
        return kingPositionalScores[colorCode - WHITE_CODE][gameStage][position];
    } else {
        return scores[code] + positionalScores[colorCode - WHITE_CODE][code][position];
    }
}
