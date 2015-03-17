using System;
using Gtk;

namespace GUI
{
    public partial class SetClockTimeDialog : Gtk.Dialog
    {
        public string Time { get; private set; }

        public SetClockTimeDialog ()
        {
            this.Build ();
        }

        protected override void OnResponse (Gtk.ResponseType response_id)
        {
            if(HoursEntry.Text == "") HoursEntry.Text = "0";
            if(MinutesEntry.Text == "") MinutesEntry.Text = "0";
            if(SecondsEntry.Text == "") SecondsEntry.Text = "0";

            Time = HoursEntry.Text + ":" + MinutesEntry.Text + ":" + SecondsEntry.Text;
        }
    }
}

