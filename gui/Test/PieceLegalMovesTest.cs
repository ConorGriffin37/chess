using NUnit.Framework;
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

            Assert.AreEqual (true, testBoard.IsMoveValid (52, 36));
            Assert.AreEqual (false, testBoard.IsMoveValid (12, 28));
        }

        [Test()]
        public void DummyBoardUndoCastlingTest()
        {
            FENParser fen = new FENParser ("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");
            DummyBoard testDummy = new DummyBoard (fen.GetBoard());
            PieceMoves.InitiateChessPieceMoves ();
            PiecePseudoLegalMoves.GeneratePseudoLegalMoves (testDummy);

            testDummy.MakeMove (4, 6);
            testDummy.UndoMove (4, 6);
            Assert.AreEqual ("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1", testDummy.ToFEN ());

            testDummy.MakeMove (4, 2);
            testDummy.UndoMove (4, 2);
            Assert.AreEqual ("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1", testDummy.ToFEN ());

            testDummy.MakeMove (60, 62);
            testDummy.UndoMove (60, 62);
            Assert.AreEqual ("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1", testDummy.ToFEN ());

            testDummy.MakeMove (60, 58);
            testDummy.UndoMove (60, 58);
            Assert.AreEqual ("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1", testDummy.ToFEN ());
        }
    }
}