#ifndef QCCE_ERROR_H_
#define QCCE_ERROR_H_

#include "types.h"

#define QCCE_ERROR_NONE 0
#define QCCE_ERROR_UNINITIALIZED 1
#define QCCE_ERROR_OUT_OF_MEMORY 2
#define QCCE_ERROR_ASSERTION 3
#define QCCE_ERROR_INVALID_FEN 4

// Returns and resets the current error code.
// If no error has occurred, QCCE_ERROR_NONE is returned.
QCCEError QCCEGetError(void);

// Returns and resets the current error message.
// If no error has occurred or the current error does not provide an error message, NULL is returned.
QCCEString QCCEGetErrorMessage(void);

// Sets the current error code.
void QCCESetError(QCCEError);

// Sets the current error message.
void QCCESetErrorMessage(const char *Message);

// Dumps the current error information to the standard error output (stderr).
void QCCEDumpError(void);

#endif