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
        public override bool BlackCastledR { get; set; }
        public override bool WhiteCastledR { get; set; }
        public override bool BlackCastledL { get; set; }
        public override bool WhiteCastledL { get; set; }
        public override PieceColour PlayerToMove { get; set; }
        public override byte EnPassantSquare { get; protected set; }
        public override PieceColour EnPassantColour { get; protected set; }

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
            BlackCastledR = other.BlackCastledR;
            WhiteCastledR = other.WhiteCastledR;
            BlackCastledL = other.BlackCastledR;
            WhiteCastledL = other.WhiteCastledR;
            PlayerToMove = other.PlayerToMove;
            EnPassantSquare = other.EnPassantSquare;
            EnPassantColour = other.EnPassantColour;
        }

        // Returns value of captured piece, for use in DummyBoard.UndoMove
        public new Piece MakeMove(byte source, byte destination, PieceType? promoteTo = null)
        {
            Piece movingPiece = Squares [source].Piece;
            Piece capturedPiece = null;

            // Special rules for castling
            if (movingPiece.Type == PieceType.King &&
                (source == 4 || source == 60) &&
                Array.IndexOf (castleDestinations, destination) != -1) {
                Square castleRookSquare = destination - source > 0 ?
                    Squares [destination + 1] : Squares [destination - 2];
                capturedPiece = castleRookSquare.Piece;
                Squares [destination].Piece = movingPiece;
                Squares [source].Piece = null;
                Squares [destination - source > 0 ?
                    destination - 1 :
                    destination + 1].Piece = castleRookSquare.Piece;
                castleRookSquare.Piece = null;
            } else {
                // Handle en passant.
                if (Squares [source].Piece.Type == PieceType.Pawn) {
                    if (Array.IndexOf (enPassantStartSquares, source) > -1 && Array.IndexOf (enPassantEndSquares, destination) > -1) {
                        EnPassantColour = Squares [source].Piece.Colour;
                        EnPassantSquare = EnPassantColour == PieceColour.White ? (byte)(destination + 8) : (byte)(destination - 8);
                    } else {
                        EnPassantSquare = 0;
                    }
                }

                capturedPiece = Squares [destination].Piece;
                switch (promoteTo) {
                    case PieceType.Bishop:
                        Squares [destination].Piece = new Piece (movingPiece.Colour, PieceType.Bishop);
                        Squares [source].Piece = null;
                        break;
                    case PieceType.Knight:
                        Squares [destination].Piece = new Piece (movingPiece.Colour, PieceType.Knight);
                        Squares [source].Piece = null;
                        break;
                    case PieceType.Rook:
                        Squares [destination].Piece = new Piece (movingPiece.Colour, PieceType.Rook);
                        Squares [source].Piece = null;
                        break;
                    case PieceType.Queen:
                        Squares [destination].Piece = new Piece (movingPiece.Colour, PieceType.Knight);
                        Squares [source].Piece = null;
                        break;
                    default:
                        // Handle en passant capture
                        if (movingPiece.Type == PieceType.Pawn && destination == EnPassantSquare && EnPassantSquare != 0) {
                            byte captureSquare = EnPassantColour == PieceColour.White ? (byte)(destination - 8) : (byte)(destination + 8);
                            Squares [destination].Piece = movingPiece;
                            capturedPiece = Squares [captureSquare].Piece;
                            Squares [captureSquare].Piece = null;
                            Squares [source].Piece = null;
                        } else {
                            Squares [destination].Piece = movingPiece;
                            Squares [source].Piece = null;
                        }
                        break;
                }
            }

            if (PlayerToMove == PieceColour.White) {
                PlayerToMove = PieceColour.Black;
            } else {
                PlayerToMove = PieceColour.White;
            }
            if (!(movingPiece.Type == PieceType.Pawn &&
                ((int)(destination / 8) == 0 ||
                (int)(destination / 8) == 7))) {
                PiecePseudoLegalMoves.GeneratePseudoLegalMoves (this);
            }
            return capturedPiece;
        }

        public void UndoMove(byte originalSource, byte originalDestination, Piece capturedPiece,
            PieceType? originalPromoteTo = null)
        {
            Piece movingPiece = Squares [originalDestination].Piece;

            // Special rules for castling
            if (movingPiece.Type == PieceType.King &&
                Array.IndexOf (castleDestinations, originalDestination) != -1 &&
                capturedPiece != null &&
                capturedPiece.Colour == movingPiece.Colour) {
                Square castleRookSquare = originalDestination - originalSource > 0 ?
                    Squares [originalDestination - 1] : Squares [originalDestination + 1];
                Squares [originalSource].Piece = movingPiece;
                Squares [originalDestination].Piece = null;
                Squares [originalDestination - originalSource > 0 ?
                    originalDestination + 1 :
                    originalDestination - 2].Piece = castleRookSquare.Piece;
                castleRookSquare.Piece = null;
            } else {
                if (originalPromoteTo != null) {
                    Squares [originalSource].Piece = new Piece (movingPiece.Colour, PieceType.Pawn);
                } else {
                    Squares [originalSource].Piece = movingPiece;
                }
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

