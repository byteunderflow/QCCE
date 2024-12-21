#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/bitboard.h"
#include "../include/position.h"
#include "../include/piece.h"
#include "../include/move.h"
#include "../include/memory.h"
#include "../include/error.h"

inline QCCEPieceColor QCCEGetPositionPieceColor(QCCEPositionRef Position, QCCEBitboard Square)
{
    return Position->UniversalPieceBitboards[QCCE_PIECE_COLOR_WHITE] & Square ?
           QCCE_PIECE_COLOR_WHITE :
           QCCE_PIECE_COLOR_BLACK;
}

QCCEPieceKind QCCEGetPositionPieceKind(QCCEPositionRef Position, QCCEBitboard Square)
{
    for (QCCEPieceKind PieceKind = QCCE_PIECE_KIND_PAWN; PieceKind < QCCE_PIECE_KIND_EMPTY; ++PieceKind)
    {
        if ((Position->PieceBitboards[QCCE_PIECE_COLOR_WHITE][PieceKind] |
             Position->PieceBitboards[QCCE_PIECE_COLOR_BLACK][PieceKind]) & Square)
        {
            return PieceKind;
        }
    }

    return QCCE_PIECE_KIND_EMPTY;
}

inline void QCCESetPositionFlags(QCCEPositionRef Position, QCCEPositionFlags Flags)
{
    Position->Flags |= Flags;
}

inline void QCCEUnsetPositionFlags(QCCEPositionRef Position, QCCEPositionFlags Flags)
{
    Position->Flags &= ~Flags;
}

inline QCCEBool QCCEHasPositionFlags(QCCEPositionRef Position, QCCEPositionFlags Flags)
{
    return (Position->Flags & Flags) == Flags;
}

inline void QCCEUpdateUniversalBitboard(QCCEPositionRef Position, QCCEPieceColor Color)
{
    Position->UniversalPieceBitboards[Color] =
            Position->PieceBitboards[Color][QCCE_PIECE_KIND_PAWN] |
            Position->PieceBitboards[Color][QCCE_PIECE_KIND_KNIGHT] |
            Position->PieceBitboards[Color][QCCE_PIECE_KIND_BISHOP] |
            Position->PieceBitboards[Color][QCCE_PIECE_KIND_ROOK] |
            Position->PieceBitboards[Color][QCCE_PIECE_KIND_QUEEN] |
            Position->PieceBitboards[Color][QCCE_PIECE_KIND_KING];
}

inline void QCCEUpdateUniversalBitboards(QCCEPositionRef Position)
{
    QCCEUpdateUniversalBitboard(Position, QCCE_PIECE_COLOR_WHITE);
    QCCEUpdateUniversalBitboard(Position, QCCE_PIECE_COLOR_BLACK);
}

inline QCCEBitboard QCCEComputePawnAttackBitboard(QCCEBitboard PawnBitboard, QCCEPieceColor Color)
{
    return Color ?
           ((PawnBitboard & QCCE_NOT_FILE_A_BITBOARD) >> 9) | ((PawnBitboard & QCCE_NOT_FILE_H_BITBOARD) >> 7) :
           ((PawnBitboard & QCCE_NOT_FILE_A_BITBOARD) << 7) | ((PawnBitboard & QCCE_NOT_FILE_H_BITBOARD) << 9);
}

inline QCCEBitboard QCCEComputeKnightAttackBitboard(QCCEBitboard KnightBitboard)
{
    return ((KnightBitboard & QCCE_NOT_RANK_8_FILE_AB_BITBOARD) >> 10) |
           ((KnightBitboard & QCCE_NOT_RANK_87_FILE_A_BITBOARD) >> 17) |
           ((KnightBitboard & QCCE_NOT_RANK_87_FILE_H_BITBOARD) >> 15) |
           ((KnightBitboard & QCCE_NOT_RANK_8_FILE_GH_BITBOARD) >> 6) |
           ((KnightBitboard & QCCE_NOT_RANK_1_FILE_GH_BITBOARD) << 10) |
           ((KnightBitboard & QCCE_NOT_RANK_21_FILE_H_BITBOARD) << 17) |
           ((KnightBitboard & QCCE_NOT_RANK_21_FILE_A_BITBOARD) << 15) |
           ((KnightBitboard & QCCE_NOT_RANK_1_FILE_AB_BITBOARD) << 6);
}

