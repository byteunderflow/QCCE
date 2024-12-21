#ifndef QCCE_MEMORY_H_
#define QCCE_MEMORY_H_

#include "types.h"

void QCCEInitMemory(void);

void QCCECleanupMemory(void);

void *QCCEAllocateMemory(QCCEUsize Size);

void QCCEDeallocateMemory(void *Memory);

QCCEString QCCEAllocateStringWithLength(QCCEUsize Length);

QCCEString QCCEAllocateStringWithData(const char *Data);

QCCEMoveListRef QCCEAllocateMoveList(void);

QCCEMoveEvaluationRef QCCEAllocateMoveEvaluation(void);

QCCEPositionRef QCCEAllocatePosition(void);

QCCEEngineConfigRef QCCEAllocateEngineConfig(void);

QCCEEngineRef QCCEAllocateEngine(void);

QCCESearchRef QCCEAllocateSearch(void);

QCCEEvaluationRef QCCEAllocateEvaluation(void);

QCCEInterfaceRef QCCEAllocateInterface(void);

#endif