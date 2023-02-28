#include <windows.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;
int wmain(int argc, wchar_t** argv)
{
    DWORD pid = stoul(argv[1]);
    auto processHandle = OpenProcess(MAXIMUM_ALLOWED, FALSE, pid);

    if (processHandle == NULL)
    {
        wcerr << L"failed opening process: " << GetLastError() << endl;
        return 1;
    }

    vector<wchar_t> filenameBuffer(1024);

    filenameBuffer.resize(
        GetFullPathNameW(argv[2],
            filenameBuffer.size(),
            filenameBuffer.data(),
            nullptr) + 1);


    auto remoteAllocation = VirtualAllocEx(
        processHandle,
        nullptr,
        filenameBuffer.size() * sizeof(wchar_t),
        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);


    if (remoteAllocation == NULL)
    {
        wcerr << L"failed allocating memory: " << GetLastError() << endl;
        return 1;
    }

    if(!WriteProcessMemory(
        processHandle,
        remoteAllocation,
        filenameBuffer.data(),
        filenameBuffer.size() * sizeof(wchar_t),
        nullptr))
    {
        wcerr << L"failed writing to memory: " << GetLastError() << endl;
        return 1;
    }

    auto LoadLibraryWPtr = GetProcAddress(
        GetModuleHandle(L"kernel32.dll"),
        "LoadLibraryW");


    DWORD threadId;
    auto threadHandle = CreateRemoteThread(
        processHandle,
        nullptr,
        0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryWPtr),
        remoteAllocation,
        0,
        &threadId);

    if (threadHandle == NULL)
    {
        wcerr << L"failed creating thread: " << GetLastError() << endl;

        return 1;
    }

    wcout << L"successfully injected " << filenameBuffer.data() << " into process with pid " << pid << endl;

    return 0;
}