/**
 * @file PieceMoves.cs
 * 
 * One half of the move validation process. Generates a collection of every
 * possible move for every piece at every square on program start. NOTE: There are no tests
 * for this file since the calculations it makes are pretty much impossible to test.
 */

using System;
using System.Collections.Generic;

namespace GUI
{
    /**
     * @brief Stores a list of possible moves for a single piece at a single square.
     */
    internal struct PieceMoveSet
    {
        internal readonly List<byte> Moves;

        internal PieceMoveSet(List<byte> moves)
        {
            Moves = moves;
        }
    }

    /**
     * @brief Stores various lists totalling a complete collection of every possible move for every possible piece at every possible square.
     * 
     * It stores one array of @c PieceMoveSet per direction per piece. For example, the bishops and rooks have four
     * @c PieceMoveSet arrays each, the knights have only one, and the queens have eight.
     */
    internal struct MoveArrays
    {
        internal static PieceMoveSet[] BishopMoves1;
        internal static PieceMoveSet[] BishopMoves2;
        internal static PieceMoveSet[] BishopMoves3;
        internal static PieceMoveSet[] BishopMoves4;

        internal static PieceMoveSet[] BlackPawnMoves;

        internal static PieceMoveSet[] WhitePawnMoves;

        internal static PieceMoveSet[] KnightMoves;

        internal static PieceMoveSet[] QueenMoves1;
        internal static PieceMoveSet[] QueenMoves2;
        internal static PieceMoveSet[] QueenMoves3;
        internal static PieceMoveSet[] QueenMoves4;
        internal static PieceMoveSet[] QueenMoves5;
        internal static PieceMoveSet[] QueenMoves6;
        internal static PieceMoveSet[] QueenMoves7;
        internal static PieceMoveSet[] QueenMoves8;

        internal static PieceMoveSet[] RookMoves1;
        internal static PieceMoveSet[] RookMoves2;
        internal static PieceMoveSet[] RookMoves3;
        internal static PieceMoveSet[] RookMoves4;

        internal static PieceMoveSet[] KingMoves;
    }

    /**
     * @class PieceMoves
     * @brief Stores all the possible moves for every piece at every square.
     * 
     * This is a large class which is constructed when the GUI loads so as to improve performance
     * when a game is in progress. It stores all the possible moves for every piece at every square,
     * and can also store checks, en passant captures, and castling possibilities.
     */
    public static class PieceMoves
    {
        /**
         * @brief Converts a column and row into a byte representing a position on the chess board.
         */
        private static byte Position(byte col, byte row)
        {
            return (byte)(col + (row * 8));
        }

        /**
         * @brief Populates all of the @c MoveArrays.
         */
        public static void InitiateChessPieceMoves()
        {
            MoveArrays.WhitePawnMoves = new PieceMoveSet[64];

            MoveArrays.BlackPawnMoves = new PieceMoveSet[64];

            MoveArrays.KnightMoves = new PieceMoveSet[64];

            MoveArrays.BishopMoves1 = new PieceMoveSet[64];
            MoveArrays.BishopMoves2 = new PieceMoveSet[64];
            MoveArrays.BishopMoves3 = new PieceMoveSet[64];
            MoveArrays.BishopMoves4 = new PieceMoveSet[64];

            MoveArrays.QueenMoves1 = new PieceMoveSet[64];
            MoveArrays.QueenMoves2 = new PieceMoveSet[64];
            MoveArrays.QueenMoves3 = new PieceMoveSet[64];
            MoveArrays.QueenMoves4 = new PieceMoveSet[64];
            MoveArrays.QueenMoves5 = new PieceMoveSet[64];
            MoveArrays.QueenMoves6 = new PieceMoveSet[64];
            MoveArrays.QueenMoves7 = new PieceMoveSet[64];
            MoveArrays.QueenMoves8 = new PieceMoveSet[64];

            MoveArrays.RookMoves1 = new PieceMoveSet[64];
            MoveArrays.RookMoves2 = new PieceMoveSet[64];
            MoveArrays.RookMoves3 = new PieceMoveSet[64];
            MoveArrays.RookMoves4 = new PieceMoveSet[64];

            MoveArrays.KingMoves = new PieceMoveSet[64];

            SetMovesWhitePawn ();
            SetMovesBlackPawn ();
            SetMovesKnight ();
            SetMovesBishop ();
            SetMovesQueen ();
            SetMovesRook ();
            SetMovesKing ();
        }

