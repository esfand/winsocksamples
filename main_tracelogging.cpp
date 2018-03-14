
#include "TwitterTraceLogging.h"


// Define the GUID to use in TraceLoggingProviderRegister 
// {3970F9cf-2c0c-4f11-b1cc-e3a1e9958833}
TRACELOGGING_DEFINE_PROVIDER(
	g_hTwitterComponentProvider,
	"TwitterTraceLoggingProvider",
	(0x3970f9cf, 0x2c0c, 0x4f11, 0xb1, 0xcc, 0xe3, 0xa1, 0xe9, 0x95, 0x88, 0x33));


void main_tracelogging()
{
	char sampleValue[] = "Sample value";

	// Register the provider
	TraceLoggingRegister(g_hTwitterComponentProvider);

	// Log an event
	TraceLoggingWrite(g_hTwitterComponentProvider,                    // handle to my provider
		              "HelloWorldTestEvent",                          // Event Name that should uniquely identify your event.
		              TraceLoggingValue(sampleValue, "TestMessage")); // Field for your event in the form of (value, field name).

	// Stop TraceLogging and unregister the provider
	TraceLoggingUnregister(g_hTwitterComponentProvider);
}
