#include <stdarg.h>
#include <stdio.h>
#include "../include/assert.h"
#include "../include/error.h"

QCCEBool QCCEAssertTrue(QCCEBool Condition, const char *Format, ...)
{
    if (Condition) return QCCE_BOOL_TRUE;

    va_list Arguments;
    va_start(Arguments, Format);

    char MessageBuffer[1024];
    vsnprintf(MessageBuffer, sizeof(MessageBuffer), Format, Arguments);
    snprintf(MessageBuffer, 1, "\n");

    QCCESetError(QCCE_ERROR_ASSERTION);
    QCCESetErrorMessage(MessageBuffer);

    va_end(Arguments);

    return QCCE_BOOL_FALSE;
}