inline QCCEBitboard QCCEComputeBishopAttackBitboard(QCCEBitboard BishopBitboard, QCCEBitboard EmptyBitboard)
{
    QCCEBitboard BishopAttackBitboard = 0ull;
    QCCEBitboard NorthWestBitboard = BishopBitboard;
    QCCEBitboard NorthEastBitboard = BishopBitboard;
    QCCEBitboard SouthEastBitboard = BishopBitboard;
    QCCEBitboard SouthWestBitboard = BishopBitboard;

    for (QCCEUint8 Index = 0; Index < 7; ++Index)
    {
        BishopAttackBitboard |=
                (NorthWestBitboard = (NorthWestBitboard & QCCE_NOT_RANK_8_FILE_A_BITBOARD) >> 9) |
                (NorthEastBitboard = (NorthEastBitboard & QCCE_NOT_RANK_8_FILE_H_BITBOARD) >> 7) |
                (SouthEastBitboard = (SouthEastBitboard & QCCE_NOT_RANK_1_FILE_H_BITBOARD) << 9) |
                (SouthWestBitboard = (SouthWestBitboard & QCCE_NOT_RANK_1_FILE_A_BITBOARD) << 7);

        NorthWestBitboard &= EmptyBitboard;
        NorthEastBitboard &= EmptyBitboard;
        SouthEastBitboard &= EmptyBitboard;
        SouthWestBitboard &= EmptyBitboard;
    }

    return BishopAttackBitboard;
}

inline QCCEBitboard QCCEComputeRookAttackBitboard(QCCEBitboard RookBitboard, QCCEBitboard EmptyBitboard)
{
    QCCEBitboard RookAttackBitboard = 0ull;
    QCCEBitboard WestBitboard = RookBitboard;
    QCCEBitboard NorthBitboard = RookBitboard;
    QCCEBitboard EastBitboard = RookBitboard;
    QCCEBitboard SouthBitboard = RookBitboard;

    for (QCCEUint8 Index = 0; Index < 7; ++Index)
    {
        RookAttackBitboard |=
                (WestBitboard = (WestBitboard & QCCE_NOT_FILE_A_BITBOARD) >> 1) |
                (NorthBitboard = (NorthBitboard & QCCE_NOT_RANK_8_BITBOARD) >> 8) |
                (EastBitboard = (EastBitboard & QCCE_NOT_FILE_H_BITBOARD) << 1) |
                (SouthBitboard = (SouthBitboard & QCCE_NOT_RANK_1_BITBOARD) << 8);

        WestBitboard &= EmptyBitboard;
        NorthBitboard &= EmptyBitboard;
        EastBitboard &= EmptyBitboard;
        SouthBitboard &= EmptyBitboard;
    }

    return RookAttackBitboard;
}

inline QCCEBitboard QCCEComputeQueenAttackBitboard(QCCEBitboard QueenBitboard, QCCEBitboard EmptyBitboard)
{
    QCCEBitboard QueenAttackBitboard = 0ull;
    QCCEBitboard NorthWestBitboard = QueenBitboard;
    QCCEBitboard NorthEastBitboard = QueenBitboard;
    QCCEBitboard SouthEastBitboard = QueenBitboard;
    QCCEBitboard SouthWestBitboard = QueenBitboard;
    QCCEBitboard WestBitboard = QueenBitboard;
    QCCEBitboard NorthBitboard = QueenBitboard;
    QCCEBitboard EastBitboard = QueenBitboard;
    QCCEBitboard SouthBitboard = QueenBitboard;

    for (QCCEUint8 Index = 0; Index < 7; ++Index)
    {
        QueenAttackBitboard |=
                (NorthWestBitboard = (NorthWestBitboard & QCCE_NOT_RANK_8_FILE_A_BITBOARD) >> 9) |
                (NorthEastBitboard = (NorthEastBitboard & QCCE_NOT_RANK_8_FILE_H_BITBOARD) >> 7) |
                (SouthEastBitboard = (SouthEastBitboard & QCCE_NOT_RANK_1_FILE_H_BITBOARD) << 9) |
                (SouthWestBitboard = (SouthWestBitboard & QCCE_NOT_RANK_1_FILE_A_BITBOARD) << 7) |
                (WestBitboard = (WestBitboard & QCCE_NOT_FILE_A_BITBOARD) >> 1) |
                (NorthBitboard = (NorthBitboard & QCCE_NOT_RANK_8_BITBOARD) >> 8) |
                (EastBitboard = (EastBitboard & QCCE_NOT_FILE_H_BITBOARD) << 1) |
                (SouthBitboard = (SouthBitboard & QCCE_NOT_RANK_1_BITBOARD) << 8);

        NorthWestBitboard &= EmptyBitboard;
        NorthEastBitboard &= EmptyBitboard;
        SouthEastBitboard &= EmptyBitboard;
        SouthWestBitboard &= EmptyBitboard;
        WestBitboard &= EmptyBitboard;
        NorthBitboard &= EmptyBitboard;
        EastBitboard &= EmptyBitboard;
        SouthBitboard &= EmptyBitboard;
    }

    return QueenAttackBitboard;
}

