#include <windows.h>
#include <string>
#include <vector>

using namespace std;
void myLoadLibrary(wstring filename)
{
    auto fileHandle = CreateFile(filename.c_str(),
        GENERIC_READ,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr);

    auto fileSize = GetFileSize(fileHandle, nullptr);

    vector<byte> fileData(fileSize);

    DWORD temp = 0;

    ReadFile(
        fileHandle,
        fileData.data(),
        static_cast<DWORD>(fileData.size()),
        &temp,
        nullptr);

    auto dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(fileData.data());
    auto ntHeader = reinterpret_cast<IMAGE_NT_HEADERS*>(fileData.data() + dosHeader->e_lfanew);

    // allocate memory for image
    auto targetMem = VirtualAllocEx(GetCurrentProcess(),
        nullptr,
        ntHeader->OptionalHeader.SizeOfImage,
        MEM_RESERVE | MEM_COMMIT,
        PAGE_EXECUTE_READWRITE);

    // copy headers
    memcpy_s(
        targetMem,
        ntHeader->OptionalHeader.SizeOfImage,
        fileData.data(),
        ntHeader->OptionalHeader.SizeOfHeaders
    );

    auto sectionPtr = reinterpret_cast<IMAGE_SECTION_HEADER*>(
        reinterpret_cast<byte*>(&ntHeader->OptionalHeader) +
        ntHeader->FileHeader.SizeOfOptionalHeader);

    // map all section
    for (auto i = 0; i < ntHeader->FileHeader.NumberOfSections; i++)
    {
        memcpy_s(
            targetMem,
            ntHeader->OptionalHeader.SizeOfImage,
            fileData.data(),
            ntHeader->OptionalHeader.SizeOfHeaders
        );
        sectionPtr++;
    }
}


int wmain(int argc, wchar_t** argv)
{
    if (argc < 3)
    {
      //  return 1;
    }

    myLoadLibrary(L"C:\\Users\\yuval.kovacs\\Development\\window-internals-course\\loader\\x64\\Debug\\injectedDll.dll");

    return 0;
}