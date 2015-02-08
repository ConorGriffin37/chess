/**
 * @file PieceValidMoves.cs
 * 
 * Uses the information generated in @c PieceMoves.cs to dynamically generate
 * a list of possible moves for a given piece in a particular board
 * configuration.
 * 
 * @see PieceMoves.cs
 */

using System;
using System.Collections.Generic;

namespace GUI
{
    public static class PieceValidMoves
    {
        internal static bool[] BlackAttackBoard;    /**< Board squares attacked by black at current board configuration. */
        internal static bool[] WhiteAttackBoard;    /**< Board squares attacked by white at current board configuration. */

        private static byte BlackKingPosition;  /**< Important for checking for check. */
        private static byte WhiteKingPosition;  /**< Important for checking for check. */

        /**
         * @fn AnalyseMove
         * @brief Looks at a move in detail and records the effects it has on the board.
         * 
         * @returns True if the move is not blocked. False if it is.
         */
        private static bool AnalyseMove(Board board, byte destination, Piece movingPiece)
        {
            // Pieces can move everywhere they can attack.
            if (movingPiece.Colour == PieceColour.White)
                WhiteAttackBoard [destination] = true;
            else
                BlackAttackBoard [destination] = true;

            // If destination is empty, add the move and return.
            if (board.Squares [destination].Piece == null) {
                movingPiece.ValidMoves.Add (destination);
                return true;
            }

            // Otherwise, start handling piece attacks.
            Piece attackedPiece = board.Squares [destination].Piece;

            if (attackedPiece.Colour != movingPiece.Colour) {
                attackedPiece.TimesAttacked++;

                // If attackedPiece is a king, set it in check
                if (attackedPiece.Type == PieceType.King) {
                    if (attackedPiece.Colour == PieceColour.White)
                        board.WhiteCheck = true;
                    else
                        board.BlackCheck = true;
                } else {
                    // Add as a valid move. Note that we do not add a valid move for the king's
                    // square above, as the king cannot be taken.
                    movingPiece.ValidMoves.Add (destination);
                }

                // Cannot continue moving past this piece.
                return false;
            }

            attackedPiece.TimesDefended++;
            return false;
        }

        /**
         * @fn CheckValidMovesPawn
         * @brief Checks available pawn moves to see which are valid.
         * 
         * Checks available pawn moves to see which are valid. If the pawn is making an attack,
         * hand over to @c CheckValidAttacksPawn, which will analyse attacks.
         * 
         * @see CheckValidAttacksPawn
         */
        private static void CheckValidMovesPawn(List<byte> moves, Piece movingPiece, byte source, Board board, int count)
        {
            for (byte i = 0; i < count; i++) {
                byte destination = moves [i];
                // If the source and destination are not in the same column
                // (i.e. if the pawn is attacking)
                if (destination % 8 != source % 8) {
                    CheckValidAttackPawn (board, destination, movingPiece);

                    if (movingPiece.Colour == PieceColour.White)
                        WhiteAttackBoard [destination] = true;
                    else
                        BlackAttackBoard [destination] = true;
                } else if (board.Squares [destination].Piece != null) {
                    // If there is something in front of the pawn
                    return;
                } else {
                    // If there is nothing in front of the pawn
                    movingPiece.ValidMoves.Add (destination);
                }
            }
        }

        private static void CheckValidAttackPawn(Board board, byte destination, Piece movingPiece)
        {
            // TODO: Add support for en passant.

            Piece attackedPiece = board.Squares [destination].Piece;

            // Check to make sure there is actually a piece at the destination
            if (attackedPiece == null)
                return;

            // Even if there is no piece there, the square is still under attack
            if (movingPiece.Colour == PieceColour.White) {
                WhiteAttackBoard [destination] = true;

                if (attackedPiece.Colour == movingPiece.Colour) {
                    attackedPiece.TimesDefended++;
                    return;
                } else
                    attackedPiece.TimesAttacked++;

                // If we are attacking a king set it in check
                if (attackedPiece.Type == PieceType.King)
                    board.BlackCheck = true;
                else
                    movingPiece.ValidMoves.Add (destination);
            } else {
                BlackAttackBoard [destination] = true;

                if (attackedPiece.Colour == movingPiece.Colour) {
                    attackedPiece.TimesDefended++;
                    return;
                } else
                    attackedPiece.TimesAttacked++;

                // If we are attacking a king set it in check
                if (attackedPiece.Type == PieceType.King)
                    board.WhiteCheck = true;
                else
                    movingPiece.ValidMoves.Add (destination);
            }
        }

