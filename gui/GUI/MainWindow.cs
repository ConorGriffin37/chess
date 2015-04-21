using System;
using System.Threading;
using System.Threading.Tasks;
using System.Text.RegularExpressions;
using System.IO;
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
        Regex engineOutputRegex = new Regex (@"^(?=.*(depth \d*))(?=.*(nps \d*))(?=.*(score (?:cp|mate) [+\-0-9]*))(?=.*(pv [a-h12345678 ]*)).*$");
        byte selectedPiece;
        int materialDifference = 0;
        Cairo.Context boardContext;
        byte[] pieceValues = { 1, 3, 3, 5, 8 }; // Pawn, Knight, Bishop, Rook, Queen
        byte[] whiteSquares = { 1, 3, 5, 7, 8, 10, 12, 14, 17, 19, 21, 23, 24, 26, 28, 30, 33,
                                35, 37, 39, 40, 42, 44, 46, 49, 51, 53, 55, 56, 58, 60, 62 };
        DateTime engineThinkCooldown = DateTime.Now;

        Task engineThinkTask;

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
            if (MainClass.EngineOne != null)
                MainClass.EngineOne.Quit ();
            if (MainClass.EngineTwo != null)
                MainClass.EngineTwo.Quit ();
            Application.Quit ();
            a.RetVal = true;
        }

        protected void OnQuit (object sender, EventArgs e)
        {
            if (MainClass.EngineOne != null)
                MainClass.EngineOne.Quit ();
            if (MainClass.EngineTwo != null)
                MainClass.EngineTwo.Quit ();
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
            if (MainClass.EngineOne != null && MainClass.EngineOne.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.EngineOne.StopAndIgnoreMove ();
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
            UpdateMaterialDifference (MainClass.CurrentBoard);
            MainClass.CurrentGameHistory = new GameHistory ();
            ClearGameHistoryView ();
            if (MainClass.CurrentBoard.PlayerToMove == PieceColour.Black) {
                GameHistoryView.Buffer.Text = "1. ... ";
            }
        }

        protected void OnLoadEngineOne (object sender, EventArgs e)
        {
            if (MainClass.EngineOne != null && MainClass.EngineOne.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.EngineOne.StopAndIgnoreMove ();
            }
            FileChooserDialog chooser = new FileChooserDialog (
                                            "Please choose an engine executable.",
                                            this,
                                            FileChooserAction.Open,
                                            "Cancel", ResponseType.Cancel,
                                            "Open", ResponseType.Accept);

            if (chooser.Run() == (int)ResponseType.Accept) {
                try {
                    MainClass.EngineOne = new UCITransceiver(chooser.Filename, 1);
                    MainClass.EngineOne.Init();
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
                case GameStatus.DrawInsuffientMaterial:
                    message = "Game is a draw due to insufficient material.";
                    MainClass.CurrentGameStatus = GameStatus.DrawInsuffientMaterial;
                    break;
                case GameStatus.DrawFifty:
                    message = "Game is a draw according to the 50-move rule.";
                    MainClass.CurrentGameStatus = GameStatus.DrawFifty;
                    break;
                case GameStatus.DrawRepetition:
                    message = "Game is a draw by threefold repetition.";
                    MainClass.CurrentGameStatus = GameStatus.DrawRepetition;
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
            if (MainClass.EngineOne != null && MainClass.EngineOne.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.EngineOne.StopAndIgnoreMove ();
            }
            if (MainClass.EngineTwo != null && MainClass.EngineOne.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.EngineTwo.StopAndIgnoreMove ();
            }

            MainClass.CurrentBoard = new Board ();
            MainClass.CurrentGameStatus = GameStatus.Inactive;
            MainClass.ResetClock ();
            PiecePseudoLegalMoves.GeneratePseudoLegalMoves (MainClass.CurrentBoard);
            PieceLegalMoves.GenerateLegalMoves (MainClass.CurrentBoard);
            MainClass.CurrentGameHistory = new GameHistory ();
            ClearGameHistoryView ();
            RedrawBoard ();
        }

        protected void OnMakeEngineMove (object sender, EventArgs e)
        {
            if ((DateTime.Now - engineThinkCooldown).TotalMilliseconds < 500)
                // Force a cooldown of 0.5 seconds between requests for the engine to think.
                return;

            if (MainClass.EngineOne == null) {
                Console.Error.WriteLine ("(EE) Engine not loaded.");
                Gtk.Application.Invoke (delegate {
                    MessageDialog errorDialog = new MessageDialog (
                                                    this,
                                                    DialogFlags.DestroyWithParent,
                                                    MessageType.Error,
                                                    ButtonsType.Ok,
                                                    "Please load an engine first.");
                    errorDialog.Run ();
                    errorDialog.Destroy ();
                });
                return;
            }

            EngineOneMove ();

            engineThinkCooldown = DateTime.Now;
        }

        void EngineOneMove()
        {
            if (MainClass.EngineOne.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.EngineOne.StopAndIgnoreMove ();
            }

            string currentFEN = MainClass.CurrentBoard.ToFEN ();
            MainClass.EngineOne.SendPosition (currentFEN);
            MainClass.EngineOne.WaitUntilReady ();
            string time = (MainClass.StrengthType == StrengthMeasure.Depth) ? "depth " : "movetime ";
            time += (MainClass.StrengthType == StrengthMeasure.Depth) ? MainClass.StrengthValue : MainClass.StrengthValue * 1000;
            try {
                engineThinkTask = Task.Factory.StartNew<string> (
                    () => MainClass.EngineOne.Go (time),
                    MainClass.EngineStopTokenSource.Token
                )
                    .ContinueWith (task => ParseAndMakeMove (task.Result, 1),
                        MainClass.EngineStopTokenSource.Token);
            } catch(AggregateException ae) {
                ae.Handle ((x) => {
                    if (x is InvalidOperationException) {
                        Console.Error.WriteLine ("(EE) Engine tried to make illegal move: " + x.Message);
                        MainClass.CurrentGameStatus = GameStatus.WhiteAdjudicate;
                        Gtk.Application.Invoke(delegate {
                            ShowGameOverDialog(MainClass.CurrentGameStatus);
                        });
                        return true;
                    }
                    return false;
                });
            }
        }

        void EngineTwoMove()
        {
            if (MainClass.EngineTwo.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.EngineTwo.StopAndIgnoreMove ();
            }

            string currentFEN = MainClass.CurrentBoard.ToFEN ();
            MainClass.EngineTwo.SendPosition (currentFEN);
            MainClass.EngineTwo.WaitUntilReady ();
            string time = (MainClass.StrengthType == StrengthMeasure.Depth) ? "depth " : "movetime ";
            time += (MainClass.StrengthType == StrengthMeasure.Depth) ? MainClass.StrengthValue : MainClass.StrengthValue * 1000;
            try {
                engineThinkTask = Task.Factory.StartNew<string> (
                    () => MainClass.EngineTwo.Go (time),
                    MainClass.EngineStopTokenSource.Token
                )
                    .ContinueWith (task => ParseAndMakeMove (task.Result, 2),
                        MainClass.EngineStopTokenSource.Token);
            } catch(AggregateException ae) {
                ae.Handle ((x) => {
                    if (x is InvalidOperationException) {
                        Console.Error.WriteLine ("(EE) Engine tried to make illegal move: " + x.Message);
                        MainClass.CurrentGameStatus = GameStatus.WhiteAdjudicate;
                        Gtk.Application.Invoke(delegate {
                            ShowGameOverDialog(MainClass.CurrentGameStatus);
                        });
                        return true;
                    }
                    return false;
                });
            }
        }

        private void ParseAndMakeMove(string move, int engine)
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
                SpecifierType specifierRequired = GameHistory.checkDisabiguationNeeded(MainClass.CurrentBoard, sourceByte, destinationByte);
                MoveResult result = MainClass.CurrentBoard.MakeMove (sourceByte, destinationByte, promoteTo);

                Piece movingPiece = null;
                if(promoteTo == null) {
                    movingPiece = MainClass.CurrentBoard.Squares[destinationByte].Piece;
                } else {
                    movingPiece = new Piece(MainClass.CurrentBoard.Squares [destinationByte].Piece.Colour, PieceType.Pawn);
                }

                if(result == MoveResult.Capture && movingPiece.Type == PieceType.Pawn) {
                    specifierRequired = SpecifierType.File;
                }

                string fenPosition = MainClass.CurrentBoard.ToFEN().Split(' ')[0];
                MainClass.CurrentGameHistory.AddMove(new Move(sourceByte, destinationByte,
                    MainClass.CurrentBoard.Squares [destinationByte].Piece.Colour,
                    movingPiece,
                    result,
                    MainClass.CurrentBoard.ToFEN(),
                    specifierRequired,
                    promoteTo), fenPosition);
                UpdateGameHistoryView();

                if (MainClass.CurrentGameHistory.UpdateFiftyMoveCount (result) == GameStatus.DrawFifty) {
                    MainClass.CurrentGameStatus = GameStatus.DrawFifty;
                } else if (MainClass.CurrentGameHistory.CheckThreefoldRepetition() == GameStatus.DrawRepetition) {
                    MainClass.CurrentGameStatus = GameStatus.DrawRepetition;
                }

                Gtk.Application.Invoke(delegate {
                    RedrawBoard();
                });
            } catch(InvalidOperationException) {
                throw new InvalidOperationException (move);
            }
            GameStatus isMate = MainClass.CurrentBoard.CheckForMate ();
            if (isMate != GameStatus.Active) {
                MainClass.CurrentGameStatus = isMate;
            }
            if (MainClass.CurrentGameStatus != GameStatus.Active && MainClass.CurrentGameStatus != GameStatus.Inactive) {
                Gtk.Application.Invoke(delegate {
                    ShowGameOverDialog(MainClass.CurrentGameStatus);
                });
            }
            Gtk.Application.Invoke (delegate {
                MainClass.UpdateClock ();
                UpdatePlayerToMove();
            });

            if (MainClass.CurrentMode == GameMode.Engines) {
                Thread.Sleep (1500);
                if (engine == 1) {
                    Gtk.Application.Invoke (delegate {
                        EngineTwoMove ();
                    });
                } else {
                    Gtk.Application.Invoke (delegate {
                        EngineOneMove ();
                    });
                }
            }
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

        public void LogEngineOutput(string output, int engine)
        {
            Match match = engineOutputRegex.Match (output);
            if (match.Success) {
                if (engine == 1) {
                    EngineOneDepthLabel.Text = "Depth: " + match.Groups [1].Value.Substring (5);
                    EngineOneNPSLabel.Text = "NPS: " + match.Groups [2].Value.Substring (3);
                } else {
                    EngineTwoDepthLabel.Text = "Depth: " + match.Groups [1].Value.Substring (5);
                    EngineTwoNPSLabel.Text = "NPS: " + match.Groups [2].Value.Substring (3);
                }
                string score = match.Groups [3].Value.Substring (6);
                if (score.StartsWith ("cp")) {
                    score = score.Substring (3);
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
                } else if (score.StartsWith ("mate")) {
                    score = score.Substring (5);
                    if (score.StartsWith ("-"))
                        score = score.Insert (1, "M");
                    else
                        score = score.Insert (0, "M");
                }
                string pv = match.Groups [4].Value.Substring (2);
                TextIter iter = EngineOutput.GetIterAtLocation (0, 0);
                EngineOutput.Buffer.Insert (ref iter, score + " " + pv + Environment.NewLine);
            }
        }

        public void LogEngineNameAndAuthor(int engine, string name, string author)
        {
            if (engine == 1) {
                EngineOneNameLabel.Text = name;
                EngineOneAuthorLabel.Text = author;
            } else {
                EngineTwoNameLabel.Text = name;
                EngineTwoAuthorLabel.Text = author;
            }
            ClearEngineOutput ();
            EngineOutput.Buffer.Text = "Engine loaded: " + name;
        }

        public void ClearEngineInfo()
        {
            EngineOneNameLabel.Text = "Engine 1 not loaded.";
            EngineOneAuthorLabel.Text = "";
            EngineOneDepthLabel.Text = "";
            EngineOneNPSLabel.Text = "";
            
            EngineTwoNameLabel.Text = "Engine 2 not loaded.";
            EngineTwoAuthorLabel.Text = "";
            EngineTwoDepthLabel.Text = "";
            EngineTwoNPSLabel.Text = "";
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
                        pieceIndex = MainClass.BoardOrientation == PieceColour.White ? i : Math.Abs (i - 63);
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
                if (MainClass.EngineOne != null && MainClass.EngineOne.IsThinking) {
                    MainClass.CancelEngineTask ();
                    MainClass.EngineOne.StopAndIgnoreMove ();
                }
                if (MainClass.EngineTwo != null && MainClass.EngineOne.IsThinking) {
                    MainClass.CancelEngineTask ();
                    MainClass.EngineTwo.StopAndIgnoreMove ();
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
                    SpecifierType specifierRequired = GameHistory.checkDisabiguationNeeded(MainClass.CurrentBoard, selectedPiece, (byte)pieceIndex);
                    MoveResult result = MainClass.CurrentBoard.MakeMove (selectedPiece, (byte)pieceIndex, promoteTo);

                    Piece movingPiece = null;
                    if(promoteTo == null) {
                        movingPiece = MainClass.CurrentBoard.Squares[(byte)pieceIndex].Piece;
                    } else {
                        movingPiece = new Piece(MainClass.CurrentBoard.Squares [(byte)pieceIndex].Piece.Colour, PieceType.Pawn);
                    }

                    if(result == MoveResult.Capture && movingPiece.Type == PieceType.Pawn) {
                        specifierRequired = SpecifierType.File;
                    }

                    string fenPosition = MainClass.CurrentBoard.ToFEN().Split(' ')[0];
                    MainClass.CurrentGameHistory.AddMove(new Move(selectedPiece, (byte)pieceIndex,
                        MainClass.CurrentBoard.Squares [(byte)pieceIndex].Piece.Colour,
                        movingPiece,
                        result,
                        MainClass.CurrentBoard.ToFEN(),
                        specifierRequired,
                        promoteTo), fenPosition);
                    UpdateGameHistoryView();

                    if (MainClass.CurrentGameHistory.UpdateFiftyMoveCount (result) == GameStatus.DrawFifty) {
                        MainClass.CurrentGameStatus = GameStatus.DrawFifty;
                    } else if (MainClass.CurrentGameHistory.CheckThreefoldRepetition() == GameStatus.DrawRepetition) {
                        MainClass.CurrentGameStatus = GameStatus.DrawRepetition;
                    }
                } catch(InvalidOperationException) {
                    Debug.Log ("Invalid move entered.");
                }
                Gtk.Application.Invoke(delegate {
                    RedrawBoard();
                });
                GameStatus isMate = MainClass.CurrentBoard.CheckForMate ();
                if (isMate != GameStatus.Active) {
                    MainClass.CurrentGameStatus = isMate;
                }
                if (MainClass.CurrentGameStatus != GameStatus.Active && MainClass.CurrentGameStatus != GameStatus.Inactive) {
                    ShowGameOverDialog (MainClass.CurrentGameStatus);
                }
                Gtk.Application.Invoke (delegate {
                    MainClass.UpdateClock ();
                    UpdatePlayerToMove();
                });

                currentSelectionState = PieceSelectionState.None;

                if (MainClass.EngineOne != null && MainClass.CurrentMode == GameMode.OnePlayer)
                    EngineOneMove ();
            }
        }

        /**
         * @fn UpdateMaterialDifference
         * @brief Updates the material difference counter in the sidebar.
         */
        public void UpdateMaterialDifference(Board board)
        {
            int totalWhite = 0;
            int totalBlack = 0;

            int materialDifference = 0;
            foreach (Square sq in board.Squares) {
                if (sq.Piece != null) {
                    if (sq.Piece.Type != PieceType.King && sq.Piece.Colour == PieceColour.White) {
                        totalWhite += pieceValues [(int)(sq.Piece.Type)];
                        materialDifference += pieceValues [(int)(sq.Piece.Type)];
                    } else if (sq.Piece.Type != PieceType.King && sq.Piece.Colour == PieceColour.Black) {
                        totalBlack += pieceValues [(int)(sq.Piece.Type)];
                        materialDifference -= pieceValues [(int)(sq.Piece.Type)];
                    }
                }
            }

            if (materialDifference < 0) {
                MaterialDifferenceLabel.Text = String.Format ("Black is {0} pawn{1} up.",
                    Math.Abs (materialDifference), materialDifference < -1 ? "s" : "");
            } else if (materialDifference > 0) {
                MaterialDifferenceLabel.Text = String.Format ("White is {0} pawn{1} up.",
                    Math.Abs (materialDifference), materialDifference > 1 ? "s" : "");
            } else {
                MaterialDifferenceLabel.Text = "Material is equal.";
            }

            int maxMaterial = Math.Max (totalWhite, totalBlack);
            if (maxMaterial <= 3) { // We compare to 3 because kings are not counted.
                int minMaterial = Math.Min (totalWhite, totalBlack);
                if (minMaterial == 0) {
                    MainClass.CurrentGameStatus = GameStatus.DrawInsuffientMaterial;
                } else if (maxMaterial == minMaterial && maxMaterial == 3) {   // Now we check for opposite-coloured bishops
                    int whiteBishopSquare = -1;
                    int blackBishopSquare = -1;
                    for(int i = 0; i < board.Squares.Length; i++) {
                        Square sq = board.Squares [i];
                        if (sq.Piece != null && sq.Piece.Type == PieceType.Bishop) {
                            if (sq.Piece.Colour == PieceColour.White)
                                whiteBishopSquare = i;
                            else
                                blackBishopSquare = i;
                        }
                    }
                    // Only continue if both pieces are actually bishops
                    if (whiteBishopSquare != -1 && blackBishopSquare != -1) {
                        // Now we check to see if they are on opposite colours, using XOR
                        bool whiteBishopOnWhiteSquare = Array.IndexOf (whiteSquares, (byte)whiteBishopSquare) > -1;
                        bool blackBishopOnWhiteSquare = Array.IndexOf (whiteSquares, (byte)blackBishopSquare) > -1;
                        if (whiteBishopOnWhiteSquare ^ blackBishopOnWhiteSquare) {
                            MainClass.CurrentGameStatus = GameStatus.DrawInsuffientMaterial;
                        }
                    }
                }
            }
            if (MainClass.CurrentGameStatus != GameStatus.Active && MainClass.CurrentGameStatus != GameStatus.Inactive) {
                Gtk.Application.Invoke(delegate {
                    ShowGameOverDialog(MainClass.CurrentGameStatus);
                });
            }
        }

        protected void OnSetEngineStrength (object sender, EventArgs e)
        {
            EngineStrengthDialog dialog = new EngineStrengthDialog ();

            if (dialog.Run() == (int)ResponseType.Ok) {
                MainClass.StrengthType = dialog.Measure;
                MainClass.StrengthValue = dialog.Value;
                Debug.Log ("New engine strength: " + dialog.Measure + " " + dialog.Value);
            }
            dialog.Destroy ();
        }

        protected void OnOnePlayerSet (object sender, EventArgs e)
        {
            if (MainClass.EngineOne != null && MainClass.EngineOne.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.EngineOne.StopAndIgnoreMove ();
            }
            if (MainClass.EngineOne != null && MainClass.EngineTwo.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.EngineTwo.StopAndIgnoreMove ();
            }
            MainClass.CurrentMode = GameMode.OnePlayer;
        }

        protected void OnTwoPlayerSet (object sender, EventArgs e)
        {
            if (MainClass.EngineOne != null && MainClass.EngineOne.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.EngineOne.StopAndIgnoreMove ();
            }
            if (MainClass.EngineOne != null && MainClass.EngineTwo.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.EngineTwo.StopAndIgnoreMove ();
            }
            MainClass.CurrentMode = GameMode.TwoPlayer;
        }

        protected void OnEnginesSet (object sender, EventArgs e)
        {
            if (MainClass.EngineOne == null || MainClass.EngineTwo == null) {
                OnePlayerAction.Activate ();

                Console.Error.WriteLine ("(EE) Not enough engines loaded.");
                MessageDialog errorDialog = new MessageDialog (
                                                this,
                                                DialogFlags.DestroyWithParent,
                                                MessageType.Error,
                                                ButtonsType.Ok,
                                                "Two engines need to be loaded!");
                errorDialog.Run ();
                errorDialog.Destroy ();

                return;
            }
            MainClass.CurrentMode = GameMode.Engines;
        }

        protected void OnAnalyseMove (object sender, EventArgs e)
        {
            if ((DateTime.Now - engineThinkCooldown).TotalMilliseconds < 500)
                // Force a cooldown of 0.5 seconds between requests for the engine to think.
                return;

            if (MainClass.EngineOne == null) {
                Console.Error.WriteLine ("(EE) Engine not loaded.");
                Gtk.Application.Invoke (delegate {
                    MessageDialog errorDialog = new MessageDialog (
                                                    this,
                                                    DialogFlags.DestroyWithParent,
                                                    MessageType.Error,
                                                    ButtonsType.Ok,
                                                    "Please load an engine first.");
                    errorDialog.Run ();
                    errorDialog.Destroy ();
                });
                return;
            }
            if (MainClass.EngineOne.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.EngineOne.StopAndIgnoreMove ();
            }
            if (MainClass.EngineTwo != null && MainClass.EngineOne.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.EngineTwo.StopAndIgnoreMove ();
            }

            string currentFEN = MainClass.CurrentBoard.ToFEN ();
            MainClass.EngineOne.SendPosition (currentFEN);
            MainClass.EngineOne.WaitUntilReady ();
            engineThinkTask = Task.Factory.StartNew<string> (
                                     () => MainClass.EngineOne.Go ("infinite"),
                                     MainClass.EngineStopTokenSource.Token
                                 );

            engineThinkCooldown = DateTime.Now;
        }

        public void UpdatePlayerToMove()
        {
            if (MainClass.CurrentGameStatus == GameStatus.Active) {
                if (MainClass.CurrentBoard.PlayerToMove == PieceColour.White) {
                    PlayerToMoveLabel.Text = "Player to move: White";
                } else {
                    PlayerToMoveLabel.Text = "Player to move: Black";
                }
            } else {
                PlayerToMoveLabel.Text = "The game is not active.";
            }
        }

        protected void OnLoadEngineTwo (object sender, EventArgs e)
        {
            if (MainClass.EngineTwo != null && MainClass.EngineTwo.IsThinking) {
                MainClass.CancelEngineTask ();
                MainClass.EngineTwo.StopAndIgnoreMove ();
            }
            FileChooserDialog chooser = new FileChooserDialog (
                                            "Please choose an engine executable.",
                                            this,
                                            FileChooserAction.Open,
                                            "Cancel", ResponseType.Cancel,
                                            "Open", ResponseType.Accept);

            if (chooser.Run() == (int)ResponseType.Accept) {
                try {
                    MainClass.EngineTwo = new UCITransceiver(chooser.Filename, 2);
                    MainClass.EngineTwo.Init();
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

        protected void OnExportPGN (object sender, EventArgs e)
        {
            var fc = new FileChooserDialog ("Choose where to save the PGN",
                         this,
                         FileChooserAction.Save,
                         "Cancel", ResponseType.Cancel,
                         "Save", ResponseType.Accept);

            if (fc.Run () == (int)ResponseType.Accept) {
                MainClass.CurrentGameHistory.SavePGN (fc.Filename);
            }
            fc.Destroy ();
        }

        private void UpdateGameHistoryView()
        {
            Move move = MainClass.CurrentGameHistory.GetLastMove ();
            string moveOutput = "";
            if (move.Colour == PieceColour.White) {
                moveOutput += " " + MainClass.CurrentGameHistory.MoveCount + ". ";
            }
            moveOutput += GameHistory.MoveToNotation (move) + " ";
            GameHistoryView.Buffer.Text += moveOutput;
        }

        private void ClearGameHistoryView()
        {
            GameHistoryView.Buffer.Text = "";
        }

        protected void OnImportPGN (object sender, EventArgs e)
        {
            var fc = new FileChooserDialog ("Choose a PGN file to open.",
                                            this,
                                            FileChooserAction.Open,
                                            "Cancel", ResponseType.Cancel,
                                            "Open", ResponseType.Accept);
            if (fc.Run () == (int)ResponseType.Accept) {
                MainClass.CurrentGameHistory = GameHistory.importPGN (File.ReadAllText (fc.Filename));
                string pgn = MainClass.CurrentGameHistory.ToPGNString ();
                int indexOfMovesStart = pgn.IndexOf ("1.");
                if (indexOfMovesStart > 0) {
                    GameHistoryView.Buffer.Text = pgn.Substring (indexOfMovesStart);
                }

                // Load the FEN from the last move
                FENParser parser = new FENParser(MainClass.CurrentGameHistory.GetLastMove().FEN);
                MainClass.CurrentBoard = parser.GetBoard();
                MainClass.CurrentGameStatus = GameStatus.Inactive;
                PiecePseudoLegalMoves.GeneratePseudoLegalMoves(MainClass.CurrentBoard);
                PieceLegalMoves.GenerateLegalMoves(MainClass.CurrentBoard);
                RedrawBoard();

                MainClass.CurrentGameStatus = GameStatus.Inactive;
                GameStatus currentStatus = MainClass.CurrentBoard.CheckForMate ();
                if (currentStatus != GameStatus.Inactive && currentStatus != GameStatus.Active) {
                    ShowGameOverDialog (currentStatus);
                }
                MainClass.ResetClock ();
                UpdateMaterialDifference (MainClass.CurrentBoard);
            }
            fc.Destroy ();
        }
    }
}
