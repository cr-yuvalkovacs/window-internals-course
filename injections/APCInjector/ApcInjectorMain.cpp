#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <TlHelp32.h>

using namespace std;

HANDLE getThreadInProcess(DWORD pid)
{
    auto snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);

    THREADENTRY32 entry{ 0 };
    entry.dwSize = sizeof(entry);
    if (!Thread32First(snapshotHandle, &entry))
    {
        return nullptr;
    }

    while (entry.th32OwnerProcessID != pid && Thread32Next(snapshotHandle, &entry));
    return OpenThread(MAXIMUM_ALLOWED, FALSE, entry.th32ThreadID);
}

int wmain(int argc, wchar_t** argv)
{
    vector<wchar_t> filenameBuffer(1024);

    filenameBuffer.resize(
        GetFullPathNameW(argv[2],
            filenameBuffer.size(),
            filenameBuffer.data(),
            nullptr) + 1);

    if (GetFileAttributes(filenameBuffer.data()) == INVALID_FILE_ATTRIBUTES)
    {
        wcerr << L"file doesn't exists: " << filenameBuffer.data() << endl;

        return 1;
    }

    DWORD pid = stoul(argv[1]);
    auto processHandle = OpenProcess(MAXIMUM_ALLOWED, FALSE, pid);

    if (processHandle == NULL)
    {
        wcerr << L"failed opening process: " << GetLastError() << endl;
        return 1;
    }

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

    if (!WriteProcessMemory(
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

    auto threadHandle = getThreadInProcess(pid);

    if (!QueueUserAPC(
        reinterpret_cast<PAPCFUNC>(LoadLibraryWPtr),
        threadHandle,
        reinterpret_cast<ULONG_PTR>(remoteAllocation)
    ))
    {
        wcerr << L"failed queueing apc: " << GetLastError() << endl;
        return 1;

    }

    wcout << L"successfully injected " << filenameBuffer.data() << " into process with pid " << pid << endl;

    return 0;
}