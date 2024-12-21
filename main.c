#include "include/core.h"
#include "include/bitboard.h"
#include <stdio.h>

int main(void)
{
    QCCEInit();

    QCCEPositionRef Position = QCCEGetPositionFromFEN(
            "r3kb1r/pb2q2p/n1pp1p2/5np1/R2P4/1PN1PB2/1BPN2PP/3QK1R1 w kq - 1 15");
    QCCEDumpPosition(Position);

    QCCEDisposePosition(Position);

    return 0;
}