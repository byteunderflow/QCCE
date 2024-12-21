#include <string.h>
#include <stdio.h>
#include "../include/move.h"
#include "../include/piece.h"
#include "../include/bitboard.h"
#include "../include/memory.h"

#define QCCE_MOVE_SOURCE_SQUARE_INDEX 3
#define QCCE_MOVE_DESTINATION_SQUARE_INDEX 9
#define QCCE_MOVE_PROMOTION_KIND_INDEX 15
#define QCCE_MOVE_DOUBLE_PUSH_INDEX 18
#define QCCE_MOVE_KING_SIDE_CASTLING_INDEX 19
#define QCCE_MOVE_QUEEN_SIDE_CASTLING_INDEX 20

inline QCCEMove QCCEGetMove(QCCEBitboard SourceSquare, QCCEBitboard DestinationSquare, QCCEPieceKind PieceKind)
{
    return PieceKind |
           (QCCEGetBitboardAsSquareIndex(SourceSquare) << QCCE_MOVE_SOURCE_SQUARE_INDEX) |
           (QCCEGetBitboardAsSquareIndex(DestinationSquare) << QCCE_MOVE_DESTINATION_SQUARE_INDEX);
}

inline QCCEMove QCCEGetPromotionMove(QCCEBitboard SourceSquare, QCCEBitboard DestinationSquare,
                                     QCCEPieceKind PromotionKind)
{
    return (QCCEGetBitboardAsSquareIndex(SourceSquare) << QCCE_MOVE_SOURCE_SQUARE_INDEX) |
           (QCCEGetBitboardAsSquareIndex(DestinationSquare) << QCCE_MOVE_DESTINATION_SQUARE_INDEX) |
           (PromotionKind << QCCE_MOVE_PROMOTION_KIND_INDEX);
}

inline QCCEMove QCCEGetDoublePushMove(QCCEBitboard SourceSquare, QCCEBitboard DestinationSquare)
{
    return (QCCEGetBitboardAsSquareIndex(SourceSquare) << QCCE_MOVE_SOURCE_SQUARE_INDEX) |
           (QCCEGetBitboardAsSquareIndex(DestinationSquare) << QCCE_MOVE_DESTINATION_SQUARE_INDEX) |
           (1 << QCCE_MOVE_DOUBLE_PUSH_INDEX);
}

inline QCCEMove QCCEGetKingSideCastlingMove(QCCEBitboard SourceSquare, QCCEBitboard DestinationSquare)
{
    return QCCE_PIECE_KIND_KING |
           (QCCEGetBitboardAsSquareIndex(SourceSquare) << QCCE_MOVE_SOURCE_SQUARE_INDEX) |
           (QCCEGetBitboardAsSquareIndex(DestinationSquare) << QCCE_MOVE_DESTINATION_SQUARE_INDEX) |
           (1 << QCCE_MOVE_KING_SIDE_CASTLING_INDEX);
}

inline QCCEMove QCCEGetQueenSideCastlingMove(QCCEBitboard SourceSquare, QCCEBitboard DestinationSquare)
{
    return QCCE_PIECE_KIND_KING |
           (QCCEGetBitboardAsSquareIndex(SourceSquare) << QCCE_MOVE_SOURCE_SQUARE_INDEX) |
           (QCCEGetBitboardAsSquareIndex(DestinationSquare) << QCCE_MOVE_DESTINATION_SQUARE_INDEX) |
           (1 << QCCE_MOVE_QUEEN_SIDE_CASTLING_INDEX);
}

inline QCCEPieceKind QCCEGetMovePieceKind(QCCEMove Move)
{
    return Move & 7;
}

inline QCCEBitboard QCCEGetMoveSourceSquare(QCCEMove Move)
{
    return QCCEGetBitboardFromSquareIndex(Move >> QCCE_MOVE_SOURCE_SQUARE_INDEX & 63);
}

inline QCCEBitboard QCCEGetMoveDestinationSquare(QCCEMove Move)
{
    return QCCEGetBitboardFromSquareIndex(Move >> QCCE_MOVE_DESTINATION_SQUARE_INDEX & 63);
}

inline QCCEPieceKind QCCEGetMovePromotionKind(QCCEMove Move)
{
    return Move >> QCCE_MOVE_PROMOTION_KIND_INDEX & 7;
}

inline QCCEBool32 QCCEIsDoublePushMove(QCCEMove Move)
{
    return Move >> QCCE_MOVE_DOUBLE_PUSH_INDEX & 1;
}

inline QCCEBool32 QCCEIsKingSideCastlingMove(QCCEMove Move)
{
    return Move >> QCCE_MOVE_KING_SIDE_CASTLING_INDEX & 1;
}

