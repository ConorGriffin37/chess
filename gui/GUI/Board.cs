using System;

namespace GUI
{
	public enum PieceColour { White, Black };
	public enum PieceType { Pawn, Knight, Bishop, Rook, Queen, King };

	/**
	 * @brief Representation of the board as a whole.
	 */
	public class Board
	{
		public Board ()
		{
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

		/**
		 * @brief Constructor for a Piece.
		 * 
		 * @param colour	the colour of the piece.
		 * @param type		the type of the piece.
		 */
		public Piece(PieceColour colour, PieceType type)
		{
			Colour = colour;
			Type = type;
		}

		/**
		 * @brief Copy constructor for a piece.
		 * 
		 * @param other		the piece from which to copy.
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
		 * @param obj	the object to compare to.
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

			if (Colour == PieceColour.Black)
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
		 * @param piece		the piece to hold in the square.
		 */
		public Square(Piece piece)
		{
			Piece = piece;
		}

		/**
		 * @brief Copy constructor.
		 * 
		 * @param other		a square to copy the piece from.
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

