#include <windows.h>
#include <thread>

#include "tracing.h"
#include "CountActivity.h"


int wmain(int argc, wchar_t** argv)
{
    // Register the provider
    TraceLoggingRegister(g_myExampleProvider);

    // Log an event
    TraceLoggingWrite(g_myExampleProvider,
        "HelloWorldTestEvent",
        TraceLoggingValue("HelloWorld", "MyFirstEvent")
    );

    CountActivity activityDoer;
    std::thread threads[10];

    for (auto& t : threads)
    {
        t = std::thread([&]() {activityDoer(); });
    }

    for (auto& t : threads)
    {
        t.join();
    }

    // Stop TraceLogging and unregister the provider
    TraceLoggingUnregister(g_myExampleProvider);

    return 0;
}