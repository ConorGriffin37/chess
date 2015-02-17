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

        [Test()]
        public void BoardToFENTest()
        {
            Board startBoard = new Board ();
            FENParser fen = new FENParser ("rn2kbnr/ppq2pp1/2p1p2p/7P/3P4/3Q1NN1/PPP2PP1/R1B1K2R w KQkq - 0 11");

            Assert.AreEqual ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", startBoard.ToFEN ());
            // Note that in the following comparison the expected FEN string has a fullmove
            // counter of 1 instead of the 11 we gave to the FEN parser. This is because move
            // history has not yet been implemented. There is no error in the code.
            Assert.AreEqual ("rn2kbnr/ppq2pp1/2p1p2p/7P/3P4/3Q1NN1/PPP2PP1/R1B1K2R w KQkq - 0 1", fen.GetBoard ().ToFEN ());
        }

        [Test()]
        public void MakeMoveTest()
        {
            Board board = new Board ();
            PieceMoves.InitiateChessPieceMoves ();
            PiecePseudoLegalMoves.GeneratePseudoLegalMoves (board);
            PieceLegalMoves.GenerateLegalMoves (board);
            board.MakeMove (62, 45);
            Assert.AreEqual ("rnbqkbnr/pppppppp/8/8/8/5N2/PPPPPPPP/RNBQKB1R b KQkq - 0 1", board.ToFEN ());
        }

        [Test()]
        public void UndoMoveTest()
        {
            Board board = new Board ();
            PieceMoves.InitiateChessPieceMoves ();
            PiecePseudoLegalMoves.GeneratePseudoLegalMoves (board);
            PieceLegalMoves.GenerateLegalMoves (board);
            board.MakeMove (62, 45);
            board.UndoMove (62, 45);
            Assert.AreEqual ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", board.ToFEN ());
        }

        [Test()]
        public void CheckForMateWithMateTest()
        {
            FENParser parser = new FENParser ("rnbqkbnr/ppppp2p/8/5ppQ/4PP2/8/PPPP2PP/RNB1KBNR b KQkq - 0 3");
            Board board = parser.GetBoard ();
            PieceMoves.InitiateChessPieceMoves ();
            PiecePseudoLegalMoves.GeneratePseudoLegalMoves (board);
            PieceLegalMoves.GenerateLegalMoves (board);
            Assert.AreEqual (GameStatus.BlackCheckmate, board.CheckForMate ());
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

            Assert.AreEqual ("Q", queen.ToString());
            Assert.AreEqual ("n", blackKnight.ToString ());
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
            Assert.AreEqual ("n", containsBlackKnight.ToString ());
            Assert.AreEqual ("_", alsoEmpty.ToString ());
        }
    }
}

