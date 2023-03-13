#include <windows.h>
#include <iostream>

#include "../driverExample/driverAPI.h"

int main()
{
    std::wcout << L"Opening device" << std::endl;

    auto device = CreateFileW(kDeviceLinkName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (device == INVALID_HANDLE_VALUE)
    {
        std::wcerr << L"Error opening file: " << GetLastError();

        return 1;
    }

    AddRequest request;
    std::wcout << L"Please enter a number: ";
    std::wcin >> request.a;
    std::wcout << L"Please enter another number: ";
    std::wcin >> request.b;
    AddResult result{ 0 };

    DWORD bytesReturned = 0;
    if (DeviceIoControl(
        device,
        static_cast<DWORD>(IoctlCodes::Add),
        &request,
        sizeof(request),
        &result,
        sizeof(result),
        &bytesReturned,
        nullptr) &&
        bytesReturned == sizeof(result))
    {
        std::wcout << L"The result of " << request.a << " + " << request.b << " = " << result.result << std::endl;

        return 0;
    }

    std::wcerr << L"error " << GetLastError();

    return 1;
}