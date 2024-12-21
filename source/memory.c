#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/memory.h"
#include "../include/error.h"

static void **MemoryPool;
static QCCEUsize MemoryPoolCapacity;
static QCCEUsize MemoryPoolIndex;

void QCCEInitMemory(void)
{
    MemoryPoolCapacity = 16;
    MemoryPoolIndex = 0;

    MemoryPool = (void **) malloc(MemoryPoolCapacity * sizeof(void *));
    if (NULL == MemoryPool)
    {
        QCCESetError(QCCE_ERROR_OUT_OF_MEMORY);
        return;
    }

    atexit(QCCECleanupMemory);
}

void QCCECleanupMemory(void)
{
    if (NULL == MemoryPool)
    {
        QCCESetError(QCCE_ERROR_UNINITIALIZED);
        return;
    }

    for (QCCEUsize Index = 0; Index < MemoryPoolIndex; ++Index)
    {
        void *Memory = MemoryPool[Index];
        if (NULL == Memory)
        {
            continue;
        }

        free(Memory);
        MemoryPool[Index] = NULL;

        fprintf(stderr, "Deallocated memory at address 0x%p (index %llu)\n",
                Memory, Index);
    }

    free(MemoryPool);
    MemoryPool = NULL;
    MemoryPoolCapacity = 0;
    MemoryPoolIndex = 0;
}

void *QCCEAllocateMemory(QCCEUsize Size)
{
    if (NULL == MemoryPool)
    {
        QCCESetError(QCCE_ERROR_UNINITIALIZED);
        return NULL;
    }

    void *Memory = (void *) malloc(Size);
    if (NULL == Memory)
    {
        QCCESetError(QCCE_ERROR_OUT_OF_MEMORY);
        return NULL;
    }

    if (MemoryPoolIndex < MemoryPoolCapacity)
    {
        MemoryPool[MemoryPoolIndex++] = Memory;
        return Memory;
    }

    const QCCEUsize NewMemoryPoolCapacity = MemoryPoolCapacity * 2;

    void **NewMemoryPool = (void **)
            realloc(MemoryPool, NewMemoryPoolCapacity * sizeof(void *));
    if (NULL == NewMemoryPool)
    {
        free(Memory);
        QCCESetError(QCCE_ERROR_OUT_OF_MEMORY);
        return NULL;
    }

    MemoryPool = NewMemoryPool;
    MemoryPoolCapacity = NewMemoryPoolCapacity;
    MemoryPool[MemoryPoolIndex++] = Memory;

    return Memory;
}

void QCCEDeallocateMemory(void *Memory)
{
    if (NULL == MemoryPool)
    {
        QCCESetError(QCCE_ERROR_UNINITIALIZED);
        return;
    }

    if (NULL == Memory)
    {
        return;
    }

    for (QCCEUsize Index = 0; Index < MemoryPoolIndex; ++Index)
    {
        if (Memory != MemoryPool[Index])
        {
            continue;
        }

        free(Memory);

        for (QCCEUsize ShiftIndex = Index; ShiftIndex < MemoryPoolIndex - 1; ++ShiftIndex)
        {
            MemoryPool[ShiftIndex] = MemoryPool[ShiftIndex + 1];
        }
        MemoryPool[--MemoryPoolIndex] = NULL;

        break;
    }
}

inline QCCEString QCCEAllocateStringWithLength(QCCEUsize Length)
{
    QCCEString String = (QCCEString) QCCEAllocateMemory((Length + 1) * sizeof(char));
    if (NULL == String)
    {
        return NULL;
    }

    String[Length] = 0;

    return String;
}

inline QCCEString QCCEAllocateStringWithData(const char *Data)
{
    QCCEString String = (QCCEString) QCCEAllocateMemory((strlen(Data) + 1) * sizeof(char));
    if (NULL == String)
    {
        return NULL;
    }

    strcpy(String, Data);

    return String;
}

inline QCCEMoveListRef QCCEAllocateMoveList(void)
{
    QCCEMoveListRef MoveList = (QCCEMoveListRef) QCCEAllocateMemory(sizeof(struct QCCEMoveList));
    if (NULL == MoveList)
    {
        return NULL;
    }

    MoveList->Index = 0;

    return MoveList;
}

inline QCCEMoveEvaluationRef QCCEAllocateMoveEvaluation(void)
{
    return (QCCEMoveEvaluationRef) QCCEAllocateMemory(sizeof(struct QCCEMoveEvaluation));
}

inline QCCEPositionRef QCCEAllocatePosition(void)
{
    QCCEMoveListRef MoveList = QCCEAllocateMoveList();
    if (NULL == MoveList)
    {
        return NULL;
    }

    QCCEPositionRef Position = (QCCEPositionRef) QCCEAllocateMemory(sizeof(struct QCCEPosition));
    if (NULL == Position)
    {
        QCCEDeallocateMemory(MoveList);
        return NULL;
    }

    Position->Color = 0;
    Position->Flags = 0;
    Position->PlyCount = 0;
    Position->MoveNumber = 0;

    for (QCCEUsize Index = 0; Index < 6; ++Index)
    {
        Position->PieceBitboards[0][Index] = 0;
    }
    for (QCCEUsize Index = 0; Index < 6; ++Index)
    {
        Position->PieceBitboards[1][Index] = 0;
    }

    Position->UniversalPieceBitboards[0] = 0;
    Position->UniversalPieceBitboards[1] = 0;

    Position->AttackBitboards[0] = 0;
    Position->AttackBitboards[1] = 0;

    Position->MoveList = MoveList;

    return Position;
}

inline QCCEEngineConfigRef QCCEAllocateEngineConfig(void)
{
    return (QCCEEngineConfigRef) QCCEAllocateMemory(sizeof(struct QCCEEngineConfig));
}

inline QCCEEngineRef QCCEAllocateEngine(void)
{
    return (QCCEEngineRef) QCCEAllocateMemory(sizeof(struct QCCEEngine));
}

inline QCCESearchRef QCCEAllocateSearch(void)
{
    return (QCCESearchRef) QCCEAllocateMemory(sizeof(struct QCCESearch));
}

inline QCCEEvaluationRef QCCEAllocateEvaluation(void)
{
    return (QCCEEvaluationRef) QCCEAllocateMemory(sizeof(struct QCCEEvaluation));
}

inline QCCEInterfaceRef QCCEAllocateInterface(void)
{
    return (QCCEInterfaceRef) QCCEAllocateMemory(sizeof(struct QCCEInterface));
}