using NUnit.Framework;
using System;
using System.IO;
using GUI;

namespace Test
{
    [TestFixture ()]
    public class UCITransceiverTest
    {
        [Test ()]
        public void BadFilenameTest ()
        {
            // This is really the only thing we can test consistently
            // as any others depend on including an engine binary in the
            // git repo, which I absolutely don't want to do.
            Assert.Throws<ArgumentException> (delegate {
                new UCITransceiver("fsdlfkjL");
            });
        }
    }
}

