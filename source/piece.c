#include <ctype.h>
#include "../include/piece.h"

QCCEPieceColor QCCEGetPieceColorFromSymbol(char Symbol)
{
    return (QCCEPieceColor) isupper(Symbol);
}

QCCEPieceKind QCCEGetPieceKindFromSymbol(char Symbol)
{
    Symbol = (char) toupper(Symbol);
    if (Symbol == 'P') return QCCE_PIECE_KIND_PAWN;
    if (Symbol == 'N') return QCCE_PIECE_KIND_KNIGHT;
    if (Symbol == 'B') return QCCE_PIECE_KIND_BISHOP;
    if (Symbol == 'R') return QCCE_PIECE_KIND_ROOK;
    if (Symbol == 'Q') return QCCE_PIECE_KIND_QUEEN;
    if (Symbol == 'K') return QCCE_PIECE_KIND_KING;
    return QCCE_PIECE_KIND_EMPTY;
}

char QCCEGetSymbolFromPieceKind(QCCEPieceKind Kind)
{
    if (Kind == QCCE_PIECE_KIND_PAWN) return 'P';
    if (Kind == QCCE_PIECE_KIND_KNIGHT) return 'N';
    if (Kind == QCCE_PIECE_KIND_BISHOP) return 'B';
    if (Kind == QCCE_PIECE_KIND_ROOK) return 'R';
    if (Kind == QCCE_PIECE_KIND_QUEEN) return 'Q';
    if (Kind == QCCE_PIECE_KIND_KING) return 'K';
    return 0;
}

char QCCEGetSymbolFromPieceKindAndColor(QCCEPieceKind Kind, QCCEPieceColor Color)
{
    char Symbol = QCCEGetSymbolFromPieceKind(Kind);
    return (char) (Color ? Symbol : tolower(Symbol));
}