#ifndef QCCE_MOVE_H_
#define QCCE_MOVE_H_

#include "types.h"

QCCEMove QCCEGetMove(QCCEBitboard SourceSquare, QCCEBitboard DestinationSquare, QCCEPieceKind);

QCCEMove QCCEGetPromotionMove(QCCEBitboard SourceSquare, QCCEBitboard DestinationSquare, QCCEPieceKind);

QCCEMove QCCEGetDoublePushMove(QCCEBitboard SourceSquare, QCCEBitboard DestinationSquare);

QCCEMove QCCEGetKingSideCastlingMove(QCCEBitboard SourceSquare, QCCEBitboard DestinationSquare);

QCCEMove QCCEGetQueenSideCastlingMove(QCCEBitboard SourceSquare, QCCEBitboard DestinationSquare);

QCCEPieceKind QCCEGetMovePieceKind(QCCEMove);

QCCEBitboard QCCEGetMoveSourceSquare(QCCEMove);

QCCEBitboard QCCEGetMoveDestinationSquare(QCCEMove);

QCCEPieceKind QCCEGetMovePromotionKind(QCCEMove);

QCCEBool32 QCCEIsDoublePushMove(QCCEMove);

QCCEBool32 QCCEIsKingSideCastlingMove(QCCEMove);

QCCEBool32 QCCEIsQueenSideCastlingMove(QCCEMove);

QCCEString QCCEGetMoveAsString(QCCEMove);

// Returns the size of the move list.
QCCEUsize QCCEGetMoveListSize(QCCEMoveListRef);

// Returns QCCE_BOOL_TRUE if the move list is empty, QCCE_BOOL_FALSE otherwise.
// This function is equal to the expression QCCEGetMoveListSize(QCCEMoveListRef) == 0.
QCCEBool QCCEIsMoveListEmpty(QCCEMoveListRef);

// Returns the move at the specified index from the move list.
// The index must be within the range [0, QCCEGetMoveListSize(QCCEMoveListRef) - 1].
QCCEMove QCCEGetFromMoveList(QCCEMoveListRef, QCCEUsize Index);

// Returns the move with the specified name from the move list.
QCCEMove QCCEGetFromMoveListWithName(QCCEMoveListRef, const char *Name);

// Returns a string representation of the move list.
QCCEString QCCEGetMoveListAsString(QCCEMoveListRef);

// Appends the specified move to the end of the move list.
void QCCEAppendToMoveList(QCCEMoveListRef, QCCEMove);

// Resets the move list to its initial empty state.
void QCCEResetMoveList(QCCEMoveListRef);

// Outputs a textual representation of the move list to the standard output (stdout).
void QCCEDumpMoveList(QCCEMoveListRef);

// Disposes the move list and releases all associated resources.
void QCCEDisposeMoveList(QCCEMoveListRef);

#endif