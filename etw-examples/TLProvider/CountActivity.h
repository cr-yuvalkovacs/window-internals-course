#pragma once
#include <windows.h>
#include "tracing.h"


struct CountActivity
{
    TraceLoggingActivity<g_myExampleProvider> activity;

    CountActivity()
    {
        TraceLoggingWriteStart(activity, "CounterActivity");
    }

    ~CountActivity()
    {
        TraceLoggingWriteStop(activity, "CounterActivity");
    }

    void operator()()
    {
        for (size_t i = 0; i < 100; i++)
        {
            TraceLoggingWriteTagged(activity, "Count",
                TraceLoggingValue(i, "Counter")
            );
        }
    }
};
