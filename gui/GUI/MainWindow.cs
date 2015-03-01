using System;
using Gtk;
using Cairo;
using System.Threading;
using System.Threading.Tasks;

namespace GUI
{
    public partial class MainWindow: Gtk.Window
    {
        ImageSurface boardBackground;

        Cairo.Context boardContext;

        public MainWindow () : base (Gtk.WindowType.Toplevel)
        {
            boardBackground = new ImageSurface ("img/board.png");
            PieceDisplay.Init ();
            Build ();
        }

        protected void OnDeleteEvent (object sender, DeleteEventArgs a)
        {
            if (MainClass.CurrentEngine != null)
                MainClass.CurrentEngine.Quit ();
            Application.Quit ();
            a.RetVal = true;
        }

        protected void OnQuit (object sender, EventArgs e)
        {
            if (MainClass.CurrentEngine != null)
                MainClass.CurrentEngine.Quit ();
            Application.Quit ();
        }

        protected void OnAbout (object sender, EventArgs e)
        {
            AboutDialog about = new AboutDialog ();
            about.ProgramName = "Gandalf Chess";
            about.Authors = new string[] { "Terry Bolt", "Conor Griffin", "Darragh Griffin" };
            about.Version = "0.1";
            about.Copyright = "Copyright 2015 Terry Bolt, Conor Griffin, Darragh Griffin";
            about.Run ();
            about.Destroy ();
        }

        protected void OnLoadFEN (object sender, EventArgs e)
        {
            if (MainClass.CurrentEngine != null && MainClass.CurrentEngine.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.CurrentEngine.StopAndIgnoreMove ();
            }
            LoadFENDialog fen = new LoadFENDialog();
            if (fen.Run () == (int)ResponseType.Ok) {
                try {
                    FENParser parser = new FENParser(fen.FENString);
                    MainClass.CurrentBoard = parser.GetBoard();
                    MainClass.CurrentGameStatus = GameStatus.Inactive;
                    PiecePseudoLegalMoves.GeneratePseudoLegalMoves(MainClass.CurrentBoard);
                    PieceLegalMoves.GenerateLegalMoves(MainClass.CurrentBoard);
                    RedrawBoard();
                } catch(ArgumentException ex) {
                    Console.Error.WriteLine ("Error parsing FEN string: " + ex.Message);
                    MessageDialog errorDialog = new MessageDialog (
                                                    fen,
                                                    DialogFlags.DestroyWithParent,
                                                    MessageType.Error,
                                                    ButtonsType.Ok,
                                                    "Error parsing FEN string.");
                    errorDialog.Run ();
                    errorDialog.Destroy ();
                }
            }
            fen.Destroy ();
            MainClass.CurrentGameStatus = GameStatus.Inactive;
            GameStatus currentStatus = MainClass.CurrentBoard.CheckForMate ();
            if (currentStatus != GameStatus.Inactive && currentStatus != GameStatus.Active) {
                ShowGameOverDialog (currentStatus);
            }
            MainClass.ResetClock ();
        }

        protected void OnLoadEngine (object sender, EventArgs e)
        {
            if (MainClass.CurrentEngine != null && MainClass.CurrentEngine.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.CurrentEngine.StopAndIgnoreMove ();
            }
            FileChooserDialog chooser = new FileChooserDialog (
                                            "Please choose an engine executable.",
                                            this,
                                            FileChooserAction.Open,
                                            "Cancel", ResponseType.Cancel,
                                            "Open", ResponseType.Accept);

            if (chooser.Run() == (int)ResponseType.Accept) {
                try {
                    MainClass.CurrentEngine = new UCITransceiver(chooser.Filename);
                    MainClass.CurrentEngine.Init();
                } catch(Exception ex) {
                    Console.Error.WriteLine ("(EE) Error opening engine file: " + ex.Message);
                    MessageDialog errorDialog = new MessageDialog (
                                                    chooser,
                                                    DialogFlags.DestroyWithParent,
                                                    MessageType.Error,
                                                    ButtonsType.Ok,
                                                    "Error loading engine file.");
                    errorDialog.Run ();
                    errorDialog.Destroy ();
                }
            }
            chooser.Destroy ();
        }

