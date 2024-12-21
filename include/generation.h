#ifndef QCCE_GENERATION_H_
#define QCCE_GENERATION_H_

#include "types.h"

void QCCEGenerateMoves(QCCEPositionRef);

void QCCEGeneratePawnMoves(QCCEPositionRef, QCCEBitboard Square, QCCEPieceColor);

void QCCEGenerateKnightMoves(QCCEPositionRef, QCCEBitboard Square);

void QCCEGenerateBishopMoves(QCCEPositionRef, QCCEBitboard Square);

void QCCEGenerateRookMoves(QCCEPositionRef, QCCEBitboard Square);

void QCCEGenerateQueenMoves(QCCEPositionRef, QCCEBitboard Square);

void QCCEGenerateKingMoves(QCCEPositionRef, QCCEBitboard Square);

#endif