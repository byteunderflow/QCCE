#include <stdio.h>
#include "../include/core.h"
#include "../include/memory.h"

static QCCEError Error;
static QCCEString ErrorMessage;

QCCEError QCCEGetError(void)
{
    QCCEError CurrentError = Error;
    Error = QCCE_ERROR_NONE;
    return CurrentError;
}

QCCEString QCCEGetErrorMessage(void)
{
    QCCEString CurrentErrorMessage = ErrorMessage;
    ErrorMessage = NULL;
    return CurrentErrorMessage;
}

void QCCESetError(QCCEError NewError)
{
    Error = NewError;
}

void QCCESetErrorMessage(const char *Message)
{
    if (NULL != ErrorMessage)
    {
        QCCEDeallocateMemory(ErrorMessage);
    }

    if (NULL == Message)
    {
        ErrorMessage = NULL;
        return;
    }

    QCCEString NewErrorMessage = QCCEAllocateStringWithData(Message);
    if (NULL != NewErrorMessage)
    {
        ErrorMessage = NewErrorMessage;
    }
}

void QCCEDumpError(void)
{
    if (QCCE_ERROR_NONE == Error)
    {
        return;
    }

    if (NULL == ErrorMessage)
    {
        fprintf(stderr, "Error: 0x%.8x\n", Error);
        return;
    }

    fprintf(stderr, "Error: 0x%.8x: %s\n", Error, ErrorMessage);
}