inline QCCEBitboard QCCEComputeKingAttackBitboard(QCCEBitboard KingBitboard)
{
    return ((KingBitboard & QCCE_NOT_FILE_A_BITBOARD) >> 1) |
           ((KingBitboard & QCCE_NOT_RANK_8_FILE_A_BITBOARD) >> 9) |
           ((KingBitboard & QCCE_NOT_RANK_8_BITBOARD) >> 8) |
           ((KingBitboard & QCCE_NOT_RANK_8_FILE_H_BITBOARD) >> 7) |
           ((KingBitboard & QCCE_NOT_FILE_H_BITBOARD) << 1) |
           ((KingBitboard & QCCE_NOT_RANK_1_FILE_H_BITBOARD) << 9) |
           ((KingBitboard & QCCE_NOT_RANK_1_BITBOARD) << 8) |
           ((KingBitboard & QCCE_NOT_RANK_1_FILE_A_BITBOARD) << 7);
}

inline QCCEBitboard QCCEComputeAttackBitboard(QCCEPositionRef Position, QCCEPieceColor Color)
{
    const QCCEBitboard EmptyBitboard =
            ~((Position->UniversalPieceBitboards[QCCE_PIECE_COLOR_WHITE] |
               Position->UniversalPieceBitboards[QCCE_PIECE_COLOR_BLACK]) ^
              Position->PieceBitboards[Color ^ 1][QCCE_PIECE_KIND_KING]);
    const QCCEBitboard QueenBitboard = Position->PieceBitboards[Color][QCCE_PIECE_KIND_QUEEN];

    return QCCEComputePawnAttackBitboard(Position->PieceBitboards[Color][QCCE_PIECE_KIND_PAWN], Color) |
           QCCEComputeKnightAttackBitboard(Position->PieceBitboards[Color][QCCE_PIECE_KIND_KNIGHT]) |
           QCCEComputeBishopAttackBitboard(
                   Position->PieceBitboards[Color][QCCE_PIECE_KIND_BISHOP] | QueenBitboard, EmptyBitboard) |
           QCCEComputeRookAttackBitboard(
                   Position->PieceBitboards[Color][QCCE_PIECE_KIND_ROOK] | QueenBitboard, EmptyBitboard) |
           QCCEComputeKingAttackBitboard(Position->PieceBitboards[Color][QCCE_PIECE_KIND_KING]);
}

inline void QCCEUpdateAttackBitboard(QCCEPositionRef Position, QCCEPieceColor Color)
{
    Position->AttackBitboards[Color] = QCCEComputeAttackBitboard(Position, Color);
}

inline void QCCEUpdateAttackBitboards(QCCEPositionRef Position)
{
    QCCEUpdateAttackBitboard(Position, QCCE_PIECE_COLOR_WHITE);
    QCCEUpdateAttackBitboard(Position, QCCE_PIECE_COLOR_BLACK);
}

void QCCEUpdatePosition(QCCEPositionRef Position)
{

}