inline QCCEBool32 QCCEIsQueenSideCastlingMove(QCCEMove Move)
{
    return Move >> QCCE_MOVE_QUEEN_SIDE_CASTLING_INDEX & 1;
}

QCCEString QCCEGetMoveAsString(QCCEMove Move)
{
    if (QCCEIsKingSideCastlingMove(Move))
    {
        return QCCEAllocateStringWithData("0-0");
    }

    if (QCCEIsQueenSideCastlingMove(Move))
    {
        return QCCEAllocateStringWithData("0-0-0");
    }

    QCCEString SourceSquareString = QCCEGetBitboardAsSquareString(QCCEGetMoveSourceSquare(Move));
    if (NULL == SourceSquareString)
    {
        return NULL;
    }

    QCCEString DestinationSquareString = QCCEGetBitboardAsSquareString(QCCEGetMoveDestinationSquare(Move));
    if (NULL == DestinationSquareString)
    {
        QCCEDeallocateMemory(SourceSquareString);
        return NULL;
    }

    const QCCEPieceKind PromotionKind = QCCEGetMovePromotionKind(Move);

    char MoveString[6];
    MoveString[0] = SourceSquareString[0];
    MoveString[1] = SourceSquareString[1];
    MoveString[2] = DestinationSquareString[0];
    MoveString[3] = DestinationSquareString[1];

    if (PromotionKind)
    {
        MoveString[4] = QCCEGetSymbolFromPieceKindAndColor(PromotionKind, QCCE_PIECE_COLOR_WHITE);
        MoveString[5] = 0;
    }
    else
    {
        MoveString[4] = 0;
    }

    QCCEDeallocateMemory(SourceSquareString);
    QCCEDeallocateMemory(DestinationSquareString);

    return QCCEAllocateStringWithData(MoveString);
}

inline QCCEUsize QCCEGetMoveListSize(QCCEMoveListRef MoveList)
{
    return MoveList->Index;
}

inline QCCEBool QCCEIsMoveListEmpty(QCCEMoveListRef MoveList)
{
    return MoveList->Index == 0;
}

inline QCCEMove QCCEGetFromMoveList(QCCEMoveListRef MoveList, QCCEUsize Index)
{
    return MoveList->Moves[Index];
}

QCCEMove QCCEGetFromMoveListWithName(QCCEMoveListRef MoveList, const char *Name)
{
    for (QCCEUsize Index = 0; Index < MoveList->Index; ++Index)
    {
        const QCCEMove Move = QCCEGetFromMoveList(MoveList, Index);
        if (strcmp(Name, QCCEGetMoveAsString(Move)) == 0)
        {
            return Move;
        }
    }

    return 0;
}

QCCEString QCCEGetMoveListAsString(QCCEMoveListRef MoveList)
{
    char MoveListString[MoveList->Index * 8 * sizeof(char)];
    QCCEUsize MoveListStringIndex = 0;

    for (QCCEUsize Index = 0; Index < MoveList->Index; ++Index)
    {
        const QCCEMove Move = QCCEGetFromMoveList(MoveList, Index);

        QCCEString MoveString = QCCEGetMoveAsString(Move);
        if (NULL == MoveString)
        {
            QCCEDeallocateMemory(MoveString);
            QCCEDeallocateMemory(MoveListString);
            return NULL;
        }

        const QCCEUsize MoveStringLength = strlen(MoveString);
        strncpy(MoveListString + MoveListStringIndex, MoveString, MoveStringLength);
        MoveListStringIndex += MoveStringLength;

        QCCEDeallocateMemory(MoveString);

        if (Index == MoveList->Index - 1)
        {
            break;
        }

        MoveListString[MoveListStringIndex++] = ',';
        MoveListString[MoveListStringIndex++] = ' ';
    }

    MoveListString[MoveListStringIndex] = 0;

    return QCCEAllocateStringWithData(MoveListString);
}

inline void QCCEAppendToMoveList(QCCEMoveListRef MoveList, QCCEMove Move)
{
    MoveList->Moves[MoveList->Index++] = Move;
}

inline void QCCEResetMoveList(QCCEMoveListRef MoveList)
{
    memset(MoveList->Moves, 0, MoveList->Index * sizeof(QCCEMove));
    MoveList->Index = 0;
}

void QCCEDumpMoveList(QCCEMoveListRef MoveList)
{
    QCCEString MoveListString = QCCEGetMoveListAsString(MoveList);
    if (NULL == MoveListString)
    {
        return;
    }

    printf("%s\n", MoveListString);

    QCCEDeallocateMemory(MoveListString);
}

void QCCEDisposeMoveList(QCCEMoveListRef MoveList)
{
    QCCEDeallocateMemory(MoveList);
}