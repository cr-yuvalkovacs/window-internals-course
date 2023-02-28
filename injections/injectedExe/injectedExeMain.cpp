#include <windows.h>
#include <iostream>

int wmain(int argc, wchar_t** argv)
{
    for (int i = 0; i < argc; i++)
    {
        std::wcout << argv[i] << std::endl;
    }

    return 0;
}