        private static void CheckValidMovesKingCastle(Board board, Piece king)
        {
            if (king.HasMoved)
                return;
            if (king.Colour == PieceColour.White && (board.WhiteCastled || board.WhiteCheck))
                return;
            if (king.Colour == PieceColour.Black && (board.BlackCastled || board.BlackCheck))
                return;

            if (king.Colour == PieceColour.White) {
                Piece KingRook = board.Squares [63].Piece;
                if (KingRook != null) {
                    // Check that KingRook is in fact a rook and of the correct colour
                    if (KingRook.Colour == PieceColour.White && KingRook.Type == PieceType.Rook) {
                        // Check squares in between rook and king
                        if (board.Squares [62].Piece == null && board.Squares [61].Piece == null) {
                            if (BlackAttackBoard [62] == false && BlackAttackBoard [61] == false) {
                                // Finally, we can add the move
                                king.ValidMoves.Add (62);
                                WhiteAttackBoard [62] = true;
                            }
                        }
                    }
                }

                Piece QueenRook = board.Squares [56].Piece;
                if (QueenRook != null) {
                    // Check that QueenRook is in fact a rook and of the correct colour
                    if (QueenRook.Colour == PieceColour.White && QueenRook.Type == PieceType.Rook) {
                        // Check squares in between rook and king
                        if (board.Squares [57].Piece == null && board.Squares [58].Piece == null && board.Squares[59] == null) {
                            if (BlackAttackBoard [57] == false && BlackAttackBoard [58] == false && BlackAttackBoard[59] == false) {
                                // Finally, we can add the move
                                king.ValidMoves.Add (58);
                                WhiteAttackBoard [58] = true;
                            }
                        }
                    }
                }
            }

            if (king.Colour == PieceColour.Black) {
                Piece KingRook = board.Squares [7].Piece;
                if (KingRook != null) {
                    // Check that KingRook is in fact a rook and of the correct colour
                    if (KingRook.Colour == PieceColour.Black && KingRook.Type == PieceType.Rook) {
                        // Check squares in between rook and king
                        if (board.Squares [6].Piece == null && board.Squares [5].Piece == null) {
                            if (BlackAttackBoard [6] == false && BlackAttackBoard [5] == false) {
                                // Finally, we can add the move
                                king.ValidMoves.Add (6);
                                WhiteAttackBoard [6] = true;
                            }
                        }
                    }
                }

                Piece QueenRook = board.Squares [0].Piece;
                if (QueenRook != null) {
                    // Check that QueenRook is in fact a rook and of the correct colour
                    if (QueenRook.Colour == PieceColour.Black && QueenRook.Type == PieceType.Rook) {
                        // Check squares in between rook and king
                        if (board.Squares [1].Piece == null && board.Squares [2].Piece == null && board.Squares[3] == null) {
                            if (WhiteAttackBoard [1] == false && WhiteAttackBoard [2] == false && WhiteAttackBoard[3] == false) {
                                // Finally, we can add the move
                                king.ValidMoves.Add (2);
                                BlackAttackBoard [2] = true;
                            }
                        }
                    }
                }
            }
        }

