using NUnit.Framework;
using System;
using GUI;

namespace Test
{
    [TestFixture ()]
    public class FENParserTest
    {
        [Test ()]
        public void GetBoardTest ()
        {
            Board defaultBoard = new Board ();
            FENParser parser = new FENParser ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            Board fenBoard = parser.GetBoard ();

            Assert.AreEqual (defaultBoard, fenBoard);
        }

        [Test()]
        public void BadFENStringTest()
        {
            Exception ex = Assert.Throws<ArgumentException> (() => new FENParser ("sdlfkjhlkJ"));
            Assert.AreEqual (ex.Message, "Bad FEN string passed to parser.\nParameter name: fen");
        }

        [Test()]
        public void PiecePlacementTokenTooLongTest()
        {
            FENParser parser = new FENParser ("rnbqkbnrr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            Exception ex = Assert.Throws<ArgumentException> (() => parser.GetBoard());
            Assert.AreEqual (ex.Message, "Bad FEN field: Piece placement.\nParameter name: fen");
        }

        [Test()]
        public void BadColourToMoveTokenTest()
        {
            FENParser parser = new FENParser ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR o KQkq - 0 1");
            Exception ex = Assert.Throws<ArgumentException> (() => parser.GetBoard());
            Assert.AreEqual (ex.Message, "Bad FEN field: Colour to move.\nParameter name: fen");
        }

        [Test()]
        public void BadCastlingPossibilitiesTokenTest()
        {
            FENParser parser = new FENParser ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQQkq - 0 1");
            Exception ex = Assert.Throws<ArgumentException> (() => parser.GetBoard());
            Assert.AreEqual (ex.Message, "Bad FEN field: Castling possibilities.\nParameter name: fen");
        }
    }
}

