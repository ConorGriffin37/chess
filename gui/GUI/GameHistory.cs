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
        public string FEN { get; set; }
        public int CheckOrCheckmate { get; set; } // 0 for none, 1 for check, 2 for checkmate

        public Move(byte source, byte destination, PieceColour colour, Piece movingPiece,
            MoveResult result, string fen, int checkOrCheckmate, SpecifierType specifierRequired = SpecifierType.None, PieceType? promoteTo = null)
        {
            Source = source;
            Destination = destination;
            Colour = colour;
            MovingPiece = movingPiece;
            SpecifierRequired = specifierRequired;
            PromoteTo = promoteTo;
            Result = result;
            FEN = fen;
            CheckOrCheckmate = checkOrCheckmate;
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
            FEN = other.FEN;
            CheckOrCheckmate = other.CheckOrCheckmate;
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

            if (move.CheckOrCheckmate == 1) {
                moveString += "+";
            } else if (move.CheckOrCheckmate == 2) {
                moveString += "#";
            }
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
            output += ("[Result \"" + Result + "\"]\n\n");

            // Now we can print the moves
            for (int i = 0; i < history.Count; i += 2) {
                string moveOutput = ((i / 2) + 1) + ". ";
                string whiteMove = "";
                string blackMove = "";

                if (history [i].Result == MoveResult.KingsideCastle) {
                    moveOutput += "O-O";
                } else if (history [i].Result == MoveResult.QueensideCastle) {
                    moveOutput += "O-O-O";
                } else if (history [i].Colour == PieceColour.Black) {
                    whiteMove = "...";
                    blackMove = MoveToNotation (history [i]);
                } else if (history.Count % 2 != 0 && i == history.Count - 1) {  // When the final move is a white move
                    whiteMove = MoveToNotation (history [i]);
                    blackMove = "";
                } else {
                    whiteMove = MoveToNotation (history [i]);
                    blackMove = MoveToNotation (history [i + 1]);
                }

                moveOutput = moveOutput + whiteMove + " " + blackMove + " ";
                output += moveOutput;
            }

            return output;
        }

        public static SpecifierType checkDisabiguationNeeded(Board theBoard, byte source, byte destination)
        {
            string pieceOnSquare = SquareToNotation(source);
            bool needsFileSpecifier = false;
            bool needsRankSpecifier = false;
            for (int i = 0; i < theBoard.Squares.Length; i++)
            {
                if (theBoard.Squares[i].Piece != null)
                {
                    if ((theBoard.Squares[i].Piece.Type == theBoard.Squares[source].Piece.Type) && (i != source))
                    {
                        for (int j = 0; j < theBoard.Squares[i].Piece.LegalMoves.Count; j++)
                        {
                            if (theBoard.Squares[i].Piece.LegalMoves[j] == destination)
                            {
                                string otherPieceSquare = SquareToNotation((byte)i);
                                if (otherPieceSquare[0] != pieceOnSquare[0]) 
                                {
                                    needsFileSpecifier = true;
                                }
                                else
                                {
                                    needsRankSpecifier = true;
                                }

                            }
                        }
                    }
                }
            }
            if (needsFileSpecifier && needsRankSpecifier)
            {
                return SpecifierType.Both;
            }
            else if (needsFileSpecifier)
            {
                return SpecifierType.File;
            }
            else if (needsRankSpecifier)
            {
                return SpecifierType.Rank;
            }
            return SpecifierType.None;
        }

        public static List<Tuple<Move, string>> getPossibleMoveNotations(Board gameBoard)
        {
            List<Tuple<Move, string>> possibleMoveNotations = new List<Tuple<Move, string>>();

            for (int i = 0; i < gameBoard.Squares.Length; i++)
            {
                if (gameBoard.Squares[i].Piece != null)
                {
                    for (int j = 0; j < gameBoard.Squares[i].Piece.LegalMoves.Count; j++)
                    {
                        SpecifierType disambiguationNeeded = checkDisabiguationNeeded(gameBoard, (byte)i, gameBoard.Squares[i].Piece.LegalMoves[j]);

                        Board copiedBoard = new Board(gameBoard);
                        MoveResult result = MoveResult.None;
                        if ((gameBoard.Squares[i].Piece.Type == PieceType.Pawn) && (Array.IndexOf(copiedBoard.pawnPromotionDestinations, gameBoard.Squares[i].Piece.LegalMoves[j]) != -1)) {
                            //Promotion
                            result = copiedBoard.MakeMove((byte)i, gameBoard.Squares[i].Piece.LegalMoves[j], PieceType.Queen);

                            string copiedBoardFEN = copiedBoard.ToFEN ();

                            int checkOrCheckmate = 0;
                            GameStatus mateState = copiedBoard.CheckForMate ();
                            if (mateState == GameStatus.WhiteCheckmate || mateState == GameStatus.BlackCheckmate) {
                                checkOrCheckmate = 2;
                            } else if (copiedBoard.WhiteCheck || copiedBoard.BlackCheck) {
                                checkOrCheckmate = 1;
                            }

                            Move newMove = new Move((byte)i, gameBoard.Squares[i].Piece.LegalMoves[j], gameBoard.Squares[i].Piece.Colour, gameBoard.Squares[i].Piece,
                                                    result, copiedBoardFEN, checkOrCheckmate, disambiguationNeeded, PieceType.Queen);
                            Tuple<Move, string> newTuple = new Tuple<Move, string>(newMove, MoveToNotation(newMove));
                            possibleMoveNotations.Add(newTuple);
                            newMove = new Move((byte)i, gameBoard.Squares[i].Piece.LegalMoves[j], gameBoard.Squares[i].Piece.Colour, gameBoard.Squares[i].Piece,
                                result, copiedBoardFEN, checkOrCheckmate, disambiguationNeeded, PieceType.Rook);
                            newTuple = new Tuple<Move, string>(newMove, MoveToNotation(newMove));
                            possibleMoveNotations.Add(newTuple);
                            newMove = new Move((byte)i, gameBoard.Squares[i].Piece.LegalMoves[j], gameBoard.Squares[i].Piece.Colour, gameBoard.Squares[i].Piece,
                                result, copiedBoardFEN, checkOrCheckmate, disambiguationNeeded, PieceType.Bishop);
                            newTuple = new Tuple<Move, string>(newMove, MoveToNotation(newMove));
                            possibleMoveNotations.Add(newTuple);
                            newMove = new Move((byte)i, gameBoard.Squares[i].Piece.LegalMoves[j], gameBoard.Squares[i].Piece.Colour, gameBoard.Squares[i].Piece,
                                result, copiedBoardFEN, checkOrCheckmate, disambiguationNeeded, PieceType.Knight);
                            newTuple = new Tuple<Move, string>(newMove, MoveToNotation(newMove));
                            possibleMoveNotations.Add(newTuple);
                        } else {
                            result = copiedBoard.MakeMove((byte)i, gameBoard.Squares[i].Piece.LegalMoves[j]);

                            string copiedBoardFEN = copiedBoard.ToFEN ();

                            int checkOrCheckmate = 0;
                            GameStatus mateState = copiedBoard.CheckForMate ();
                            if (mateState == GameStatus.WhiteCheckmate || mateState == GameStatus.BlackCheckmate) {
                                checkOrCheckmate = 2;
                            } else if (copiedBoard.WhiteCheck || copiedBoard.BlackCheck) {
                                checkOrCheckmate = 1;
                            }

                            Move newMove = new Move((byte)i, gameBoard.Squares[i].Piece.LegalMoves[j], gameBoard.Squares[i].Piece.Colour, gameBoard.Squares[i].Piece,
                                result, copiedBoardFEN, checkOrCheckmate, disambiguationNeeded, null);
                            Tuple<Move, string> newTuple = new Tuple<Move, string>(newMove, MoveToNotation(newMove));
                            possibleMoveNotations.Add(newTuple);
                        }
                    }
                }
            }
            return possibleMoveNotations;
        }

        public static GameHistory importPGN(string PGN)
        {
            GameHistory newGameHistory = new GameHistory();
            //Parse tag pairs first
            string startingFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
            while (PGN.Contains("["))
            {
                string tagPair = PGN.Substring(PGN.IndexOf('[') + 1, PGN.IndexOf(']') - PGN.IndexOf('[') - 1);
                PGN = PGN.Substring(PGN.IndexOf(']') + 1);

                switch (tagPair.Substring(0, tagPair.IndexOf(' ')))
                {
                    case "Event":
                        newGameHistory.Event = tagPair.Substring(tagPair.IndexOf('"') + 1).TrimEnd('"');
                        break;
                    case "Site":
                        newGameHistory.Site = tagPair.Substring(tagPair.IndexOf('"') + 1).TrimEnd('"');
                        break;
                    case "Date":
                        newGameHistory.Date = tagPair.Substring(tagPair.IndexOf('"') + 1).TrimEnd('"');
                        break;
                    case "Round":
                        newGameHistory.Round = tagPair.Substring(tagPair.IndexOf('"') + 1).TrimEnd('"');
                        break;
                    case "White":
                        newGameHistory.White = tagPair.Substring(tagPair.IndexOf('"') + 1).TrimEnd('"');
                        break;
                    case "Black":
                        newGameHistory.Black = tagPair.Substring(tagPair.IndexOf('"') + 1).TrimEnd('"');
                        break;
                    case "Result":
                        newGameHistory.Result = tagPair.Substring(tagPair.IndexOf('"') + 1).TrimEnd('"');
                        break;
                    case "FEN":
                        startingFEN = tagPair.Substring(tagPair.IndexOf('"') + 1).TrimEnd('"');
                        break;
                    default:
                        break;
                }
            }
            //Remove comments from PGN
            while (PGN.Contains("{"))
            {
                PGN = PGN.Remove(PGN.IndexOf('{'), PGN.IndexOf('}') - PGN.IndexOf('{') + 1);
            }

            //Take in moves one by one
            FENParser importFEN = new FENParser(startingFEN);
            Board gameBoard = importFEN.GetBoard();

            PiecePseudoLegalMoves.GeneratePseudoLegalMoves(gameBoard);
            PieceLegalMoves.GenerateLegalMoves(gameBoard);

            List<Tuple<Move, string>> possibleMoveNotations = getPossibleMoveNotations(gameBoard);
            string[] tokens = PGN.Split(null);

            for (int i = 0; i < tokens.Length; i++)
            {
                char[] trimChars = { '!', '?', '\n'};
                string moveNotation = tokens[i].Trim(trimChars);

                if (moveNotation.Contains('.'))
                {
                    moveNotation = moveNotation.Substring(moveNotation.IndexOf('.') + 1);
                }

                if (moveNotation.Length != 0)
                {
                    //Console.WriteLine("***" + moveNotation + "***"); //debugging output
                    for (int j = 0; j < possibleMoveNotations.Count; j++)
                    {
                        Console.Write(possibleMoveNotations[j].Item2 + " ");
                        if (moveNotation.Equals(possibleMoveNotations[j].Item2))
                        {
                            gameBoard.MakeMove(possibleMoveNotations[j].Item1.Source, possibleMoveNotations[j].Item1.Destination, possibleMoveNotations[j].Item1.PromoteTo);
                            newGameHistory.AddMove(possibleMoveNotations[j].Item1, gameBoard.ToFEN().Split(' ')[0]);
                            possibleMoveNotations = getPossibleMoveNotations(gameBoard);
                            break;
                        }
                    }
                    //Console.WriteLine(""); //debugging output
                }
            }
            return newGameHistory;
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

