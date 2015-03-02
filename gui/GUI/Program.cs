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
        public static CancellationTokenSource EngineStopTokenSource { get; private set; }
        public static ChessClock WhiteClock { get; private set; }
        public static ChessClock BlackClock { get; private set; }
        public static MainWindow win { get; private set; }
        public static PieceColour BoardOrientation { get; set; }

        public static void Main (string[] args)
        {
            CurrentBoard = new Board ();
            CurrentGameStatus = GameStatus.Inactive;
            PieceMoves.InitiateChessPieceMoves ();
            PiecePseudoLegalMoves.GeneratePseudoLegalMoves (CurrentBoard);
            PieceLegalMoves.GenerateLegalMoves (CurrentBoard);
            EngineStopTokenSource = new CancellationTokenSource ();
            BoardOrientation = PieceColour.White;
            WhiteClock = new ChessClock (PieceColour.White, new TimeSpan (0, 0, 10));
            BlackClock = new ChessClock (PieceColour.Black, new TimeSpan (0, 0, 10));

            Application.Init ();
            win = new MainWindow ();
            win.UpdateClock (WhiteClock);
            win.UpdateClock (BlackClock);
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

        public static void CancelEngineTask()
        {
            EngineStopTokenSource.Cancel ();
            Debug.Log ("Engine task cancel requested.");
        }

        public static void ResetEngineStopTokenSource()
        {
            EngineStopTokenSource.Dispose ();
            EngineStopTokenSource = new CancellationTokenSource ();
            Debug.Log ("Engine cancellation token source reset.");
        }

        public static void UpdateClock()
        {
            if (CurrentGameStatus != GameStatus.Active) {
                WhiteClock.Stop ();
                BlackClock.Stop ();
            } else {
                if (CurrentBoard.PlayerToMove == PieceColour.White) {
                    BlackClock.Stop ();
                    WhiteClock.Start ();
                } else {
                    WhiteClock.Stop ();
                    BlackClock.Start ();
                }
            }
        }

        public static void ResetClock()
        {
            WhiteClock.Stop ();
            BlackClock.Stop ();
            WhiteClock = new ChessClock (PieceColour.White, new TimeSpan (0, 30, 0));
            BlackClock = new ChessClock (PieceColour.Black, new TimeSpan (0, 30, 0));
            win.UpdateClock (WhiteClock);
            win.UpdateClock (BlackClock);
        }
    }
}
