using NUnit.Framework;
using System;
using GUI;

namespace Test
{
    [TestFixture ()]
    public class EngineTest
    {
        [Test ()]
        public void BadStartTest ()
        {
            Engine engine = new Engine("fkjflk");
            try {
                engine.Start();
                Assert.Fail("Expected engine startup to fail.");
            } catch(InvalidOperationException ex) {
                Assert.Pass ("Engine startup failed as expected.");
            }
        }

        [Test()]
        public void BadWriteTest()
        {
            Engine engine = new Engine ("");    // The actual filename doesn't matter
            try {
                engine.Write("If this doesn't fail then God help me.");
                Assert.Fail("Expected writing to engine to fail.");
            } catch(InvalidOperationException ex) {
                Assert.Pass ("Writing to engine failed as expected.");
            }
        }

        [Test()]
        public void BadReadTest()
        {
            Engine engine = new Engine ("");    // Filename doesn't matter
            try {
                engine.Read();
                Assert.Fail("Expected reading from engine to fail.");
            } catch(InvalidOperationException ex) {
                Assert.Pass ("Reading from engine failed as expected.");
            }
        }
    }
}

