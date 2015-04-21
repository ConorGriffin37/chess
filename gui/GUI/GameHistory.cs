using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace GUI
{
    public enum SpecifierType { None, File, Rank, Both }

    public class Move
    {
        public byte Source { get; set; }
        public byte Destination { get; set; }
        public PieceColour Colour { get; set; }
        public Piece MovingPiece { get; set; }
        public SpecifierType SpecifierRequired { get; set; } // Whether or not to specify the source square in the short algebraic notation
        public PieceType? PromoteTo { get; set; }
        public MoveResult Result { get; set; }

        public Move(byte source, byte destination, PieceColour colour, Piece movingPiece,
                    MoveResult result, SpecifierType specifierRequired = SpecifierType.None, PieceType? promoteTo = null)
        {
            Source = source;
            Destination = destination;
            Colour = colour;
            MovingPiece = movingPiece;
            SpecifierRequired = specifierRequired;
            PromoteTo = promoteTo;
            Result = result;
        }

        public Move(Move other)
        {
            Source = other.Source;
            Destination = other.Destination;
            Colour = other.Colour;
            MovingPiece = other.MovingPiece;
            SpecifierRequired = other.SpecifierRequired;
            PromoteTo = other.PromoteTo;
            Result = other.Result;
        }
    }

    public class GameHistory
    {
        private List<Move> history;
        private List<string> positions;

        // PGN Metadata
        public string Event { get; private set; }
        public string Site { get; private set; }
        public string Date { get; private set; }
        public string Round { get; private set; }
        public string White { get; private set; }
        public string Black { get; private set; }
        public string Result { get; private set; }

        public int FiftyMoveRuleCount { get; private set; }
        public int MoveCount { get; private set; }

        private static string[] columns = { "a", "b", "c", "d", "e", "f", "g", "h" };

        public GameHistory ()
        {
            history = new List<Move> ();
            positions = new List<string> ();

            Event = "Casual game.";
            Site = System.Environment.MachineName;
            Date = DateTime.Now.ToString("yyyy.MM.dd");
            Round = "1";
            White = "Unknown";
            Black = "Unknown";
            Result = "*";

            FiftyMoveRuleCount = 0;
            MoveCount = 1;
        }

        public static string MoveToNotation(Move move)
        {
            string movePiece = PieceToNotation (move.MovingPiece);
            string moveSpecifier = "";
            if (move.SpecifierRequired != SpecifierType.None) {
                moveSpecifier = GetSpecifier(move);
            }
            string captureSpecifier = "";
            if (move.Result == MoveResult.Capture) {
                captureSpecifier = "x";
            }
            string moveSquare = SquareToNotation (move.Destination);
            string promoteTo = PromoteToNotation(move.PromoteTo);
            string moveString = movePiece + moveSpecifier + captureSpecifier + moveSquare + promoteTo;

            return moveString;
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
            switch (move.SpecifierRequired) {
                case SpecifierType.File:
                    return columns [move.Source % 8];
                case SpecifierType.Rank:
                    return Convert.ToString (Math.Abs((int)(move.Source / 8) - 7) + 1);
                case SpecifierType.Both:
                    return SquareToNotation (move.Source);
                default:
                    break;
            }
            return "";
        }

        public void AddMove(Move move, string fen)
        {
            history.Add (move);
            positions.Add (fen);
            if (move.Colour == PieceColour.Black) {
                MoveCount++;
            }
        }

        public void UndoLastMove()
        {
            history.RemoveAt (history.Count - 1);
        }

        public Move GetLastMove()
        {
            return new Move (history [history.Count - 1]);
        }

        public GameStatus? UpdateFiftyMoveCount(MoveResult result)
        {
            if (result == MoveResult.PawnMove || result == MoveResult.Capture) {
                FiftyMoveRuleCount = 0;
            } else {
                FiftyMoveRuleCount++;
            }
            if (FiftyMoveRuleCount >= 100) {
                return GameStatus.DrawFifty;
            }
            return null;
        }

        public GameStatus? CheckThreefoldRepetition()
        {
            Dictionary<string, int> counts = positions.GroupBy (x => x)
                                                      .ToDictionary (g => g.Key,
                                                                     g => g.Count ());
            if (counts.ContainsValue (3)) {
                return GameStatus.DrawRepetition;
            }
            return null;
        }

        public string ToPGNString()
        {
            string output = "";

            // First, create the PGN headers
            output += ("[Event \"" + Event + "\"]\n");
            output += ("[Site \"" + Site + "\"]\n");
            output += ("[Date \"" + Date + "\"]\n");
            output += ("[Round \"" + Round + "\"]\n");
            output += ("[White \"" + White + "\"]\n");
            output += ("[Black \"" + Black + "\"]\n");
            output += ("[Result \"" + Result + "\"]\n");

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
                    blackMove = MoveToNotation (history [i]);
                } else {
                    whiteMove = MoveToNotation (history [i]);
                    blackMove = MoveToNotation (history [i + 1]);
                }

                moveOutput = moveOutput + whiteMove + " " + blackMove + " ";
                output += moveOutput;
            }

            return output;
        }

        public bool SavePGN(string filename)
        {
            StreamWriter PGNFile = new StreamWriter (filename);

            try {
                PGNFile.WriteLine(ToPGNString());
            } catch(Exception ex) {
                Console.Error.WriteLine ("(EE) Error writing to PGN file: " + ex.Message);
                return false;
            }

            PGNFile.Close ();
            return true;
        }
    }
}