void QCCEPositionDoMove(QCCEPositionRef Position, QCCEMove Move)
{
    const QCCEPieceColor Color = Position->Color;
    const QCCEPieceColor OpponentColor = Color ^ 1;

    const QCCEPieceKind PieceKind = QCCEGetMovePieceKind(Move);
    const QCCEPieceKind PromotionKind = QCCEGetMovePromotionKind(Move);
    const QCCEBitboard SourceSquare = QCCEGetMoveSourceSquare(Move);
    const QCCEBitboard DestinationSquare = QCCEGetMoveDestinationSquare(Move);

    Position->UniversalPieceBitboards[Color] ^= SourceSquare;
    Position->UniversalPieceBitboards[Color] |= DestinationSquare;
    Position->PieceBitboards[Color][PieceKind] ^= SourceSquare;
    Position->PieceBitboards[Color][PromotionKind ? PromotionKind : PieceKind] |= DestinationSquare;

    QCCEBool IsCaptureMove = QCCE_BOOL_FALSE;

    if (Position->EnPassantSquare & DestinationSquare)
    {
        const QCCEBitboard EnPassantPawnSquare = Color ? DestinationSquare << 8 : DestinationSquare >> 8;
        Position->UniversalPieceBitboards[OpponentColor] ^= EnPassantPawnSquare;
        Position->PieceBitboards[OpponentColor][QCCE_PIECE_KIND_PAWN] ^= EnPassantPawnSquare;
        IsCaptureMove = QCCE_BOOL_TRUE;
    }
    else
    {
        for (QCCEPieceKind CapturePieceKind = QCCE_PIECE_KIND_PAWN;
             CapturePieceKind < QCCE_PIECE_KIND_KING; ++CapturePieceKind)
        {
            if (Position->PieceBitboards[OpponentColor][CapturePieceKind] & DestinationSquare)
            {
                Position->UniversalPieceBitboards[OpponentColor] ^= DestinationSquare;
                Position->PieceBitboards[OpponentColor][CapturePieceKind] ^= DestinationSquare;
                IsCaptureMove = QCCE_BOOL_TRUE;
                break;
            }
        }
    }

    if (QCCEIsKingSideCastlingMove(Move))
    {
        const QCCEBitboard RookMovementBitboard = DestinationSquare >> 1 | DestinationSquare << 1;
        Position->UniversalPieceBitboards[Color] ^= RookMovementBitboard;
        Position->PieceBitboards[Color][QCCE_PIECE_KIND_ROOK] ^= RookMovementBitboard;
    }
    else if (QCCEIsQueenSideCastlingMove(Move))
    {
        const QCCEBitboard RookMovementBitboard = DestinationSquare >> 2 | DestinationSquare << 1;
        Position->UniversalPieceBitboards[Color] ^= RookMovementBitboard;
        Position->PieceBitboards[Color][QCCE_PIECE_KIND_ROOK] ^= RookMovementBitboard;
    }

    if (QCCE_PIECE_KIND_KING == PieceKind)
    {
        QCCEUnsetPositionFlags(Position, Color ? QCCE_FLAG_CASTLE_KQ : QCCE_FLAG_CASTLE_kq);
    }

    const QCCEBitboard WhiteRookBitboard = Position->PieceBitboards[QCCE_PIECE_COLOR_WHITE][QCCE_PIECE_KIND_ROOK];
    if (!(WhiteRookBitboard & QCCE_WHITE_KING_SIDE_ROOK_SQUARE))
    {
        QCCEUnsetPositionFlags(Position, QCCE_FLAG_CASTLE_K);
    }
    if (!(WhiteRookBitboard & QCCE_WHITE_QUEEN_SIDE_ROOK_SQUARE))
    {
        QCCEUnsetPositionFlags(Position, QCCE_FLAG_CASTLE_Q);
    }

    const QCCEBitboard BlackRookBitboard = Position->PieceBitboards[QCCE_PIECE_COLOR_BLACK][QCCE_PIECE_KIND_ROOK];
    if (!(BlackRookBitboard & QCCE_BLACK_KING_SIDE_ROOK_SQUARE))
    {
        QCCEUnsetPositionFlags(Position, QCCE_FLAG_CASTLE_k);
    }
    if (!(BlackRookBitboard & QCCE_BLACK_QUEEN_SIDE_ROOK_SQUARE))
    {
        QCCEUnsetPositionFlags(Position, QCCE_FLAG_CASTLE_q);
    }

    if (QCCEIsDoublePushMove(Move))
    {
        Position->EnPassantSquare = Color ? DestinationSquare << 8 : DestinationSquare >> 8;
    }
    else
    {
        Position->EnPassantSquare = 0ull;
    }

    QCCEUpdatePosition(Position);

    if (QCCE_PIECE_KIND_PAWN != PieceKind && !IsCaptureMove && ++Position->PlyCount > 99)
    {
        QCCESetPositionFlags(Position, QCCE_FLAG_DRAW);
        return;
    }

    if (!Color)
    {
        Position->MoveNumber++;
    }

    Position->Color = OpponentColor;
}

