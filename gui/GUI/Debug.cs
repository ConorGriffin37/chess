using System;

namespace GUI
{
    public static class Debug
    {
        [System.Diagnostics.Conditional("DEBUG")]
        public static void Log(string message)
        {
            Console.WriteLine ("DEBUG: " + message);
        }
    }
}

