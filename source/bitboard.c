#include <stdio.h>
#include <string.h>
#include "../include/bitboard.h"
#include "../include/memory.h"

inline QCCEBitboard QCCEGetBitboardFromSquareIndex(QCCEUint8 SquareIndex)
{
    return 1ull << SquareIndex;
}

QCCEBitboard QCCEGetBitboardFromSquareString(const char *SquareString)
{
    if (2 != strlen(SquareString))
    {
        return 0;
    }

    const QCCEUint8 SquareRank = 8 - (SquareString[1] - '0');
    const QCCEUint8 SquareFile = SquareString[0] - 'a';
    return QCCEGetBitboardFromSquareIndex(SquareRank * 8 + SquareFile);
}

QCCEString QCCEGetBitboardAsString(QCCEBitboard Bitboard)
{
    char BitboardString[64 * 2 + 1];
    QCCEUint8 BitboardStringIndex = 0;

    for (QCCEUint8 SquareIndex = 0; SquareIndex < 64; ++SquareIndex)
    {
        BitboardString[BitboardStringIndex++] =
                Bitboard & QCCEGetBitboardFromSquareIndex(SquareIndex) ? 'x' : '-';
        if (7 == SquareIndex % 8)
        {
            BitboardString[BitboardStringIndex++] = '\n';
        }
        else
        {
            BitboardString[BitboardStringIndex++] = ' ';
        }
    }

    BitboardString[BitboardStringIndex] = 0;

    return QCCEAllocateStringWithData(BitboardString);
}

inline QCCEUint8 QCCEGetBitboardAsSquareIndex(QCCEBitboard Bitboard)
{
    return Bitboard ? (QCCEUint8) __builtin_ctzll(Bitboard) : 64;
}

QCCEString QCCEGetBitboardAsSquareString(QCCEBitboard Bitboard)
{
    const QCCEUint8 SquareIndex = QCCEGetBitboardAsSquareIndex(Bitboard);

    char SquareString[3];
    SquareString[0] = (char) ('a' + SquareIndex % 8);
    SquareString[1] = (char) ('0' + (8 - SquareIndex / 8));
    SquareString[2] = 0;

    return QCCEAllocateStringWithData(SquareString);
}

void QCCEDumpBitboard(QCCEBitboard Bitboard)
{
    QCCEString BitboardString = QCCEGetBitboardAsString(Bitboard);
    if (NULL == BitboardString)
    {
        return;
    }

    printf("%s\n", BitboardString);

    QCCEDeallocateMemory(BitboardString);
}