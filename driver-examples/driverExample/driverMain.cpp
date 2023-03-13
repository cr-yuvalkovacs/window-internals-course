#include <wdm.h>
#include <Wdmsec.h>

#include "driverAPI.h"

PDEVICE_OBJECT g_deviceObj = nullptr;

void
driverUnload(
    _In_ struct _DRIVER_OBJECT* DriverObject
)
{
    UNREFERENCED_PARAMETER(DriverObject);

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Driver unloading");

    if (g_deviceObj)
    {
        UNICODE_STRING linkName;
        RtlInitUnicodeString(&linkName, kDeviceLinkName);

        IoDeleteSymbolicLink(&linkName);

        IoDeleteDevice(g_deviceObj);
    };
}

NTSTATUS CreateRoutine(
    _DEVICE_OBJECT* DeviceObject,
    _IRP* Irp
)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Device opened");

    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status = STATUS_SUCCESS;

    IoCompleteRequest(Irp, 0);

    return STATUS_SUCCESS;
}

NTSTATUS CloseRoutine(
    _DEVICE_OBJECT* DeviceObject,
    _IRP* Irp
)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Device closed");

    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status = STATUS_SUCCESS;

    IoCompleteRequest(Irp, 0);

    return STATUS_SUCCESS;
}

NTSTATUS ControlRoutine(
    _DEVICE_OBJECT* DeviceObject,
    _IRP* Irp
)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Device got IRP");

    auto stack = IoGetCurrentIrpStackLocation(Irp);

    // Input checks
    if (stack->Parameters.DeviceIoControl.IoControlCode != static_cast<ULONG>(IoctlCodes::Add))
    {
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Got bad io request");

        return STATUS_INVALID_DEVICE_REQUEST;
    }

    if (stack->Parameters.DeviceIoControl.InputBufferLength < sizeof(AddRequest))
    {
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Got small buffer");

        return STATUS_BUFFER_TOO_SMALL;
    }

    if (stack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(AddResult))
    {
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Got small buffer");

        return STATUS_BUFFER_TOO_SMALL;
    }

    // Process request
    auto request = reinterpret_cast<AddRequest*>(Irp->AssociatedIrp.SystemBuffer);
    auto result = reinterpret_cast<AddResult*>(Irp->AssociatedIrp.SystemBuffer);


    result->result = static_cast<LONGLONG>(request->a) + request->b;

    // Finish request
    Irp->IoStatus.Information = sizeof(AddResult);
    Irp->IoStatus.Status = STATUS_SUCCESS;

    IoCompleteRequest(Irp, 0);

    return STATUS_SUCCESS;
}

extern "C" NTSTATUS DriverEntry(
     _DRIVER_OBJECT* DriverObject,
    PUNICODE_STRING RegistryPath
)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Driver Loading");
    DriverObject->DriverUnload = driverUnload;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = &CreateRoutine;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = &CloseRoutine;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = &ControlRoutine;


    UNICODE_STRING deviceName;
    RtlInitUnicodeString(&deviceName, kDeviceName);

    auto status = IoCreateDevice(
        DriverObject,               // DriverObject
        0,                          // DeviceExtensionSize
        &deviceName,                 // DeviceName
        FILE_DEVICE_UNKNOWN,        // DeviceType
        FILE_DEVICE_SECURE_OPEN,    // DeviceCharacteristics
        FALSE,                       // Exclusive
        &g_deviceObj);                  // Device output

    if (!NT_SUCCESS(status))
    {
        DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Device creation failed");

        return status;
    }

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Device created");

    UNICODE_STRING linkName;
    RtlInitUnicodeString(&linkName, kDeviceLinkName);

    status = IoCreateSymbolicLink(&linkName, &deviceName);

    if (!NT_SUCCESS(status))
    {
        IoDeleteDevice(g_deviceObj);

        DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Device creation failed");

        return status;
    }

    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Driver Loaded");

    return STATUS_SUCCESS;
}
