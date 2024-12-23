#include <stdio.h>
#include "../include/core.h"
#include "../include/memory.h"

static QCCEBool IsInitialized = QCCE_BOOL_FALSE;

void QCCEInit(void)
{
    if (IsInitialized)
    {
        return;
    }

    QCCEInitMemory();
    
    IsInitialized = QCCE_BOOL_TRUE;
}

inline QCCEString QCCEGetInfo(void)
{
    return QCCE_INFO;
}

void QCCEDumpInfo(void)
{
    printf("%s\n", QCCE_INFO);
}

void QCCEDisposeString(QCCEString String)
{
    QCCEDeallocateMemory(String);
}