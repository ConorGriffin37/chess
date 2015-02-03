using NUnit.Framework;
using System;
using GUI;

namespace Test
{
	[TestFixture ()]
	public class PieceTest
	{
		[Test ()]
		public void BasicPieceTest ()
		{
			Piece queen = new Piece (PieceColour.White, PieceType.Queen);
			Piece newQueen = new Piece (queen);
			Piece blackKnight = new Piece (PieceColour.Black, PieceType.Knight);

			Assert.AreEqual (queen, newQueen);
			Assert.AreEqual (queen.GetHashCode(), newQueen.GetHashCode());
			Assert.AreEqual ("q", queen.ToString());
			Assert.AreEqual ("N", blackKnight.ToString ());
		}
	}
}

