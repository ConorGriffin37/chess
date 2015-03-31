using System;

namespace GUI
{
    /**
     * @brief Class to parse a FEN file.
     * 
     * This class can parse a FEN file and store the information received for conversion
     * into Board objects and Game objects.
     */
    public class FENParser
    {
        private string piecePlacement;
        private char colourToMove;
        private string castlingPossibilities;
        private string enPassantTarget;         /**< If a pawn can be captured by en passant, this stores the target square. */
        private int halfMoveClock;              /**< Stores the number of half-moves since a capture or pawn advance. */
        private int fullMoveNumber;             /**< Stores total number of full-moves all game. */

        public FENParser (string fen)
        {
            try {
                ParseFEN(fen);
            } catch(ArgumentException) {
                throw;
            }
        }

        public void ParseFEN (string fen)
        {
            try {
                string[] fenTokens = fen.Split(' ');
                piecePlacement = fenTokens [0];
                colourToMove = Convert.ToChar (fenTokens [1]);
                castlingPossibilities = fenTokens [2];
                enPassantTarget = fenTokens [3];
                halfMoveClock = Int32.Parse (fenTokens [4]);
                fullMoveNumber = Int32.Parse (fenTokens [5]);
            } catch(IndexOutOfRangeException) {
                throw new ArgumentException("Bad FEN string passed to parser.", "fen");
            }
        }

        /**
         * @brief Creates a @c Board object according to the FEN's piece placement token.
         * 
         * Parses the FEN string's piece placement token and returns a board object
         * representing the placement of the pieces.
         * 
         * @return A @c Board object representing the pieces from the FEN string.
         */
        public Board GetBoard ()
        {
            int position = 0;
            Board output = new Board (true);

            switch (colourToMove) {
                case 'w':
                    output.PlayerToMove = PieceColour.White;
                    break;
                case 'b':
                    output.PlayerToMove = PieceColour.Black;
                    break;
                default:
                    throw new ArgumentException ("Bad FEN field: Colour to move.", "fen");
            }

            if (castlingPossibilities.Length == 4) {
                output.BlackCastledR = false;
                output.WhiteCastledR = false;
                output.BlackCastledL = false;
                output.WhiteCastledL = false;
            } else if (castlingPossibilities == "-") {
                output.BlackCastledR = true;
                output.WhiteCastledR = true;
                output.BlackCastledL = true;
                output.WhiteCastledL = true;
            } else if (castlingPossibilities.Length < 4) {
                foreach (char c in castlingPossibilities) {
                    if (c == 'q') {
                        output.BlackCastledL = false;
                    } else if (c == 'Q') {
                        output.WhiteCastledL = false;
                    } else if (c == 'K') {
                        output.WhiteCastledR = false;
                    } else if (c == 'k') {
                        output.BlackCastledR = false;
                    }
                }
            } else
                throw new ArgumentException ("Bad FEN field: Castling possibilities.", "fen");

            foreach (char c in piecePlacement) {
                if (Char.IsNumber (c)) {
                    int number = (int)Char.GetNumericValue (c);
                    for (int i = 0; i < number; i++) {
                        position++;
                    }
                } else {
                    try {
                        switch (c) {
                            // Black pieces
                            case 'p':
                                output.AddPiece (PieceColour.Black, PieceType.Pawn, position);
                                position++;
                                break;
                            case 'r':
                                output.AddPiece (PieceColour.Black, PieceType.Rook, position);
                                position++;
                                break;
                            case 'n':
                                output.AddPiece (PieceColour.Black, PieceType.Knight, position);
                                position++;
                                break;
                            case 'b':
                                output.AddPiece (PieceColour.Black, PieceType.Bishop, position);
                                position++;
                                break;
                            case 'q':
                                output.AddPiece (PieceColour.Black, PieceType.Queen, position);
                                position++;
                                break;
                            case 'k':
                                output.AddPiece (PieceColour.Black, PieceType.King, position);
                                position++;
                                break;
                            
                            // White pieces
                            case 'P':
                                output.AddPiece (PieceColour.White, PieceType.Pawn, position);
                                position++;
                                break;
                            case 'R':
                                output.AddPiece (PieceColour.White, PieceType.Rook, position);
                                position++;
                                break;
                            case 'N':
                                output.AddPiece (PieceColour.White, PieceType.Knight, position);
                                position++;
                                break;
                            case 'B':
                                output.AddPiece (PieceColour.White, PieceType.Bishop, position);
                                position++;
                                break;
                            case 'Q':
                                output.AddPiece (PieceColour.White, PieceType.Queen, position);
                                position++;
                                break;
                            case 'K':
                                output.AddPiece (PieceColour.White, PieceType.King, position);
                                position++;
                                break;
                            
                            // Line delimeter is '/'. These are ignored.
                            default:
                                break;
                        }
                    } catch(Exception) {
                        throw new ArgumentException("Bad FEN field: Piece placement.", "fen");
                    }
                }
            }

            if (position > 64)
                throw new ArgumentException ("FEN string passed to parser too long.", "fen");

            return output;
        }
    }
}

