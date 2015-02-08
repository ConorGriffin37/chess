using NUnit.Framework;
using System;
using GUI;

namespace Test
{
    [TestFixture ()]
    public class PieceValidMovesTest
    {
        [Test ()]
        public void StartPositionTest ()
        {
            Board testBoard = new Board ();
            PieceMoves.InitiateChessPieceMoves ();
            PieceValidMoves.GenerateValidMoves (testBoard);

            Assert.AreEqual (true, testBoard.IsMoveValid (8, 16));
            Assert.AreEqual (false, testBoard.IsMoveValid (0, 37));
        }
    }
}