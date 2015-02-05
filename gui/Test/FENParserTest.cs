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
    }
}

