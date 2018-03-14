// "TwitterTraceLogging.h"

#pragma once

#include <windows.h>               // Defines macros used by TraceLoggingProvider.h
#include <TraceLoggingProvider.h>  // The native TraceLogging API

// Forward-declare the g_hTwitterComponentProvider variable that you will use for tracing in this component
TRACELOGGING_DECLARE_PROVIDER(g_hTwitterComponentProvider);

