#ifndef QCCE_POSITION_H_
#define QCCE_POSITION_H_

#include "types.h"

#define QCCE_FLAG_CHECKMATE 1
#define QCCE_FLAG_DRAW 2
#define QCCE_FLAG_GAME_OVER (QCCE_FLAG_CHECKMATE | QCCE_FLAG_DRAW)
#define QCCE_FLAG_CASTLE_K 4
#define QCCE_FLAG_CASTLE_Q 8
#define QCCE_FLAG_CASTLE_k 16
#define QCCE_FLAG_CASTLE_q 32
#define QCCE_FLAG_CASTLE_KQ (QCCE_FLAG_CASTLE_K | QCCE_FLAG_CASTLE_Q)
#define QCCE_FLAG_CASTLE_kq (QCCE_FLAG_CASTLE_k | QCCE_FLAG_CASTLE_q)

QCCEPieceColor QCCEGetPositionPieceColor(QCCEPositionRef, QCCEBitboard Square);

QCCEPieceKind QCCEGetPositionPieceKind(QCCEPositionRef, QCCEBitboard Square);

void QCCESetPositionFlags(QCCEPositionRef, QCCEPositionFlags);

void QCCEUnsetPositionFlags(QCCEPositionRef, QCCEPositionFlags);

QCCEBool QCCEHasPositionFlags(QCCEPositionRef, QCCEPositionFlags);

void QCCEUpdateUniversalBitboard(QCCEPositionRef Position, QCCEPieceColor Color);

void QCCEUpdateUniversalBitboards(QCCEPositionRef Position);

QCCEBitboard QCCEComputePawnAttackBitboard(QCCEBitboard PawnBitboard, QCCEPieceColor);

QCCEBitboard QCCEComputeKnightAttackBitboard(QCCEBitboard KnightBitboard);

QCCEBitboard QCCEComputeBishopAttackBitboard(QCCEBitboard BishopBitboard, QCCEBitboard EmptyBitboard);

QCCEBitboard QCCEComputeRookAttackBitboard(QCCEBitboard RookBitboard, QCCEBitboard EmptyBitboard);

QCCEBitboard QCCEComputeQueenAttackBitboard(QCCEBitboard QueenBitboard, QCCEBitboard EmptyBitboard);

QCCEBitboard QCCEComputeKingAttackBitboard(QCCEBitboard KingBitboard);

QCCEBitboard QCCEComputeAttackBitboard(QCCEPositionRef, QCCEPieceColor);

void QCCEUpdateAttackBitboard(QCCEPositionRef, QCCEPieceColor);

void QCCEUpdateAttackBitboards(QCCEPositionRef);

void QCCEUpdatePosition(QCCEPositionRef);

void QCCEPositionDoMove(QCCEPositionRef, QCCEMove);

void QCCEPositionUndoMove(QCCEPositionRef, QCCEMove);

// Creates the corresponding position to the specified string in Forsyth-Edwards Notation.
QCCEPositionRef QCCEGetPositionFromFEN(const char *FEN);

// Returns a string representation of this position in Forsyth-Edwards Notation.
QCCEString QCCEGetPositionAsFENString(QCCEPositionRef);

// Returns a string representation of this position in a human-readable grid format.
QCCEString QCCEGetPositionAsString(QCCEPositionRef);

// Outputs a textual representation of the position to the standard output (stdout).
void QCCEDumpPosition(QCCEPositionRef);

// Disposes the position and releases all associated resources.
void QCCEDisposePosition(QCCEPositionRef);

#endif