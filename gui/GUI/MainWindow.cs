using System;
using Gtk;
using Cairo;

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
            Application.Quit ();
            a.RetVal = true;
        }

        protected void OnQuit (object sender, EventArgs e)
        {
            Application.Quit ();
        }

        protected void OnLoadFEN (object sender, EventArgs e)
        {
            LoadFENDialog fen = new LoadFENDialog();
            if (fen.Run () == (int)ResponseType.Ok) {
                try {
                    FENParser parser = new FENParser(fen.FENString);
                    MainClass.CurrentBoard = parser.GetBoard();
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
        }

        protected void OnLoadEngine (object sender, EventArgs e)
        {
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
            throw new NotImplementedException();
        }

        protected void OnBoardExpose (object o, ExposeEventArgs args)
        {
            RedrawBoard ();
        }

        void RedrawBoard()
        {
            boardContext = Gdk.CairoHelper.Create (BoardArea.GdkWindow);
            double transx = Math.Abs((this.Allocation.Width - (boardBackground.Width * 0.75))) / 2;
            boardContext.Translate (transx, 0);
            boardContext.Scale (0.75, 0.75);
            boardBackground.Show (boardContext, 0, 0);
            PieceDisplay.DrawPieces (boardContext);
        }
    }
}