void QCCEPositionUndoMove(QCCEPositionRef Position, QCCEMove Move)
{

}

QCCEPositionRef QCCEGetPositionFromFEN(const char *FEN)
{
    QCCEString FENString = QCCEAllocateStringWithData(FEN);
    if (NULL == FENString)
    {
        return NULL;
    }

    const char Delimiter[2] = " ";
    char *PositionString = strtok(FENString, Delimiter);
    char *ColorString = strtok(NULL, Delimiter);
    char *FlagsString = strtok(NULL, Delimiter);
    char *EnPassantString = strtok(NULL, Delimiter);
    char *PlyCountString = strtok(NULL, Delimiter);
    char *MoveNumberString = strtok(NULL, Delimiter);

    if (NULL == PositionString ||
        NULL == ColorString ||
        NULL == FlagsString ||
        NULL == EnPassantString ||
        NULL == PlyCountString ||
        NULL == MoveNumberString)
    {
        QCCESetError(QCCE_ERROR_INVALID_FEN);
        QCCEDeallocateMemory(FENString);
        return NULL;
    }

    QCCEPositionRef Position = QCCEAllocatePosition();
    if (NULL == Position)
    {
        QCCEDeallocateMemory(FENString);
        return NULL;
    }

    QCCEUint8 SquareIndex = 0;
    for (QCCEUsize Index = 0; Index < strlen(PositionString); ++Index)
    {
        const char Symbol = PositionString[Index];

        if ('/' == Symbol)
        {
            continue;
        }

        if (isdigit(Symbol))
        {
            SquareIndex += Symbol - '0';
            continue;
        }

        const QCCEPieceColor PieceColor = QCCEGetPieceColorFromSymbol(Symbol);
        const QCCEPieceKind PieceKind = QCCEGetPieceKindFromSymbol((char) toupper(Symbol));

        Position->PieceBitboards[PieceColor][PieceKind] |= QCCEGetBitboardFromSquareIndex(SquareIndex);
        SquareIndex++;
    }

    Position->Color = ColorString[0] == 'w';

    if (strchr(FlagsString, 'K'))
    {
        QCCESetPositionFlags(Position, QCCE_FLAG_CASTLE_K);
    }
    if (strchr(FlagsString, 'Q'))
    {
        QCCESetPositionFlags(Position, QCCE_FLAG_CASTLE_Q);
    }
    if (strchr(FlagsString, 'k'))
    {
        QCCESetPositionFlags(Position, QCCE_FLAG_CASTLE_k);
    }
    if (strchr(FlagsString, 'q'))
    {
        QCCESetPositionFlags(Position, QCCE_FLAG_CASTLE_q);
    }

    Position->EnPassantSquare = QCCEGetBitboardFromSquareString(EnPassantString);
    Position->PlyCount = (QCCEUint16) strtoul(PlyCountString, NULL, 10);
    Position->MoveNumber = (QCCEUint16) strtoul(MoveNumberString, NULL, 10);

    QCCEUpdateUniversalBitboards(Position);
    QCCEUpdateAttackBitboards(Position);

    QCCEDeallocateMemory(FENString);

    return Position;
}

