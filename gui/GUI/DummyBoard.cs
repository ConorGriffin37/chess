using System;

namespace GUI
{
    /**
     * @class DummyBoard
     * @brief Version of @c Board used for generating legal moves.
     * 
     * @c DummyBoard is identical to Board except it does not call
     * @c PieceLegalMoves.GenerateLegalMoves after each call to @c MakeMove
     * and @c UndoMove. It only generates pseudo-legal moves because it is
     * used to generate legal moves.
     * 
     * @see Board
     * @see PieceLegalMoves
     */
    public class DummyBoard : Board
    {
        public new Square[] Squares { get; private set; }
        public new bool BlackCheck { get; set; }
        public new bool WhiteCheck { get; set; }
        public new bool BlackCastled { get; set; }
        public new bool WhiteCastled { get; set; }
        public new PieceColour PlayerToMove { get; set; }

        public DummyBoard (Board other)
        {
            Squares = new Square[64];
            for (int i = 0; i < 64; i++) {
                Squares [i] = new Square ();
                if (other.Squares [i].Piece != null) {
                    Squares [i].Piece = new Piece (other.Squares [i].Piece);
                }
            }
            BlackCheck = other.BlackCheck;
            WhiteCheck = other.WhiteCheck;
            BlackCastled = other.BlackCastled;
            WhiteCastled = other.WhiteCastled;
            PlayerToMove = other.PlayerToMove;
        }

        public new void MakeMove(byte source, byte destination, PieceType? promoteTo = null)
        {
            Piece movingPiece = Squares [source].Piece;
            Squares [destination].Piece = movingPiece;
            Squares [source].Piece = null;
            if (PlayerToMove == PieceColour.White) {
                PlayerToMove = PieceColour.Black;
            } else {
                PlayerToMove = PieceColour.White;
            }
            PiecePseudoLegalMoves.GeneratePseudoLegalMoves (this);
        }

        public new void UndoMove(byte originalSource, byte originalDestination,
            PieceType? originalPromoteTo = null)
        {
            Piece movingPiece = Squares [originalDestination].Piece;
            Squares [originalSource].Piece = movingPiece;
            Squares [originalDestination].Piece = null;
            if (PlayerToMove == PieceColour.White) {
                PlayerToMove = PieceColour.Black;
            } else {
                PlayerToMove = PieceColour.White;
            }
            PiecePseudoLegalMoves.GeneratePseudoLegalMoves (this);
        }
    }
}

