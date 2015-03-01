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
        public override Square[] Squares { get; protected set; }
        public override bool BlackCheck { get; set; }
        public override bool WhiteCheck { get; set; }
        public override bool BlackCastled { get; set; }
        public override bool WhiteCastled { get; set; }
        public override PieceColour PlayerToMove { get; set; }

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

        // Returns value of captured piece, for use in DummyBoard.UndoMove
        public new Piece MakeMove(byte source, byte destination, PieceType? promoteTo = null)
        {
            Piece movingPiece = Squares [source].Piece;
            Piece capturedPiece = null;

            // Special rules for castling
            if (movingPiece.Type == PieceType.King &&
                Array.IndexOf (castleDestinations, destination) != -1) {
                Square castleRookSquare = destination - source > 0 ?
                    Squares [destination + 1] : Squares [destination - 2];
                Squares [destination].Piece = movingPiece;
                Squares [source].Piece = null;
                Squares [destination - source > 0 ?
                    destination - 1 :
                    destination + 1].Piece = castleRookSquare.Piece;
                castleRookSquare.Piece = null;
            } else {
                capturedPiece = Squares [destination].Piece;
                Squares [destination].Piece = movingPiece;
                Squares [source].Piece = null;
            }

            if (PlayerToMove == PieceColour.White) {
                PlayerToMove = PieceColour.Black;
            } else {
                PlayerToMove = PieceColour.White;
            }
            PiecePseudoLegalMoves.GeneratePseudoLegalMoves (this);
            return capturedPiece;
        }

        public void UndoMove(byte originalSource, byte originalDestination, Piece capturedPiece,
            PieceType? originalPromoteTo = null)
        {
            Piece movingPiece = Squares [originalDestination].Piece;

            // Special rules for castling
            if (movingPiece.Type == PieceType.King &&
                Array.IndexOf (castleDestinations, originalDestination) != -1) {
                Square castleRookSquare = originalDestination - originalSource > 0 ?
                    Squares [originalDestination - 1] : Squares [originalDestination + 1];
                Squares [originalSource].Piece = movingPiece;
                Squares [originalDestination].Piece = null;
                Squares [originalDestination - originalSource > 0 ?
                    originalDestination + 1 :
                    originalDestination - 2].Piece = castleRookSquare.Piece;
                castleRookSquare.Piece = null;
            } else {
                Squares [originalSource].Piece = movingPiece;
                Squares [originalDestination].Piece = capturedPiece;
            }

            if (PlayerToMove == PieceColour.White) {
                PlayerToMove = PieceColour.Black;
            } else {
                PlayerToMove = PieceColour.White;
            }
            PiecePseudoLegalMoves.GeneratePseudoLegalMoves (this);
        }
    }
}

