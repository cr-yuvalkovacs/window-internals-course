#include <windows.h>
#include <iostream>

int wmain(int, wchar_t**)
{
    std::wcout << GetCurrentProcessId() << std::endl;
    while (true)
    {
        auto result = WaitForSingleObjectEx(GetCurrentProcess(), 1000, TRUE);
        if (result != WAIT_TIMEOUT)
        {
            std::wcout << "APC" << std::endl;
        }
    }

    return 0;
}