        /**
         * @fn GenerateValidMoves
         * @brief Generates all valid moves for a specific board configuration.
         */
        public static void GenerateValidMoves(Board board)
        {
            // Reset board
            board.WhiteCheck = false;
            board.BlackCheck = false;

            WhiteAttackBoard = new bool[64];
            BlackAttackBoard = new bool[64];

            // Cycle through each square in the board and generate moves for each piece
            for (byte position = 0; position < 64; position++) {
                Square currentSquare = board.Squares [position];

                // We can't do anything with an empty square, so skip this iteration of the loop
                if (currentSquare.Piece == null)
                    continue;

                currentSquare.Piece.ValidMoves = new List<byte> ();

                switch (currentSquare.Piece.Type) {
                    case PieceType.Pawn:
                        if (currentSquare.Piece.Colour == PieceColour.White) {
                            CheckValidMovesPawn (MoveArrays.WhitePawnMoves [position].Moves, currentSquare.Piece,
                                position, board, MoveArrays.WhitePawnMoves [position].Moves.Count);
                        } else if (currentSquare.Piece.Colour == PieceColour.Black) {
                            CheckValidMovesPawn (MoveArrays.BlackPawnMoves [position].Moves, currentSquare.Piece,
                                position, board, MoveArrays.BlackPawnMoves [position].Moves.Count);
                        }
                        break;
                    
                    case PieceType.Knight:
                        foreach (byte move in MoveArrays.KnightMoves[position].Moves) {
                            AnalyseMove (board, move, currentSquare.Piece);
                        }
                        break;
                      
                    case PieceType.Bishop:
                        foreach (byte move in MoveArrays.BishopMoves1[position].Moves) {
                            // We test for a false return value because if it occurs we have
                            // encountered a blocking piece
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        foreach (byte move in MoveArrays.BishopMoves2[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        foreach (byte move in MoveArrays.BishopMoves3[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        foreach (byte move in MoveArrays.BishopMoves4[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        break;

                    case PieceType.Rook:
                        foreach (byte move in MoveArrays.RookMoves1[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        foreach (byte move in MoveArrays.RookMoves2[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        foreach (byte move in MoveArrays.RookMoves3[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        foreach (byte move in MoveArrays.RookMoves4[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        break;

                    case PieceType.Queen:
                        foreach (byte move in MoveArrays.QueenMoves1[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        foreach (byte move in MoveArrays.QueenMoves2[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        foreach (byte move in MoveArrays.QueenMoves3[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        foreach (byte move in MoveArrays.QueenMoves4[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        foreach (byte move in MoveArrays.QueenMoves5[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        foreach (byte move in MoveArrays.QueenMoves6[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        foreach (byte move in MoveArrays.QueenMoves7[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        foreach (byte move in MoveArrays.QueenMoves8[position].Moves) {
                            if (AnalyseMove (board, move, currentSquare.Piece) == false)
                                break;
                        }
                        break;

                    case PieceType.King:
                        if (currentSquare.Piece.Colour == PieceColour.White)
                            WhiteKingPosition = position;
                        else
                            BlackKingPosition = position;
                        break;
                }


            }

            if (board.PlayerToMove == PieceColour.White) {
                // We begin with the king which does not have the move since it does
                // not have to worry about check
                foreach (byte move in MoveArrays.KingMoves[BlackKingPosition].Moves) {
                    AnalyseMove (board, move, board.Squares [BlackKingPosition].Piece);
                }
                foreach (byte move in MoveArrays.KingMoves[WhiteKingPosition].Moves) {
                    AnalyseMove (board, move, board.Squares [WhiteKingPosition].Piece);
                }
            } else {
                foreach (byte move in MoveArrays.KingMoves[WhiteKingPosition].Moves) {
                    AnalyseMove (board, move, board.Squares [WhiteKingPosition].Piece);
                }
                foreach (byte move in MoveArrays.KingMoves[BlackKingPosition].Moves) {
                    AnalyseMove (board, move, board.Squares [BlackKingPosition].Piece);
                }
            }

            // Finally, we can check for castling now that we know about king check, attacked squares, etc.
            CheckValidMovesKingCastle (board, board.Squares [WhiteKingPosition].Piece);
            CheckValidMovesKingCastle (board, board.Squares [BlackKingPosition].Piece);
        }
    }
}

