#pragma once

enum class ioctl : ULONG
{
    hello = CTL_CODE(
        FILE_DEVICE_UNKNOWN,
        0,
        METHOD_BUFFERED,
        FILE_ANY_ACCESS)
};

constexpr wchar_t const* kDeviceName = L"\\Device\\testDevice1";
constexpr wchar_t const * kDeviceLinkName = L"\\??\\testDevice1";