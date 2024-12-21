#ifndef QCCE_TYPES_H_
#define QCCE_TYPES_H_

#include "inttypes.h"

#define QCCE_BOOL_TRUE 1
#define QCCE_BOOL_FALSE 0

typedef signed char QCCEInt8;
typedef signed short QCCEInt16;
typedef signed int QCCEInt32;
typedef signed long long QCCEInt64;
typedef QCCEInt64 QCCESize;
typedef unsigned char QCCEUint8;
typedef unsigned short QCCEUint16;
typedef unsigned int QCCEUint32;
typedef unsigned long long QCCEUint64;
typedef QCCEUint64 QCCEUsize;
typedef QCCEUint8 QCCEBool;
typedef QCCEUint32 QCCEBool32;
typedef char *QCCEString;
typedef QCCEInt32 QCCEError;

typedef QCCEUint8 QCCEPieceColor;
typedef QCCEUint8 QCCEPieceKind;
typedef QCCEUint8 QCCEPositionFlags;
typedef QCCEUint64 QCCEBitboard;
typedef QCCEUint32 QCCEMove;

struct QCCEMoveList
{
    QCCEMove Moves[256];
    QCCEUint8 Index;
};
typedef struct QCCEMoveList *QCCEMoveListRef;

struct QCCEMoveEvaluation
{
};
typedef struct QCCEMoveEvaluation *QCCEMoveEvaluationRef;

struct QCCEPosition
{
    QCCEPieceColor Color;
    QCCEPositionFlags Flags;
    QCCEUint16 PlyCount;
    QCCEUint16 MoveNumber;

    QCCEBitboard PieceBitboards[2][6];
    QCCEBitboard UniversalPieceBitboards[2];
    QCCEBitboard AttackBitboards[2];
    QCCEBitboard EnPassantSquare;

    QCCEMoveListRef MoveList;
};
typedef struct QCCEPosition *QCCEPositionRef;

struct QCCEEngineConfig
{
};
typedef struct QCCEEngineConfig *QCCEEngineConfigRef;

struct QCCEEngine
{
};
typedef struct QCCEEngine *QCCEEngineRef;

struct QCCESearch
{
};
typedef struct QCCESearch *QCCESearchRef;

struct QCCEEvaluation
{
};
typedef struct QCCEEvaluation *QCCEEvaluationRef;

struct QCCEInterface
{
};
typedef struct QCCEInterface *QCCEInterfaceRef;

#endif