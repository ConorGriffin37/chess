using System;

namespace GUI
{
    public partial class PawnPromotionDialog : Gtk.Dialog
    {
        public PieceType PromoteTo { get; private set; }

        public PawnPromotionDialog ()
        {
            this.Build ();
        }

        protected override void OnResponse (Gtk.ResponseType response_id)
        {
            foreach (Gtk.RadioButton button in QueenButton.Group) {
                if (button.Active) {
                    switch (button.Name) {
                        case "QueenButton":
                            PromoteTo = PieceType.Queen;
                            break;
                        case "RookButton":
                            PromoteTo = PieceType.Rook;
                            break;
                        case "BishopButton":
                            PromoteTo = PieceType.Bishop;
                            break;
                        case "KnightButton":
                            PromoteTo = PieceType.Knight;
                            break;
                    }
                }
            }
        }
    }
}

