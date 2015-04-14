using System;
using System.Collections.Generic;
using System.IO;

namespace GUI
{
    public class Move
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
        private List<Move> history;

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
            history = new List<Move> ();

            Event = "Casual game.";
            Site = System.Environment.MachineName;
            Date = DateTime.Now.ToString("yyyy.MM.dd");
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
                    break;
                case PieceType.Knight:
                    pieceNotation = "n";
                    break;
                case PieceType.Bishop:
                    pieceNotation = "b";
                    break;
                case PieceType.Rook:
                    pieceNotation = "r";
                    break;
                case PieceType.King:
                    pieceNotation = "k";
                    break;
                case PieceType.Queen:
                    pieceNotation = "q";
                    break;
                default:
                    break;
            }
            if(piece.Colour == PieceColour.Black) {
                pieceNotation = pieceNotation.ToUpper ();
            }
            return pieceNotation;
        }

        public void AddMove(Move move)
        {
            history.Add (move);
        }

        public void UndoLastMove()
        {
            history.RemoveAt (history.Count - 1);
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
            for (int i = 0; i < history.Count / 2; i++) {
                string moveOutput = i + ". ";
                string whiteMove = "";
                string blackMove = "";
                if (history [i].Colour == PieceColour.Black) {
                    whiteMove = "...";

                    string blackMovePiece = PieceToNotation (history [i].MovingPiece);
                    string blackMoveSpecifier = "";
                    if (history [i].SpecifierRequired) {
                        blackMoveSpecifier = SquareToNotation (history [i].Source).Substring (0, 1);
                    }
                    string blackMoveSquare = SquareToNotation (history [i].Destination);
                    blackMove = blackMovePiece + blackMoveSpecifier + blackMoveSquare;
                } else {
                    string whiteMovePiece = PieceToNotation (history [i].MovingPiece);
                    string whiteMoveSpecifier = "";
                    if (history [i].SpecifierRequired) {
                        whiteMoveSpecifier = SquareToNotation (history [i].Source).Substring (0, 1);
                    }
                    string whiteMoveSquare = SquareToNotation (history [i + 1].Destination);
                    whiteMove = whiteMovePiece + whiteMoveSpecifier + whiteMoveSquare;

                    string blackMovePiece = PieceToNotation (history [i + 1].MovingPiece);
                    string blackMoveSpecifier = "";
                    if (history [i].SpecifierRequired) {
                        blackMoveSpecifier = SquareToNotation (history [i].Source).Substring (0, 1);
                    }
                    string blackMoveSquare = SquareToNotation (history [i + 1].Destination);
                    blackMove = blackMovePiece + blackMoveSpecifier + blackMoveSquare;
                }

                moveOutput = moveOutput + whiteMove + " " + blackMove + " ";
                try {
                    PGNFile.Write (moveOutput);
                } catch(Exception ex) {
                    Console.Error.WriteLine ("(EE) Error writing to file " + filename + ": " + ex.Message);
                    return false;
                }
            }
            return true;
        }
    }
}

