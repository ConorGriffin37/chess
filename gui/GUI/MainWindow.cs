using System;
using System.Threading;
using System.Threading.Tasks;
using System.Text.RegularExpressions;
using Gtk;
using Cairo;

namespace GUI
{
    public enum PieceSelectionState { None, Selected }

    public partial class MainWindow: Gtk.Window
    {
        ImageSurface boardBackground;
        ImageSurface selectionBorder;
        PieceSelectionState currentSelectionState = PieceSelectionState.None;
        Regex engineOutputRegex = new Regex (@"^(?=.*(depth \d*))(?=.*(nps \d*))(?=.*(score cp [+\-0-9]*))(?=.*(pv [a-h12345678 ]*)).*$");
        byte selectedPiece;
        int materialDifference = 0;
        Cairo.Context boardContext;

        public MainWindow () : base (Gtk.WindowType.Toplevel)
        {
            boardBackground = new ImageSurface ("img/board.png");
            selectionBorder = new ImageSurface ("img/border.png");
            PieceDisplay.Init ();
            Build ();
        }

        public void InitWidgets()
        {
            BoardArea.AddEvents ((int)Gdk.EventMask.ButtonPressMask);
            MaterialDifferenceLabel.Text = "Material is equal.";
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

        protected void OnBoardExpose (object o, ExposeEventArgs args)
        {
            RedrawBoard ();
        }

        public void ShowGameOverDialog(GameStatus status)
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
                case GameStatus.WhiteTime:
                    message = "Time expired: White. Black wins.";
                    MainClass.CurrentGameStatus = GameStatus.WhiteTime;
                    break;
                case GameStatus.BlackTime:
                    message = "Time expired: Black. White wins.";
                    MainClass.CurrentGameStatus = GameStatus.BlackTime;
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
                    .ContinueWith (task => ParseAndMakeMove (task.Result),
                        MainClass.EngineStopTokenSource.Token);
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
                Gtk.Application.Invoke(delegate {
                    RedrawBoard();
                });
            } catch(InvalidOperationException) {
                throw new InvalidOperationException (move);
            }
            MainClass.CurrentGameStatus = MainClass.CurrentBoard.CheckForMate ();
            if (MainClass.CurrentGameStatus != GameStatus.Active && MainClass.CurrentGameStatus != GameStatus.Inactive) {
                Gtk.Application.Invoke(delegate {
                    ShowGameOverDialog(MainClass.CurrentGameStatus);
                });
            }
            Gtk.Application.Invoke (delegate {
                MainClass.UpdateClock ();
            });
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

        protected void OnSetClock (object sender, EventArgs e)
        {
            SetClockTimeDialog time = new SetClockTimeDialog ();
            if (time.Run () == (int)ResponseType.Ok) {
                try {
                    MainClass.SetClock(TimeSpan.Parse(time.Time));
                    MainClass.CurrentGameStatus = GameStatus.Inactive;
                } catch(Exception ex) {
                    Console.Error.WriteLine ("Error parsing game time: " + ex.Message);
                    MessageDialog errorDialog = new MessageDialog (
                                                    time,
                                                    DialogFlags.DestroyWithParent,
                                                    MessageType.Error,
                                                    ButtonsType.Ok,
                                                    "Invalid game time entered.");
                    errorDialog.Run ();
                    errorDialog.Destroy ();
                }
            }
            time.Destroy ();
        }

        public void LogEngineOutput(string output)
        {
            Match match = engineOutputRegex.Match (output);
            if (match.Success) {
                EngineDepthLabel.Text = "Depth: " + match.Groups [1].Value.Substring (5);
                EngineNPSLabel.Text = "NPS: " + match.Groups [2].Value.Substring (3);
                string score = match.Groups [3].Value.Substring (9);
                if (score != "0") {
                    if (score.StartsWith ("-")) {
                        if (MainClass.CurrentBoard.PlayerToMove == PieceColour.White) {
                            score = "-" + score.Substring (1).PadLeft (2, '0');
                        } else {
                            score = "+" + score.Substring (1).PadLeft (2, '0');
                        }
                        score = score.Insert (score.Length - 2, ".");
                        if (score.Length == 4) {
                            score = score.Insert (1, "0");
                        }
                    } else {
                        score = score.PadLeft (2, '0');
                        score = score.Insert (score.Length - 2, ".");
                        if (MainClass.CurrentBoard.PlayerToMove == PieceColour.White) {
                            score = score.Insert (0, "+");
                        } else {
                            score = score.Insert (0, "-");
                        }
                        if (score.Length == 4) {
                            score = score.Insert (1, "0");
                        }
                    }
                }
                string pv = match.Groups [4].Value.Substring (2);
                TextIter iter = EngineOutput.GetIterAtLocation (0, 0);
                EngineOutput.Buffer.Insert (ref iter, score + " " + pv + Environment.NewLine);
            }
        }

