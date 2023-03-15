#include <wdm.h>

#include "driverHeader.h"

PDEVICE_OBJECT g_deviceObject = nullptr;

VOID
driverUnload(
    _In_ struct _DRIVER_OBJECT* DriverObject
)
{
    UNREFERENCED_PARAMETER(DriverObject);

    if (g_deviceObject) {

        UNICODE_STRING symbolicLinkName;
        RtlInitUnicodeString(&symbolicLinkName, kDeviceLinkName);

        IoDeleteSymbolicLink(&symbolicLinkName);
        IoDeleteDevice(g_deviceObject);

    }
}

NTSTATUS createDevice(PDEVICE_OBJECT object, PIRP irp)
{
    UNREFERENCED_PARAMETER(object);
    UNREFERENCED_PARAMETER(irp);

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "device created\n");

    irp->IoStatus.Status = STATUS_SUCCESS;
    irp->IoStatus.Information = 0;

    IoCompleteRequest(irp, 0);

    return STATUS_SUCCESS;
}

NTSTATUS closeDevice(PDEVICE_OBJECT object, PIRP irp)
{
    UNREFERENCED_PARAMETER(object);
    UNREFERENCED_PARAMETER(irp);
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "device closed\n");

    irp->IoStatus.Status = STATUS_SUCCESS;
    irp->IoStatus.Information = 0;

    IoCompleteRequest(irp, 0);

    return STATUS_SUCCESS;
}

NTSTATUS deviceControl(PDEVICE_OBJECT object, PIRP irp)
{
    UNREFERENCED_PARAMETER(object);
    UNREFERENCED_PARAMETER(irp);
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "device control called\n");

    irp->IoStatus.Status = STATUS_SUCCESS;
    irp->IoStatus.Information = 0;

    IoCompleteRequest(irp, 0);

    return STATUS_SUCCESS;
}

extern "C" NTSTATUS DriverEntry(
     _DRIVER_OBJECT* DriverObject,
     PUNICODE_STRING RegistryPath
)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "driver loading\n");
    DriverObject->DriverUnload = driverUnload;

    DriverObject->MajorFunction[IRP_MJ_CREATE] = createDevice;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = closeDevice;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = deviceControl;
    UNICODE_STRING deviceName;

    RtlInitUnicodeString(&deviceName, kDeviceName);

    auto status = IoCreateDevice(
        DriverObject,
        0,
        &deviceName,
        FILE_DEVICE_UNKNOWN,
        0,
        FALSE,
        &g_deviceObject);

    if (!NT_SUCCESS(status))
    {
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "error creating device\n");

        return status;
    }

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "device created successfully\n");

    UNICODE_STRING symbolicLinkName;

    RtlInitUnicodeString(&symbolicLinkName, kDeviceLinkName);
    status = IoCreateSymbolicLink(&symbolicLinkName, &deviceName);

    if (!NT_SUCCESS(status))
    {
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "error creating link\n");

        return status;
    }

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "link created successfully\n");

    return 0;
}