#ifndef QCCE_QCCE_H_
#define QCCE_QCCE_H_

#include "types.h"
#include "move.h"
#include "piece.h"
#include "position.h"
#include "generation.h"
#include "engine.h"
#include "search.h"
#include "evaluation.h"
#include "interface.h"
#include "error.h"

#define QCCE_NAME "QCCE (Quick C Chess Engine)"
#define QCCE_VENDOR "QCCE"
#define QCCE_VERSION "0.0.1-Alpha"
#define QCCE_INFO (QCCE_NAME "\nQCCE Vendor: " QCCE_VENDOR "\nQCCE Version: " QCCE_VERSION)

// Initializes QCCE.
// This function should be called before using any other QCCE functions.
void QCCEInit(void);

// Returns a string containing general information about the QCCE build.
QCCEString QCCEGetInfo(void);

// Dumps general information about the QCCE build to the standard output.
void QCCEDumpInfo(void);

// Disposes the string and releases all associated resources.
void QCCEDisposeString(QCCEString);

#endif