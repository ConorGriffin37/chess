using System;
using System.Collections.Generic;

namespace GUI
{
    public static class PieceLegalMoves
    {
        public static void GenerateLegalMoves(Board board)
        {
            DummyBoard tempBoard = new DummyBoard (board);
            for (byte i = 0; i < 64; i++) {
                // currentPiece points to a Piece on board instead of
                // tempBoard because when we make a move on tempBoard
                // it changes the positions of the pieces and their
                // pseudo-legal moves.
                Piece currentPiece = board.Squares [i].Piece;
                if (currentPiece == null)
                    continue;
                currentPiece.LegalMoves = new List<byte> ();
                foreach (byte pseudoLegalMove in currentPiece.PseudoLegalMoves) {
                    // Make a move on the temp board
                    tempBoard.MakeMove (i, pseudoLegalMove);
                    // Check to see if own king is in check after that move.
                    // If it is simply undo the move. Else, add it to the list
                    // of legal moves for that piece and then undo.
                    if (currentPiece.Colour == PieceColour.White &&
                        tempBoard.WhiteCheck) {
                        tempBoard.UndoMove (i, pseudoLegalMove);
                    } else if (currentPiece.Colour == PieceColour.Black &&
                               tempBoard.BlackCheck) {
                        tempBoard.UndoMove (i, pseudoLegalMove);
                    } else {
                        currentPiece.LegalMoves.Add (pseudoLegalMove);
                        tempBoard.UndoMove (i, pseudoLegalMove);
                    }
                }
            }
        }
    }
}

