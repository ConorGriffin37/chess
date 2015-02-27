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
            Assert.Throws<ArgumentException> (delegate {
                new FENParser("glkfjlk");
            });
        }

        [Test()]
        public void PiecePlacementTokenTooLongTest()
        {
            FENParser parser = new FENParser ("rnbqkbnrr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            Assert.Throws<ArgumentException> (delegate {
                parser.GetBoard ();
            });
        }

        [Test()]
        public void BadColourToMoveTokenTest()
        {
            FENParser parser = new FENParser ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR o KQkq - 0 1");
            Assert.Throws<ArgumentException> (delegate {
                parser.GetBoard ();
            });
        }

        [Test()]
        public void BadCastlingPossibilitiesTokenTest()
        {
            FENParser parser = new FENParser ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQQkq - 0 1");
            Assert.Throws<ArgumentException> (delegate {
                parser.GetBoard ();
            });
        }
    }
}

