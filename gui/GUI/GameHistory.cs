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
        public MoveResult Result { get; set; }

        public Move(byte source, byte destination, PieceColour colour, Piece movingPiece,
                    MoveResult result, bool specifierRequired = false, PieceType? promoteTo = null)
        {
            Source = source;
            Destination = destination;
            Colour = colour;
            MovingPiece = movingPiece;
            SpecifierRequired = specifierRequired;
            PromoteTo = promoteTo;
            Result = result;
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

        private static string[] columns = { "a", "b", "c", "d", "e", "f", "g", "h" };

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

        private static string SquareToNotation(byte square)
        {
            string column = columns [square % 8];
            string row = Convert.ToString (Math.Abs((int)(square / 8) - 7) + 1);
            return column + row;
        }

        private static string PieceToNotation(Piece piece)
        {
            string pieceNotation = "";
            switch (piece.Type) {
                case PieceType.Pawn:
                    pieceNotation = "";
                    break;
                case PieceType.Knight:
                    pieceNotation = "N";
                    break;
                case PieceType.Bishop:
                    pieceNotation = "B";
                    break;
                case PieceType.Rook:
                    pieceNotation = "R";
                    break;
                case PieceType.King:
                    pieceNotation = "K";
                    break;
                case PieceType.Queen:
                    pieceNotation = "Q";
                    break;
                default:
                    break;
            }

            return pieceNotation;
        }

        private static string PromoteToNotation(PieceType? promoteTo)
        {
            switch (promoteTo) {
                case null:
                    return null;
                case PieceType.Knight:
                    return "N";
                case PieceType.Bishop:
                    return "B";
                case PieceType.Rook:
                    return "R";
                case PieceType.Queen:
                    return "Q";
                default:
                    return null;
            }
        }

        private static string GetSpecifier(Move move)
        {
            return columns [move.Source % 8];
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
            PGNFile.WriteLine ();

            // Now we can print the moves
            for (int i = 0; i < history.Count; i += 2) {
                string moveOutput = (i + 1) + ". ";
                string whiteMove = "";
                string blackMove = "";

                if (history[i].Result == MoveResult.KingsideCastle) {
                    moveOutput += "O-O";
                } else if (history[i].Result == MoveResult.QueensideCastle) {
                    moveOutput += "O-O-O";
                } else if (history [i].Colour == PieceColour.Black) {
                    whiteMove = "...";

                    string blackMovePiece = PieceToNotation (history [i].MovingPiece);
                    string blackMoveSpecifier = "";
                    if (history [i].SpecifierRequired) {
                        blackMoveSpecifier = GetSpecifier(history[i]);
                    }
                    string blackCaptureSpecifier = "";
                    if (history[i].Result == MoveResult.Capture) {
                        blackCaptureSpecifier = "x";
                    }
                    string blackMoveSquare = SquareToNotation (history [i].Destination);
                    string blackPromoteTo = PromoteToNotation(history[i].PromoteTo);
                    blackMove = blackMovePiece + blackMoveSpecifier + blackCaptureSpecifier + blackMoveSquare + blackPromoteTo;
                } else {
                    string whiteMovePiece = PieceToNotation (history [i].MovingPiece);
                    string whiteMoveSpecifier = "";
                    if (history [i].SpecifierRequired) {
                        whiteMoveSpecifier = GetSpecifier(history[i]);
                    }
                    string whiteCaptureSpecifier = "";
                    if (history[i].Result == MoveResult.Capture) {
                        whiteCaptureSpecifier = "x";
                    }
                    string whiteMoveSquare = SquareToNotation (history [i].Destination);
                    string whitePromoteTo = PromoteToNotation(history[i].PromoteTo);
                    whiteMove = whiteMovePiece + whiteMoveSpecifier + whiteCaptureSpecifier + whiteMoveSquare + whitePromoteTo;

                    string blackMovePiece = PieceToNotation (history [i + 1].MovingPiece);
                    string blackMoveSpecifier = "";
                    if (history [i + 1].SpecifierRequired) {
                        blackMoveSpecifier = GetSpecifier(history[i + 1]);
                    }
                    string blackCaptureSpecifier = "";
                    if (history[i + 1].Result == MoveResult.Capture) {
                        blackCaptureSpecifier = "x";
                    }
                    string blackMoveSquare = SquareToNotation (history [i + 1].Destination);
                    string blackPromoteTo = PromoteToNotation(history[i + 1].PromoteTo);
                    blackMove = blackMovePiece + blackMoveSpecifier + blackCaptureSpecifier + blackMoveSquare + blackPromoteTo;
                }

                moveOutput = moveOutput + whiteMove + " " + blackMove + " ";
                try {
                    PGNFile.Write (moveOutput);
                } catch(Exception ex) {
                    Console.Error.WriteLine ("(EE) Error writing to file " + filename + ": " + ex.Message);
                    return false;
                }
            }
            PGNFile.Close ();
            return true;
        }
    }
}

