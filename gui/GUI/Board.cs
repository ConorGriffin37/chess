using System;
using System.Collections.Generic;

namespace GUI
{
    public enum PieceColour { White, Black };
    public enum PieceType { Pawn, Knight, Bishop, Rook, Queen, King };

    /**
     * @brief Representation of the board as a whole.
     */
    public class Board
    {
        public Square[] Squares { get; private set; }
        public bool BlackCheck { get; set; }
        public bool WhiteCheck { get; set; }
        public bool BlackCastled { get; set; }
        public bool WhiteCastled { get; set; }
        public PieceColour PlayerToMove { get; set; }

        /**
         * @brief Default constructor.
         * 
         * Creates a board laid out in the standard chess starting position.
         */
        public Board (bool empty = false)
        {
            BlackCheck = false;
            WhiteCheck = false;
            // For castling, default to true and change based on position
            BlackCastled = true;
            WhiteCastled = true;
            PlayerToMove = PieceColour.White;

            if (empty) {
                Squares = new Square[64];
                // Initialise empty squares
                for (int i = 0; i < 64; i++) {
                    Squares [i] = new Square ();
                }
            } else {
                BlackCastled = false;
                WhiteCastled = false;

                Squares = new Square[64];
                // Initialise empty squares
                for (int i = 0; i < 64; i++) {
                    Squares [i] = new Square ();
                }

                // Add pieces for black.
                Squares [0].Piece = new Piece (PieceColour.Black, PieceType.Rook);
                Squares [1].Piece = new Piece (PieceColour.Black, PieceType.Knight);
                Squares [2].Piece = new Piece (PieceColour.Black, PieceType.Bishop);
                Squares [3].Piece = new Piece (PieceColour.Black, PieceType.Queen);
                Squares [4].Piece = new Piece (PieceColour.Black, PieceType.King);
                Squares [5].Piece = new Piece (PieceColour.Black, PieceType.Bishop);
                Squares [6].Piece = new Piece (PieceColour.Black, PieceType.Knight);
                Squares [7].Piece = new Piece (PieceColour.Black, PieceType.Rook);
                for (int i = 8; i <= 15; i++) {
                    Squares [i].Piece = new Piece (PieceColour.Black, PieceType.Pawn);
                }

                // Add pieces for white.
                for (int i = 48; i <= 55; i++) {
                    Squares [i].Piece = new Piece (PieceColour.White, PieceType.Pawn);
                }
                Squares [56].Piece = new Piece (PieceColour.White, PieceType.Rook);
                Squares [57].Piece = new Piece (PieceColour.White, PieceType.Knight);
                Squares [58].Piece = new Piece (PieceColour.White, PieceType.Bishop);
                Squares [59].Piece = new Piece (PieceColour.White, PieceType.Queen);
                Squares [60].Piece = new Piece (PieceColour.White, PieceType.King);
                Squares [61].Piece = new Piece (PieceColour.White, PieceType.Bishop);
                Squares [62].Piece = new Piece (PieceColour.White, PieceType.Knight);
                Squares [63].Piece = new Piece (PieceColour.White, PieceType.Rook);
            }
        }

        /**
         * @brief Copy constructor.
         * 
         * Copies the list of pieces from one board to another.
         */
        public Board(Board other)
        {
            Squares = new Square[64];
            Array.Copy (other.Squares, Squares, 64);
        }

        public void AddPiece(PieceColour colour, PieceType type, int position)
        {
            if (position >= 64)
                throw new ArgumentOutOfRangeException ("position",
                    "Position index for the chessboard must be less than 64.");

            if (Squares [position].Piece != null)
                throw new ArgumentException ("That square is already occupied.", "position");
                    
            Squares[position].Piece = new Piece(colour, type);
        }

        public bool IsMoveValid(byte source, byte destination)
        {
            Piece movingPiece = Squares [source].Piece;

            if (movingPiece == null)
                return false;

            if (movingPiece.ValidMoves.Contains (destination))
                return true;

            return false;
        }

        public Piece PieceAt(int square)
        {
            return Squares [square].Piece;
        }

