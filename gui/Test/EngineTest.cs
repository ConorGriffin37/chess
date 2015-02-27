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
            Assert.Throws<InvalidOperationException> (delegate {
                engine.Write("Should fail.");
            });
        }

        [Test()]
        public void BadReadTest()
        {
            Engine engine = new Engine ("./Test.dll");    // Filename doesn't matter
            Assert.Throws<InvalidOperationException> (delegate {
                engine.Read();
            });
        }
    }
}

