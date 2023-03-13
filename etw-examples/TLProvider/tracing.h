#pragma once

#include <TraceLoggingProvider.h>
#include <traceloggingactivity.h>

TRACELOGGING_DECLARE_PROVIDER(g_myExampleProvider);

TRACELOGGING_DEFINE_PROVIDER(
    g_myExampleProvider,
    "exampleProvider",
    // {298D3D12-1222-4E83-B892-2962D6F2996B}
    (0x298d3d12, 0x1222, 0x4e83, 0xb8, 0x92, 0x29, 0x62, 0xd6, 0xf2, 0x99, 0x6b));

