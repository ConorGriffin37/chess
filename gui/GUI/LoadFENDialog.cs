using System;

namespace GUI
{
    public partial class LoadFENDialog : Gtk.Dialog
    {
        public string FENString { get; private set; }

        public LoadFENDialog ()
        {
            this.Build ();
        }

        protected override void OnResponse (Gtk.ResponseType response_id)
        {
            FENString = FENEntry.Text;
        }
    }
}

