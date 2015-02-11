using NUnit.Framework;
using System;
using GUI;

namespace Test
{
    [TestFixture ()]
    public class EngineTest
    {
        [Test()]
        public void BadWriteTest()
        {
            Engine engine = new Engine ("./Test.dll");    // All we need is a file which exists.
            try {
                engine.Write("If this doesn't fail then God help me.");
                Assert.Fail("Expected writing to engine to fail.");
            } catch(InvalidOperationException) {
                Assert.Pass ("Writing to engine failed as expected.");
            }
        }

        [Test()]
        public void BadReadTest()
        {
            Engine engine = new Engine ("./Test.dll");    // Filename doesn't matter
            try {
                engine.Read();
                Assert.Fail("Expected reading from engine to fail.");
            } catch(InvalidOperationException) {
                Assert.Pass ("Reading from engine failed as expected.");
            }
        }
    }
}