        private static void SetMovesBlackPawn()
        {
            for (byte index = 8; index <= 55; index++) {
                PieceMoveSet moveset = new PieceMoveSet (new List<byte> ());

                byte x = (byte)(index % 8);
                byte y = (byte)(index / 8);

                // Diagonal: can move if capturing
                if (x < 7 && y < 7)
                    moveset.Moves.Add ((byte)(index + 8 + 1));
                if (x > 0 && y < 7)
                    moveset.Moves.Add ((byte)(index + 8 - 1));

                // Move one space forward
                moveset.Moves.Add ((byte)(index + 8));

                // If the pawn is in its starting position, it can jump 2
                if (y == 1)
                    moveset.Moves.Add ((byte)(index + 16));

                MoveArrays.BlackPawnMoves [index] = moveset;
            }
        }

        private static void SetMovesWhitePawn()
        {
            for (byte index = 8; index <= 55; index++) {
                PieceMoveSet moveset = new PieceMoveSet (new List<byte> ());

                byte x = (byte)(index % 8);
                byte y = (byte)(index / 8);

                // Diagonal: can move if capturing
                if (x < 7 && y < 0)
                    moveset.Moves.Add ((byte)(index - 8 + 1));
                if (x > 0 && y < 0)
                    moveset.Moves.Add ((byte)(index - 8 - 1));

                // Move one space forward
                moveset.Moves.Add ((byte)(index - 8));

                // If the pawn is in its starting position, it can jump 2
                if (y == 6)
                    moveset.Moves.Add ((byte)(index - 16));

                MoveArrays.WhitePawnMoves [index] = moveset;
            }
        }

        private static void SetMovesKnight()
        {
            for (byte y = 0; y < 8; y++) {
                for (byte x = 0; x < 8; x++) {
                    byte index = (byte)(y + (x * 8));
                    PieceMoveSet moveset = new PieceMoveSet (new List<byte> ());
                    byte move;

                    if (y < 6 && x > 0) {
                        move = Position ((byte)(y + 2), (byte)(x - 1));
                        if (move < 64)
                            moveset.Moves.Add (move);
                    }

                    if (y > 1 && x < 7) {
                        move = Position ((byte)(y - 2), (byte)(x + 1));
                        if (move < 64)
                            moveset.Moves.Add (move);
                    }

                    if (y > 0 && x < 6) {
                        move = Position ((byte)(y - 1), (byte)(x + 2));
                        if (move < 64)
                            moveset.Moves.Add (move);
                    }

                    if (y < 7 && x > 1) {
                        move = Position ((byte)(y + 1), (byte)(x - 2));
                        if (move < 64)
                            moveset.Moves.Add (move);
                    }

                    if (y > 0 && x > 1) {
                        move = Position ((byte)(y - 1), (byte)(x - 2));
                        if (move < 64)
                            moveset.Moves.Add (move);
                    }

                    if (y < 7 && x < 6) {
                        move = Position ((byte)(y + 1), (byte)(x + 2));
                        if (move < 64)
                            moveset.Moves.Add (move);
                    }

                    MoveArrays.KnightMoves [index] = moveset;
                }
            }
        }

