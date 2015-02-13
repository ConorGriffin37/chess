using System;
using Gtk;
using Cairo;

namespace GUI
{
    public partial class MainWindow: Gtk.Window
    {
        public MainWindow () : base (Gtk.WindowType.Toplevel)
        {
            Build ();
            InitBoard ();
        }

        private void InitBoard()
        {
            int height = BoardArea.Allocation.Height;
            int width = BoardArea.Allocation.Width;

            Gdk.Pixbuf boardPixbuf = new Gdk.Pixbuf ("img/board.png");
            Cairo.Context cr = Gdk.CairoHelper.Create (BoardArea.GdkWindow);
            cr.Translate (0, 0);
            boardPixbuf.RenderToDrawable (BoardArea.GdkWindow,
                BoardArea.Style.BackgroundGC (StateType.Normal),
                0, 0,
                0, 0,
                -1, -1,
                Gdk.RgbDither.None,
                0, 0);
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
    }
}
