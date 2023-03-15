#include <windows.h>
#include <iostream>

#include "../testDriver/driverHeader.h"

int wmain()
{
    auto handle = CreateFileW(
        kDeviceLinkName,
        GENERIC_ALL,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    if (handle == INVALID_HANDLE_VALUE)
    {
        std::wcerr << "error opening device: " << GetLastError() << std::endl;

        return 1;
    }

    DWORD outputSize;

    DeviceIoControl(handle,
        static_cast<DWORD>(ioctl::hello),
        nullptr, 0,
        nullptr, 0,
        &outputSize,
        nullptr);

    return 0;
}