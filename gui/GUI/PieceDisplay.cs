using System;
using Gtk;
using Cairo;

namespace GUI
{
    /**
     * @class PieceDisplay
     * @brief Class which displays the pieces on the GUI board.
     * 
     * This class manages the @c ImageSurface objects for each piece. It also contains
     * an array of @c PointD objects, one for each square on the chessboard.
     */
    public static class PieceDisplay
    {
        private static ImageSurface whitePawn;
        private static ImageSurface whiteRook;
        private static ImageSurface whiteKnight;
        private static ImageSurface whiteBishop;
        private static ImageSurface whiteQueen;
        private static ImageSurface whiteKing;

        private static ImageSurface blackPawn;
        private static ImageSurface blackRook;
        private static ImageSurface blackKnight;
        private static ImageSurface blackBishop;
        private static ImageSurface blackQueen;
        private static ImageSurface blackKing;

        private static PointD[] pieceCoordinates = 
            new PointD[]{
                // 8th rank
                new PointD(40, 40),
                new PointD(122, 40),
                new PointD(204, 40),
                new PointD(286, 40),
                new PointD(368, 40),
                new PointD(450, 40),
                new PointD(532, 40),
                new PointD(614, 40),

                // 7th rank
                new PointD(40, 122),
                new PointD(122, 122),
                new PointD(204, 122),
                new PointD(286, 122),
                new PointD(368, 122),
                new PointD(450, 122),
                new PointD(532, 122),
                new PointD(614, 122),

                // 6th rank
                new PointD(40, 204),
                new PointD(122, 204),
                new PointD(204, 204),
                new PointD(286, 204),
                new PointD(368, 204),
                new PointD(450, 204),
                new PointD(532, 204),
                new PointD(614, 204),

                // 5th rank
                new PointD(40, 286),
                new PointD(122, 286),
                new PointD(204, 286),
                new PointD(286, 286),
                new PointD(368, 286),
                new PointD(450, 286),
                new PointD(532, 286),
                new PointD(614, 286),

                // 4th rank
                new PointD(40, 368),
                new PointD(122, 368),
                new PointD(204, 368),
                new PointD(286, 368),
                new PointD(368, 368),
                new PointD(450, 368),
                new PointD(532, 368),
                new PointD(614, 368),

                // 3rd rank
                new PointD(40, 450),
                new PointD(122, 450),
                new PointD(204, 450),
                new PointD(286, 450),
                new PointD(368, 450),
                new PointD(450, 450),
                new PointD(532, 450),
                new PointD(614, 450),

                // 2nd rank
                new PointD(40, 532),
                new PointD(122, 532),
                new PointD(204, 532),
                new PointD(286, 532),
                new PointD(368, 532),
                new PointD(450, 532),
                new PointD(532, 532),
                new PointD(614, 532),

                // 1st rank
                new PointD(40, 614),
                new PointD(122, 614),
                new PointD(204, 614),
                new PointD(286, 614),
                new PointD(368, 614),
                new PointD(450, 614),
                new PointD(532, 614),
                new PointD(614, 614),
            };


        public static void Init()
        {
            whitePawn = new ImageSurface ("img/wp.png");
            whiteRook = new ImageSurface ("img/wr.png");
            whiteKnight = new ImageSurface ("img/wn.png");
            whiteBishop = new ImageSurface ("img/wb.png");
            whiteQueen = new ImageSurface ("img/wq.png");
            whiteKing = new ImageSurface ("img/wk.png");

            blackPawn = new ImageSurface ("img/bp.png");
            blackRook = new ImageSurface ("img/br.png");
            blackKnight = new ImageSurface ("img/bn.png");
            blackBishop = new ImageSurface ("img/bb.png");
            blackQueen = new ImageSurface ("img/bq.png");
            blackKing = new ImageSurface ("img/bk.png");
        }

        public static void DrawPieces(Cairo.Context cr)
        {
            for (int i = 0; i < 64; i++) {
                Piece currentPiece = MainClass.CurrentBoard.Squares [i].Piece;
                PointD currentPoint = pieceCoordinates [i];
                if (currentPiece == null)
                    continue;
                if (currentPiece.Colour == PieceColour.White) {
                    switch (currentPiece.Type) {
                        case PieceType.Pawn:
                            whitePawn.Show (cr, currentPoint.X, currentPoint.Y);
                            break;
                        case PieceType.Rook:
                            whiteRook.Show (cr, currentPoint.X, currentPoint.Y);
                            break;
                        case PieceType.Knight:
                            whiteKnight.Show (cr, currentPoint.X, currentPoint.Y);
                            break;
                        case PieceType.Bishop:
                            whiteBishop.Show (cr, currentPoint.X, currentPoint.Y);
                            break;
                        case PieceType.Queen:
                            whiteQueen.Show (cr, currentPoint.X, currentPoint.Y);
                            break;
                        case PieceType.King:
                            whiteKing.Show (cr, currentPoint.X, currentPoint.Y);
                            break;
                        default:
                            break;
                    }
                } else {
                    switch (currentPiece.Type) {
                        case PieceType.Pawn:
                            blackPawn.Show (cr, currentPoint.X, currentPoint.Y);
                            break;
                        case PieceType.Rook:
                            blackRook.Show (cr, currentPoint.X, currentPoint.Y);
                            break;
                        case PieceType.Knight:
                            blackKnight.Show (cr, currentPoint.X, currentPoint.Y);
                            break;
                        case PieceType.Bishop:
                            blackBishop.Show (cr, currentPoint.X, currentPoint.Y);
                            break;
                        case PieceType.Queen:
                            blackQueen.Show (cr, currentPoint.X, currentPoint.Y);
                            break;
                        case PieceType.King:
                            blackKing.Show (cr, currentPoint.X, currentPoint.Y);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}

