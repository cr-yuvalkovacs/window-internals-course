#pragma once

constexpr const wchar_t* kDeviceName = L"\\Device\\testDevice";
constexpr const wchar_t* kDeviceLinkName = L"\\??\\testDevice";

enum class IoctlCodes : ULONG // IOCTL codes are ULONG
{
    Add = CTL_CODE(
        FILE_DEVICE_UNKNOWN,
        1,
        METHOD_BUFFERED,
        FILE_WRITE_DATA | FILE_READ_ACCESS
    ),
};

struct AddRequest
{
    LONG a;
    LONG b;
};

struct AddResult
{
    LONGLONG result;
};