        protected void OnMoveEntry (object sender, EventArgs e)
        {
            if (MainClass.CurrentEngine != null && MainClass.CurrentEngine.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.CurrentEngine.StopAndIgnoreMove ();
            }
            string userMove = MoveEntry.Text;
            MoveEntry.Text = "";
            if (MainClass.CurrentGameStatus != GameStatus.Active &&
                MainClass.CurrentGameStatus != GameStatus.Inactive) {
                Console.Error.WriteLine ("(EE) Attempted move during finished game.");
                MessageDialog errorDialog = new MessageDialog (
                                                this,
                                                DialogFlags.DestroyWithParent,
                                                MessageType.Error,
                                                ButtonsType.Ok,
                                                "The game is over!");
                errorDialog.Run ();
                errorDialog.Destroy ();
                return;
            }

            if (userMove.Length < 4) {
                Console.Error.WriteLine ("(EE) Error making move.");
                MessageDialog errorDialog = new MessageDialog (
                                                this,
                                                DialogFlags.DestroyWithParent,
                                                MessageType.Error,
                                                ButtonsType.Ok,
                                                "Please enter a move.");
                errorDialog.Run ();
                errorDialog.Destroy ();
                return;
            }

            try {
                ParseAndMakeMove(userMove);
            } catch(InvalidOperationException io) {
                Console.Error.WriteLine ("(EE) Illegal move entered: " + io.Message);
                MessageDialog errorDialog = new MessageDialog (
                                                this,
                                                DialogFlags.DestroyWithParent,
                                                MessageType.Error,
                                                ButtonsType.Ok,
                                                "Illegal move entered.");
                errorDialog.Run ();
                errorDialog.Destroy ();
                return;
            }
        }

        protected void OnBoardExpose (object o, ExposeEventArgs args)
        {
            RedrawBoard ();
        }

        void ShowGameOverDialog(GameStatus status)
        {
            string message = "";
            switch (status) {
                case GameStatus.Stalemate:
                    message = "Draw by stalemate.";
                    MainClass.CurrentGameStatus = GameStatus.Stalemate;
                    break;
                case GameStatus.WhiteCheckmate:
                    message = "Black wins by checkmate!";
                    MainClass.CurrentGameStatus = GameStatus.WhiteCheckmate;
                    break;
                case GameStatus.BlackCheckmate:
                    message = "White wins by checkmate!";
                    MainClass.CurrentGameStatus = GameStatus.BlackCheckmate;
                    break;
                case GameStatus.WhiteAdjudicate:
                    message = "Illegal move by white. Gandalf adjudication: Black wins.";
                    MainClass.CurrentGameStatus = GameStatus.BlackAdjudicate;
                    break;
                case GameStatus.BlackAdjudicate:
                    message = "Illegal move by black. Gandalf adjudication: White wins.";
                    MainClass.CurrentGameStatus = GameStatus.WhiteAdjudicate;
                    break;
                default:
                    break;
            }
            MessageDialog gameOverDialog = new MessageDialog (
                                               this,
                                               DialogFlags.DestroyWithParent,
                                               MessageType.Info,
                                               ButtonsType.Ok,
                                               message);
            gameOverDialog.Run ();
            gameOverDialog.Destroy ();
        }

        void RedrawBoard()
        {
            Debug.Log ("Redrawing board.");
            boardContext = Gdk.CairoHelper.Create (BoardArea.GdkWindow);
            double transx = Math.Abs((BoardArea.Allocation.Width - (boardBackground.Width * 0.75))) / 2;
            boardContext.Translate (transx, 0);
            boardContext.Scale (0.75, 0.75);
            boardBackground.Show (boardContext, 0, 0);
            PieceDisplay.DrawPieces (boardContext);
            boardContext.Dispose ();
        }

        byte NotationToBoardSquare(string notation)
        {
            byte col = (byte)(Convert.ToInt32 (notation [0]) - 97);  // 97 is ASCII value for 'a'
            byte row = (byte)Math.Abs(Char.GetNumericValue (notation [1]) - 8);
            byte square = (byte)(col + (row * 8));

            return square;
        }

