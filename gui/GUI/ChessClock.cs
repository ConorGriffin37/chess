using System;
using System.Timers;

namespace GUI
{
    public class ChessClock
    {
        private Timer timer;
        public TimeSpan TimeLeft { get; private set; }
        public PieceColour Colour { get; private set; }
        private readonly TimeSpan second;

        public ChessClock (PieceColour colour, TimeSpan span)
        {
            TimeLeft = span;
            timer = new Timer (1000);
            this.Colour = colour;
            second = new TimeSpan (0, 0, 1);
            if (MainClass.win != null)
                MainClass.win.UpdateClock (this);
        }

        public bool Start()
        {
            if (timer.Enabled)
                return false;

            timer.Elapsed += OnTimedEvent;
            timer.Enabled = true;
            Debug.Log ("Clock started: " + Colour.ToString ());
            return true;
        }

        public bool Stop()
        {
            if (!timer.Enabled)
                return false;

            timer.Elapsed -= OnTimedEvent;
            timer.Enabled = false;
            Debug.Log ("Clock stopped: " + Colour.ToString ());
            return true;
        }

        private void OnTimedEvent(Object source, ElapsedEventArgs e)
        {
            TimeLeft -= second;
            if (TimeLeft == new TimeSpan (0)) {
                this.Stop ();
                if (Colour == PieceColour.White) {
                    Gtk.Application.Invoke(delegate {
                        MainClass.win.ShowGameOverDialog (GameStatus.WhiteTime);
                    });
                } else {
                    Gtk.Application.Invoke(delegate {
                        MainClass.win.ShowGameOverDialog (GameStatus.WhiteTime);
                    });
                }
            }
            MainClass.win.UpdateClock (this);
        }
    }
}

