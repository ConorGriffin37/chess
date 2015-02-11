using System;
using Gtk;
using System.Threading;

namespace GUI
{
    class MainClass
    {
        public static void Main (string[] args)
        {
            /*
            Application.Init ();
            MainWindow win = new MainWindow ();
            win.Show ();
            Application.Run ();
            */

            /*
            UCITransceiver uci = new UCITransceiver ("./stockfish_6_x64");
            uci.Init ();
            Console.WriteLine (uci.EngineName);
            Console.WriteLine (uci.EngineAuthor);
            uci.SendPosition ("startpos", "e2e4 e7e5");
            uci.Go ();
            Thread.Sleep (5000);
            Console.WriteLine (uci.StopAndGetBestMove ());
            uci.Quit ();
            */
        }
    }
}