        protected void OnResetBoard (object sender, EventArgs e)
        {
            if (MainClass.CurrentEngine != null && MainClass.CurrentEngine.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.CurrentEngine.StopAndIgnoreMove ();
            }
            MainClass.CurrentBoard = new Board ();
            MainClass.CurrentGameStatus = GameStatus.Inactive;
            MainClass.ResetClock ();
            PiecePseudoLegalMoves.GeneratePseudoLegalMoves (MainClass.CurrentBoard);
            PieceLegalMoves.GenerateLegalMoves (MainClass.CurrentBoard);
            RedrawBoard ();
        }

        protected void OnMakeEngineMove (object sender, EventArgs e)
        {
            if (MainClass.CurrentEngine == null) {
                Console.Error.WriteLine ("(EE) Engine not loaded.");
                MessageDialog errorDialog = new MessageDialog (
                                                this,
                                                DialogFlags.DestroyWithParent,
                                                MessageType.Error,
                                                ButtonsType.Ok,
                                                "Please load an engine first.");
                errorDialog.Run ();
                errorDialog.Destroy ();
                return;
            }

            if (MainClass.CurrentEngine.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.CurrentEngine.StopAndIgnoreMove ();
            }
            string currentFEN = MainClass.CurrentBoard.ToFEN ();
            MainClass.CurrentEngine.SendPosition (currentFEN);
            MainClass.CurrentEngine.WaitUntilReady ();
            try {
                var engineMoveTask = Task.Factory.StartNew<string> (
                                         () => MainClass.CurrentEngine.Go ("depth 5"),
                                         MainClass.EngineStopTokenSource.Token
                                     )
                    .ContinueWith (task => ParseAndMakeMove (task.Result), MainClass.EngineStopTokenSource.Token);
            } catch(AggregateException ae) {
                ae.Handle ((x) => {
                    if (x is InvalidOperationException) {
                        Console.Error.WriteLine ("(EE) Engine tried to make illegal move: " + x.Message);
                        MainClass.CurrentGameStatus = GameStatus.WhiteAdjudicate;
                        ShowGameOverDialog (GameStatus.WhiteAdjudicate);
                        return true;
                    }
                    return false;
                });
            }
        }

        private void ParseAndMakeMove(string move)
        {
            string sourceStr = move.Substring (0, 2);
            string destinationStr = move.Substring (2, 2);
            string promoteToStr = "";
            if (move.Length > 4) {
                promoteToStr = move.Substring (4, 1);
            }

            byte sourceByte = NotationToBoardSquare (sourceStr);
            byte destinationByte = NotationToBoardSquare (destinationStr);
            PieceType? promoteTo = null;
            switch (promoteToStr) {
                case "n":
                    promoteTo = PieceType.Knight;
                    break;
                case "b":
                    promoteTo = PieceType.Bishop;
                    break;
                case "r":
                    promoteTo = PieceType.Rook;
                    break;
                case "q":
                    promoteTo = PieceType.Queen;
                    break;
                default:
                    break;
            }

            try {
                MainClass.CurrentBoard.MakeMove (sourceByte, destinationByte, promoteTo);
                RedrawBoard();
            } catch(InvalidOperationException) {
                throw new InvalidOperationException (move);
            }
            MainClass.CurrentGameStatus = MainClass.CurrentBoard.CheckForMate ();
            if (MainClass.CurrentGameStatus != GameStatus.Active && MainClass.CurrentGameStatus != GameStatus.Inactive) {
                ShowGameOverDialog (MainClass.CurrentGameStatus);
            }
            MainClass.UpdateClock ();
        }

        public void UpdateClock(ChessClock clock)
        {
            if (clock.Colour == PieceColour.White) {
                WhiteClockLabel.Text = clock.TimeLeft.ToString ("g");
            } else {
                BlackClockLabel.Text = clock.TimeLeft.ToString ("g");
            }
        }

        protected void OnFlipBoard (object sender, EventArgs e)
        {
            MainClass.BoardOrientation = MainClass.BoardOrientation == PieceColour.White ?
                PieceColour.Black : PieceColour.White;
            RedrawBoard ();
        }
    }
}
