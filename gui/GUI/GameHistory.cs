using System;
using System.Collections.Generic;
using System.IO;

namespace GUI
{
    public struct Move
    {
        public byte Source { get; set; }
        public byte Destination { get; set; }
        public PieceColour Colour { get; set; }
        public Piece MovingPiece { get; set; }
        public bool SpecifierRequired { get; set; } // Whether or not to specify the source square in the short algebraic notation
        public PieceType? PromoteTo { get; set; }

        public Move(byte source, byte destination, PieceColour colour, Piece movingPiece,
                    bool specifierRequired = false, PieceType? promoteTo = null)
        {
            Source = source;
            Destination = destination;
            Colour = colour;
            MovingPiece = movingPiece;
            SpecifierRequired = specifierRequired;
            PromoteTo = promoteTo;
        }
    }

    public class GameHistory
    {
        private List<Move> History { get; private set; }

        // PGN Metadata
        public string Event { get; private set; }
        public string Site { get; private set; }
        public string Date { get; private set; }
        public string Round { get; private set; }
        public string White { get; private set; }
        public string Black { get; private set; }
        public string Result { get; private set; }

        private string[] columns = { "a", "b", "c", "d", "e", "f", "g", "h" };

        public GameHistory ()
        {
            History = new List<Move> ();

            Event = "Casual game.";
            Site = System.Environment.MachineName;
            Date = DateTime.Now.ToString("yyyy\.mm\.dd");
            Round = "1";
            White = "Unknown";
            Black = "Unknown";
            Result = "*";
        }

        private string SquareToNotation(byte square)
        {
            string column = columns [square % 8];
            string row = Convert.ToString ((int)(square / 8));
            return column + row;
        }

        private string PieceToNotation(Piece piece)
        {
            string pieceNotation = "";
            switch (piece.Type) {
                case PieceType.Pawn:
                    pieceNotation = "";
                case PieceType.Knight:
                    pieceNotation = "n";
                case PieceType.Bishop:
                    pieceNotation = "b";
                case PieceType.Rook:
                    pieceNotation = "r";
                case PieceType.King:
                    pieceNotation = "k";
                case PieceType.Queen:
                    pieceNotation = "q";
            }
            if(piece.Colour == PieceColour.Black) {
                pieceNotation = pieceNotation.ToUpper ();
            }
        }

        public void AddMove(Move move)
        {
            History.Add (move);
        }

        public void UndoLastMove()
        {
            History.RemoveAt (History.Count - 1);
        }

        public bool SavePGN(string filename)
        {
            StreamWriter PGNFile = new StreamWriter (filename);
            // First, create the PGN headers
            PGNFile.WriteLine ("[Event \"" + Event + "\"]");
            PGNFile.WriteLine ("[Site \"" + Site + "\"]");
            PGNFile.WriteLine ("[Date \"" + Date + "\"]");
            PGNFile.WriteLine ("[Round \"" + Round + "\"]");
            PGNFile.WriteLine ("[White \"" + White + "\"]");
            PGNFile.WriteLine ("[Black \"" + Black + "\"]");
            PGNFile.WriteLine ("[Result \"" + Result + "\"]");

            // Now we can print the moves
            for (int i = 0; i < History.Count / 2; i++) {
                string moveOutput = i + ". ";
                string whiteMove = "";
                string blackMove = "";
                if (History [i].Colour == PieceColour.Black) {
                    whiteMove = "...";

                    string blackMovePiece = PieceToNotation (History [i].MovingPiece);
                    string blackMoveSpecifier = History [i].SpecifierRequired ?
                        SquareToNotation (History [i].Source) [0] :
                        "";
                    string blackMoveSquare = SquareToNotation (History [i].Destination);
                    blackMove = blackMovePiece + blackMoveSpecifier + blackMoveSquare;
                } else {
                    string whiteMovePiece = PieceToNotation (History [i].MovingPiece);
                    string whiteMoveSpecifier = History [i].SpecifierRequired ?
                        SquareToNotation (History [i].Source) [0] :
                        "";
                    string whiteMoveSquare = SquareToNotation (History [i + 1].Destination);
                    whiteMove = whiteMovePiece + whiteMoveSpecifier + whiteMoveSquare;

                    string blackMovePiece = PieceToNotation (History [i + 1].MovingPiece);
                    string blackMoveSpecifier = History [i + 1].SpecifierRequired ?
                        SquareToNotation (History [i + 1].Source) [0] :
                        "";
                    string blackMoveSquare = SquareToNotation (History [i + 1].Destination);
                    blackMove = blackMovePiece + blackMoveSpecifier + blackMoveSquare;
                }
            }
        }
    }
}

