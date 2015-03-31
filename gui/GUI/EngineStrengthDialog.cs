using System;

namespace GUI
{
    public enum StrengthMeasure { Depth, Time };

    public partial class EngineStrengthDialog : Gtk.Dialog
    {
        public StrengthMeasure Measure { get; private set; }
        public int Value { get; private set; }

        public EngineStrengthDialog ()
        {
            this.Build ();
        }

        protected override void OnResponse (Gtk.ResponseType response_id)
        {
            if (DepthButton.Active) {
                Measure = StrengthMeasure.Depth;
            } else {
                Measure = StrengthMeasure.Time;
            }

            Value = (int)StrengthValue.Value;
        }
    }
}