        public string ToFEN()
        {
            string fen = String.Empty;
            // Board configuration
            int blankCounter = 0;
            for (int i = 0; i < 64; i++) {
                Square sq = Squares[i];
                if (sq.Piece != null && blankCounter > 0) {
                    fen += blankCounter.ToString ();
                    blankCounter = 0;
                }
                if (sq.Piece == null) {
                    blankCounter++;
                } else {
                    switch (sq.Piece.Type) {
                        case PieceType.Pawn:
                            if (sq.Piece.Colour == PieceColour.Black)
                                fen += 'p';
                            else
                                fen += 'P';
                            break;
                        case PieceType.Rook:
                            if (sq.Piece.Colour == PieceColour.Black)
                                fen += 'r';
                            else
                                fen += 'R';
                            break;
                        case PieceType.Knight:
                            if (sq.Piece.Colour == PieceColour.Black)
                                fen += 'n';
                            else
                                fen += 'N';
                            break;
                        case PieceType.Bishop:
                            if (sq.Piece.Colour == PieceColour.Black)
                                fen += 'b';
                            else
                                fen += 'B';
                            break;
                        case PieceType.Queen:
                            if (sq.Piece.Colour == PieceColour.Black)
                                fen += 'q';
                            else
                                fen += 'Q';
                            break;
                        case PieceType.King:
                            if (sq.Piece.Colour == PieceColour.Black)
                                fen += 'k';
                            else
                                fen += 'K';
                            break;
                        default:
                            break;
                    }
                }

                // If at end of row (but not the last one), append '/'.
                // Also append the blank square count if not 0
                if (i != 63 && i % 8 == 7) {
                    if (blankCounter > 0) {
                        fen += blankCounter.ToString ();
                        blankCounter = 0;
                    }
                    fen += '/';
                }
            }
            fen += ' ';

            // Active colour
            if (PlayerToMove == PieceColour.White)
                fen += 'w';
            else
                fen += 'b';
            fen += ' ';

            // Castling availability
            if (!WhiteCastled)
                fen += "KQ";
            if (!BlackCastled)
                fen += "kq";
            if (WhiteCastled && BlackCastled)
                fen += '-';
            fen += ' ';

            // En passant target
            // NOT IMPLEMENTED
            fen += "- ";

            // Halfmove clock
            // NOT IMPLEMENTED
            fen += "0 ";

            // Fullmove count
            // NOT IMPLEMENTED
            fen += "1";

            return fen;
        }

        public override string ToString ()
        {
            string output = "";

            for (int i = 0; i < 64; i++) {
                output += (Squares [i].ToString () + " ");
                // If i is in column 7, start a new line
                if (i % 8 == 7)
                    output += Environment.NewLine;
            }

            return output;
        }

        public override bool Equals (object obj)
        {
            if (obj == null)
                return false;

            Board otherBoard = (Board)obj;
            if (otherBoard == null)
                return false;

            if (ToString () == otherBoard.ToString ())
                return true;
            else
                return false;
        }
    }

    /**
     * @brief Representation of a single piece.
     * 
     * @fn Note that it contains no default constructor as there is no "base" piece.
     */
    public class Piece
    {
        public PieceColour Colour { get; private set; }
        public PieceType Type { get; private set; }
        public List<byte> ValidMoves { get; set; }
        public byte TimesAttacked { get; set; }
        public byte TimesDefended { get; set; }
        public bool HasMoved { get; set; } /**< We need this to know if we can castle, for example. */

        /**
         * @brief Constructor for a Piece.
         * 
         * @param colour    the colour of the piece.
         * @param type      the type of the piece.
         */
        public Piece(PieceColour colour, PieceType type)
        {
            Colour = colour;
            Type = type;
        }

        /**
         * @brief Copy constructor for a piece.
         * 
         * @param other     the piece from which to copy.
         */
        public Piece(Piece other)
        {
            Colour = other.Colour;
            Type = other.Type;
        }

        /**
         * @brief Tests whether a piece is equal to another.
         * 
         * @fn Override of @c Object.Equals. First tests whether @c obj is null, then attempts to cast to
         * Piece. If it is still not null, it compares the @c PieceType and @c PieceColour properties.
         * 
         * @param obj   the object to compare to.
         */
        public override bool Equals (object obj)
        {
            if (obj == null)
                return false;

            Piece otherPiece = (Piece)obj;
            if (otherPiece == null)
                return false;

            if (Colour == otherPiece.Colour && Type == otherPiece.Type) {
                return true;
            } else {
                return false;
            }
        }

        /**
         * @brief Returns a string representation of the Piece object.
         * 
         * @return A string representation according to FEN notation (i.e., a black
         * queen is "Q" and a white bishop is "b").
         */
        public override string ToString ()
        {
            string output = "";

            switch (Type) {
                case PieceType.Pawn:
                    output = "p";
                    break;
                case PieceType.Knight:
                    output = "n";
                    break;
                case PieceType.Bishop:
                    output = "b";
                    break;
                case PieceType.Rook:
                    output = "r";
                    break;
                case PieceType.Queen:
                    output = "q";
                    break;
                case PieceType.King:
                    output = "k";
                    break;
            }

            if (Colour == PieceColour.White)
                output = output.ToUpper ();

            return output;
        }

        /**
         * @brief Generates a hash code which C# uses to compare objects in IEnumerables.
         * 
         * @return A hash of the integer representation of the piece colour added to the
         * integer representation of the piece type.
         */
        public override int GetHashCode ()
        {
            return (int)Colour + (int)Type;
        }
    }

    /**
     * @brief A representation of a single square.
     * 
     * @class A representation of a single square which can hold a Piece object or be empty.
     */
    public class Square
    {
        public Piece Piece { get; set; }

        /**
         * @brief Default constructor creates an empty square.
         */
        public Square()
        {
            Piece = null;
        }

        /**
         * @brief Constructor which initialises to hold a piece.
         * 
         * @param piece     the piece to hold in the square.
         */
        public Square(Piece piece)
        {
            Piece = piece;
        }

        /**
         * @brief Copy constructor.
         * 
         * @param other     a square to copy the piece from.
         */
        public Square(Square other)
        {
            Piece = other.Piece;
        }

        public override string ToString ()
        {
            if (Piece == null)
                return "_";
            else
                return Piece.ToString ();
        }

        public bool isEmpty()
        {
            return Piece == null;
        }
    }
}