        private static void SetMovesBishop()
        {
            for (byte y = 0; y < 8; y++) {
                for (byte x = 0; x < 8; x++) {
                    byte index = (byte)(y + (x * 8));
                    PieceMoveSet moveset = new PieceMoveSet (new List<byte> ());
                    byte move;
                    byte row = x;
                    byte col = y;

                    // Diagonally southeast
                    while (row < 7 && col < 7) {
                        row++;
                        col++;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.BishopMoves1 [index] = moveset;
                    moveset = new PieceMoveSet (new List<byte> ());

                    row = x;
                    col = y;

                    // Diagonally southwest
                    while (row < 7 && col > 0) {
                        row++;
                        col--;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.BishopMoves2 [index] = moveset;
                    moveset = new PieceMoveSet (new List<byte> ());

                    row = x;
                    col = y;

                    // Diagonally northeast
                    while (row > 0 && col < 7) {
                        row--;
                        col++;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.BishopMoves3 [index] = moveset;
                    moveset = new PieceMoveSet (new List<byte> ());

                    row = x;
                    col = y;

                    // Diagonally northwest
                    while (row > 0 && col > 0) {
                        row--;
                        col--;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.BishopMoves4 [index] = moveset;
                }
            }
        }

        private static void SetMovesQueen()
        {
            for (byte y = 0; y < 8; y++) {
                for (byte x = 0; x < 8; x++) {
                    byte index = (byte)(y + (x * 8));
                    PieceMoveSet moveset = new PieceMoveSet (new List<byte> ());
                    byte move;
                    byte row = x;
                    byte col = y;

                    // South
                    while (row < 7) {
                        row++;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.QueenMoves1 [index] = moveset;
                    moveset = new PieceMoveSet (new List<byte> ());
                    row = x;
                    col = y;

                    // North
                    while (row > 0) {
                        row--;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.QueenMoves2 [index] = moveset;
                    moveset = new PieceMoveSet (new List<byte> ());
                    row = x;
                    col = y;

                    // West
                    while (col > 0) {
                        col--;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.QueenMoves3 [index] = moveset;
                    moveset = new PieceMoveSet (new List<byte> ());
                    row = x;
                    col = y;

                    // East
                    while (col < 7) {
                        col++;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.QueenMoves4 [index] = moveset;
                    moveset = new PieceMoveSet (new List<byte> ());
                    row = x;
                    col = y;

                    // Southeast
                    while (row < 7 && col < 7) {
                        row++;
                        col++;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.QueenMoves5 [index] = moveset;
                    moveset = new PieceMoveSet (new List<byte> ());

                    row = x;
                    col = y;

                    // Southwest
                    while (row < 7 && col > 0) {
                        row++;
                        col--;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.QueenMoves6 [index] = moveset;
                    moveset = new PieceMoveSet (new List<byte> ());

                    row = x;
                    col = y;

                    // Northeast
                    while (row > 0 && col < 7) {
                        row--;
                        col++;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.QueenMoves7 [index] = moveset;
                    moveset = new PieceMoveSet (new List<byte> ());

                    row = x;
                    col = y;

                    // Northwest
                    while (row > 0 && col > 0) {
                        row--;
                        col--;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.QueenMoves8 [index] = moveset;
                }
            }
        }

        private static void SetMovesRook()
        {
            for (byte y = 0; y < 8; y++) {
                for (byte x = 0; x < 8; x++) {
                    byte index = (byte)(y + (x * 8));
                    PieceMoveSet moveset = new PieceMoveSet (new List<byte> ());
                    byte move;
                    byte row = x;
                    byte col = y;

                    // South
                    while (row < 7) {
                        row++;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.RookMoves1 [index] = moveset;
                    moveset = new PieceMoveSet (new List<byte> ());
                    row = x;
                    col = y;

                    // North
                    while (row > 0) {
                        row--;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.RookMoves2 [index] = moveset;
                    moveset = new PieceMoveSet (new List<byte> ());
                    row = x;
                    col = y;

                    // West
                    while (col > 0) {
                        col--;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.RookMoves3 [index] = moveset;
                    moveset = new PieceMoveSet (new List<byte> ());
                    row = x;
                    col = y;

                    // East
                    while (col < 7) {
                        col++;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.RookMoves4 [index] = moveset;
                    moveset = new PieceMoveSet (new List<byte> ());
                }
            }
        }

        private static void SetMovesKing()
        {
            for (byte y = 0; y < 8; y++) {
                for (byte x = 0; x < 8; x++) {
                    byte index = (byte)(y + (x * 8));
                    PieceMoveSet moveset = new PieceMoveSet (new List<byte> ());
                    byte move;
                    byte row = x;
                    byte col = y;

                    // South
                    if (row < 7) {
                        row++;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }
                        
                    row = x;
                    col = y;

                    // North
                    if (row > 0) {
                        row--;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }
                        
                    row = x;
                    col = y;

                    // West
                    if (col > 0) {
                        col--;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }
                        
                    row = x;
                    col = y;

                    // East
                    if (col < 7) {
                        col++;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }
                        
                    row = x;
                    col = y;

                    // Southeast
                    if (row < 7 && col < 7) {
                        row++;
                        col++;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    row = x;
                    col = y;

                    // Southwest
                    if (row < 7 && col > 0) {
                        row++;
                        col--;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    row = x;
                    col = y;

                    // Northeast
                    if (row > 0 && col < 7) {
                        row--;
                        col++;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    row = x;
                    col = y;

                    // Northwest
                    if (row > 0 && col > 0) {
                        row--;
                        col--;

                        move = Position (col, row);
                        moveset.Moves.Add (move);
                    }

                    MoveArrays.KingMoves [index] = moveset;
                }
            }
        }
    }
}

