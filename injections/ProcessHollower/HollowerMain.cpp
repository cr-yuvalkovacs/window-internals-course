#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

int wmain(int argc, wchar_t** argv)
{
    wstring hollowedExe(argv[1]);
    wstring injectedExe(argv[2]);

    STARTUPINFOW si{ 0 };
    PROCESS_INFORMATION pi{ 0 };
    if (!CreateProcessW(
        hollowedExe.c_str(),
        const_cast<LPWSTR>(L"arg1 arg2"),
        nullptr,
        nullptr,
        FALSE,
        CREATE_SUSPENDED,
        nullptr,
        nullptr,
        &si,
        &pi
    ))
    {
        wcerr << "failed creating process " << hollowedExe << endl;

        return 1;
    }


    return 0;
}