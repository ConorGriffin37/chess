using System;
using Gtk;
using System.Threading;

namespace GUI
{
    class MainClass
    {
        public static Board CurrentBoard { get; set; }
        public static UCITransceiver CurrentEngine { get; set; }
        public static GameStatus CurrentGameStatus { get; set; }

        public static void Main (string[] args)
        {
            CurrentBoard = new Board ();
            CurrentGameStatus = GameStatus.Unfinished;
            PieceMoves.InitiateChessPieceMoves ();
            PiecePseudoLegalMoves.GeneratePseudoLegalMoves (CurrentBoard);
            PieceLegalMoves.GenerateLegalMoves (CurrentBoard);

            Application.Init ();
            MainWindow win = new MainWindow ();
            win.Show ();
            Application.Run ();

            /*
            UCITransceiver uci = new UCITransceiver ("./stockfish_6_x64");
            uci.Init ();
            Console.WriteLine (uci.EngineName);
            Console.WriteLine (uci.EngineAuthor);
            uci.SendPosition ("rn2kbnr/ppq2pp1/2p1p2p/7P/3P4/3Q1NN1/PPP2PP1/R1B1K2R w KQkq - 0 11");
            uci.Go ();
            Thread.Sleep (5000);
            Console.WriteLine (uci.StopAndGetBestMove ());
            uci.Quit ();
            */
        }
    }
}