QCCEString QCCEGetPositionAsFENString(QCCEPositionRef Position)
{
    char FENString[128 + 1];
    QCCEUsize FENStringIndex = 0;
    QCCEUint8 EmptyPieceCount = 0;

    for (QCCEUint8 SquareIndex = 0; SquareIndex < 64; ++SquareIndex)
    {
        const QCCEBitboard Square = QCCEGetBitboardFromSquareIndex(SquareIndex);
        const QCCEPieceKind PieceKind = QCCEGetPositionPieceKind(Position, Square);

        if (QCCE_PIECE_KIND_EMPTY == PieceKind)
        {
            EmptyPieceCount++;
            if (7 == SquareIndex % 8)
            {
                FENString[FENStringIndex++] = (char) ('0' + EmptyPieceCount);
                EmptyPieceCount = 0;
            }
        }
        else
        {
            if (EmptyPieceCount > 0)
            {
                FENString[FENStringIndex++] = (char) ('0' + EmptyPieceCount);
            }
            EmptyPieceCount = 0;

            FENString[FENStringIndex++] =
                    QCCEGetSymbolFromPieceKindAndColor(PieceKind, QCCEGetPositionPieceColor(Position, Square));
        }

        if (7 == SquareIndex % 8 && SquareIndex < 63)
        {
            FENString[FENStringIndex++] = '/';
        }
    }
    FENString[FENStringIndex++] = ' ';

    FENString[FENStringIndex++] = Position->Color ? 'w' : 'b';
    FENString[FENStringIndex++] = ' ';

    if (QCCEHasPositionFlags(Position, QCCE_FLAG_CASTLE_K))
    {
        FENString[FENStringIndex++] = 'K';
    }
    if (QCCEHasPositionFlags(Position, QCCE_FLAG_CASTLE_Q))
    {
        FENString[FENStringIndex++] = 'Q';
    }
    if (QCCEHasPositionFlags(Position, QCCE_FLAG_CASTLE_k))
    {
        FENString[FENStringIndex++] = 'k';
    }
    if (QCCEHasPositionFlags(Position, QCCE_FLAG_CASTLE_q))
    {
        FENString[FENStringIndex++] = 'q';
    }
    if (!QCCEHasPositionFlags(Position, QCCE_FLAG_CASTLE_K) &&
        !QCCEHasPositionFlags(Position, QCCE_FLAG_CASTLE_Q) &&
        !QCCEHasPositionFlags(Position, QCCE_FLAG_CASTLE_k) &&
        !QCCEHasPositionFlags(Position, QCCE_FLAG_CASTLE_q))
    {
        FENString[FENStringIndex++] = '-';
    }
    FENString[FENStringIndex++] = ' ';

    if (Position->EnPassantSquare)
    {
        QCCEString EnPassantSquareString = QCCEGetBitboardAsSquareString(Position->EnPassantSquare);
        if (NULL == EnPassantSquareString)
        {
            return NULL;
        }

        FENString[FENStringIndex++] = EnPassantSquareString[0];
        FENString[FENStringIndex++] = EnPassantSquareString[1];
        QCCEDeallocateMemory(EnPassantSquareString);
    }
    else
    {
        FENString[FENStringIndex++] = '-';
    }
    FENString[FENStringIndex++] = ' ';

    char PlyCountString[5 + 1];
    snprintf(PlyCountString, 5 + 1, "%d", Position->PlyCount);
    for (QCCEUsize Index = 0; Index < strlen(PlyCountString); ++Index)
    {
        FENString[FENStringIndex++] = PlyCountString[Index];
    }
    FENString[FENStringIndex++] = ' ';

    char MoveNumberString[5 + 1];
    snprintf(MoveNumberString, 5 + 1, "%d", Position->MoveNumber);
    for (QCCEUsize Index = 0; Index < strlen(MoveNumberString); ++Index)
    {
        FENString[FENStringIndex++] = MoveNumberString[Index];
    }

    FENString[FENStringIndex] = 0;

    return QCCEAllocateStringWithData(FENString);
}

QCCEString QCCEGetPositionAsString(QCCEPositionRef Position)
{
    return QCCEGetPositionAsFENString(Position);
}

void QCCEDumpPosition(QCCEPositionRef Position)
{
    QCCEString PositionString = QCCEGetPositionAsString(Position);
    if (NULL == PositionString)
    {
        return;
    }

    printf("%s\n", PositionString);

    QCCEDeallocateMemory(PositionString);
}

void QCCEDisposePosition(QCCEPositionRef Position)
{
    QCCEDeallocateMemory(Position->MoveList);
    QCCEDeallocateMemory(Position);
}