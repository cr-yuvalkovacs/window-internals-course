#include <wdm.h>

#include "wppHeader.h"

#include "driverMain.cpp.tmh"

VOID
driverUnload(
    _In_ struct _DRIVER_OBJECT* DriverObject
)
{
    INFO("driverUnload called");

    WPP_CLEANUP(DriverObject);
}

extern "C" NTSTATUS DriverEntry(
     _DRIVER_OBJECT* DriverObject,
    PUNICODE_STRING RegistryPath
)
{
    WPP_INIT_TRACING(DriverObject, RegistryPath);
    DriverObject->DriverUnload = driverUnload;

    INFO("DriverEntry called");

    return STATUS_SUCCESS;
}
