﻿using NUnit.Framework;
using System;
using GUI;

namespace Test
{
    [TestFixture ()]
    public class PieceLegalMovesTest
    {
        [Test ()]
        public void StartPositionTest ()
        {
            Board testBoard = new Board ();
            PieceMoves.InitiateChessPieceMoves ();
            PiecePseudoLegalMoves.GeneratePseudoLegalMoves (testBoard);
            PieceLegalMoves.GenerateLegalMoves (testBoard);

            Assert.AreEqual (true, testBoard.IsMoveValid (8, 16));
            Assert.AreEqual (false, testBoard.IsMoveValid (0, 37));
        }
    }
}