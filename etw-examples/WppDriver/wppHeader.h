#pragma once
#define WPP_CONTROL_GUIDS                                               \
    WPP_DEFINE_CONTROL_GUID(                                            \
        myDriverTraceGuid, (84bdb2e9,829e,41b3,b891,02f454bc2bd7),      \
        WPP_DEFINE_BIT(MYDRIVER_ALL_INFO)                               \
        )

//
// This comment block is scanned by the trace preprocessor to define our
// Trace* functions
//
// begin_wpp config
// FUNC INFO(MSG,...);
// end_wpp
//