        public void LogEngineNameAndAuthor(string name, string author)
        {
            EngineNameLabel.Text = name;
            EngineAuthorLabel.Text = author;
            ClearEngineOutput ();
            EngineOutput.Buffer.Text = "Engine loaded: " + name;
        }

        public void ClearEngineInfo()
        {
            EngineNameLabel.Text = "No engine loaded";
            EngineAuthorLabel.Text = "";
            EngineDepthLabel.Text = "";
            EngineNPSLabel.Text = "";
        }

        public void ClearEngineOutput()
        {
            EngineOutput.Buffer.Clear ();
        }

        protected void OnPieceClick (object o, ButtonPressEventArgs args)
        {
            Debug.Log (String.Format("BoardArea press at ({0}, {1})", args.Event.X, args.Event.Y));

            double transx = Math.Abs ((BoardArea.Allocation.Width - (boardBackground.Width * 0.75))) / 2;

            PointD clickLocation = new PointD (args.Event.X - transx, args.Event.Y - transx);
            if (clickLocation.X < 30 || clickLocation.Y < 30
                || clickLocation.X > 522 || clickLocation.Y > 522)
                return;

            PointD pieceLocation = PieceDisplay.pieceCoordinates [0];
            int pieceIndex = 0;
            for (int i = 0; i < PieceDisplay.pieceCoordinates.Length; i++) {
                PointD p = PieceDisplay.pieceCoordinates[i];
                double x1 = p.X * 0.75;
                double y1 = p.Y * 0.75;
                double x2 = x1 + 61.5;
                double y2 = y1 + 61.5;
                if (x1 <= clickLocation.X && clickLocation.X <= x2) {
                    if (y1 <= clickLocation.Y && clickLocation.Y <= y2) {
                        pieceLocation = p;
                        pieceIndex = i;
                        break;
                    }
                }
            }

            if (currentSelectionState == PieceSelectionState.None) {
                if (MainClass.CurrentBoard.Squares [pieceIndex].Piece == null)
                    return;
                selectedPiece = (byte)pieceIndex;

                boardContext = Gdk.CairoHelper.Create (BoardArea.GdkWindow);
                boardContext.Translate (transx, 0);
                boardContext.Scale (0.75, 0.75);
                selectionBorder.Show (boardContext, pieceLocation.X + 1, pieceLocation.Y + 1);
                boardContext.Dispose ();
                currentSelectionState = PieceSelectionState.Selected;
            } else {
                if (MainClass.CurrentEngine != null && MainClass.CurrentEngine.IsThinking) {
                    MainClass.CancelEngineTask ();
                    MainClass.CurrentEngine.StopAndIgnoreMove ();
                }

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

                // Handle pawn promotion
                PieceType? promoteTo = null;
                if (MainClass.CurrentBoard.Squares [selectedPiece].Piece.Type == PieceType.Pawn &&
                   MainClass.CurrentBoard.IsMoveValid (selectedPiece, (byte)pieceIndex) &&
                   Array.IndexOf (MainClass.CurrentBoard.pawnPromotionDestinations, (byte)pieceIndex) != -1) {
                    PawnPromotionDialog dialog = new PawnPromotionDialog ();
                    if (dialog.Run () == (int)Gtk.ResponseType.Ok) {
                        promoteTo = dialog.PromoteTo;
                    } else {
                        dialog.Destroy ();
                        return;
                    }
                    dialog.Destroy ();
                }

                try {
                    MainClass.CurrentBoard.MakeMove (selectedPiece, (byte)pieceIndex, promoteTo);
                } catch(InvalidOperationException) {
                    Debug.Log ("Invalid move entered.");
                }
                Gtk.Application.Invoke(delegate {
                    RedrawBoard();
                });
                MainClass.CurrentGameStatus = MainClass.CurrentBoard.CheckForMate ();
                if (MainClass.CurrentGameStatus != GameStatus.Active && MainClass.CurrentGameStatus != GameStatus.Inactive) {
                    ShowGameOverDialog (MainClass.CurrentGameStatus);
                }
                Gtk.Application.Invoke (delegate {
                    MainClass.UpdateClock ();
                });

                currentSelectionState = PieceSelectionState.None;
            }
        }

        /**
         * @fn UpdateMaterialDifference
         * @brief Updates the material difference counter in the sidebar.
         * 
         * @param value the value of the piece just taken. Positive for white and negative for black
         */
        public void UpdateMaterialDifference(int value)
        {
            materialDifference += value;
            if (materialDifference < 0) {
                MaterialDifferenceLabel.Text = String.Format ("Black is {0} pawn{1} up.",
                    Math.Abs (materialDifference), materialDifference < -1 ? "s" : "");
            } else if (materialDifference > 0) {
                MaterialDifferenceLabel.Text = String.Format ("White is {0} pawn{1} up.",
                    Math.Abs (materialDifference), materialDifference > 1 ? "s" : "");
            } else {
                MaterialDifferenceLabel.Text = "Material is equal.";
            }
        }
    }
}
