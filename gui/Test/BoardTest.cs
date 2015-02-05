using NUnit.Framework;
using System;
using GUI;

namespace Test
{
    [TestFixture()]
    public class BoardTest
    {
        [Test()]
        public void BoardCopyAndComparisonTest()
        {
            Board board = new Board ();
            Board otherBoard = new Board (board);

            Assert.AreEqual (board.ToString(), otherBoard.ToString());
        }
    }

    [TestFixture ()]
    public class PieceTest
    {
        [Test ()]
        public void PieceCopyAndComparisonTest ()
        {
            Piece queen = new Piece (PieceColour.White, PieceType.Queen);
            Piece newQueen = new Piece (queen);

            Assert.AreEqual (queen, newQueen);
            Assert.AreEqual (queen.GetHashCode(), newQueen.GetHashCode());
        }

        [Test()]
        public void PieceToStringTest()
        {
            Piece queen = new Piece (PieceColour.White, PieceType.Queen);
            Piece blackKnight = new Piece (PieceColour.Black, PieceType.Knight);

            Assert.AreEqual ("q", queen.ToString());
            Assert.AreEqual ("N", blackKnight.ToString ());
        }
    }

    [TestFixture()]
    public class SquareTest
    {
        [Test()]
        public void SquareCopyAndToStringTest()
        {
            Square empty = new Square ();
            Square containsBlackKnight = new Square (new Piece (PieceColour.Black, PieceType.Knight));
            Square alsoEmpty = new Square (empty);

            Assert.AreEqual ("_", empty.ToString ());
            Assert.AreEqual ("N", containsBlackKnight.ToString ());
            Assert.AreEqual ("_", alsoEmpty.ToString ());
        }
    }
}

