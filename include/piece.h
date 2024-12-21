#ifndef QCCE_PIECE_H_
#define QCCE_PIECE_H_

#include "types.h"

#define QCCE_PIECE_COLOR_WHITE 1
#define QCCE_PIECE_COLOR_BLACK 0

#define QCCE_PIECE_KIND_PAWN 0
#define QCCE_PIECE_KIND_KNIGHT 1
#define QCCE_PIECE_KIND_BISHOP 2
#define QCCE_PIECE_KIND_ROOK 3
#define QCCE_PIECE_KIND_QUEEN 4
#define QCCE_PIECE_KIND_KING 5
#define QCCE_PIECE_KIND_EMPTY 6

QCCEPieceColor QCCEGetPieceColorFromSymbol(char);

QCCEPieceKind QCCEGetPieceKindFromSymbol(char);

char QCCEGetSymbolFromPieceKind(QCCEPieceKind);

char QCCEGetSymbolFromPieceKindAndColor(QCCEPieceKind, QCCEPieceColor);

#endif