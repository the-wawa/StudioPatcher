#include <chrono>
#include <thread>

#include "ProcUtil.h"
#include "MemUtil.h"
#include "Addresses.h"

int main()
{
    bool outputMsg = false;
    HANDLE hProcess;
    do {
        hProcess = GetProcessFromName(L"RobloxStudioBeta.exe");
        if (!hProcess)
        {
            if (!outputMsg)
            {
                outputMsg = true;
                std::cout << "Unable to get a process handle, are you sure Roblox Studio is open?\nThe program will keep on retrying.\n" << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } while (!hProcess);

    DWORD64 dwProcBase = GetProcessBase(hProcess);
    std::cout << "Successfully got handle!\n";
    std::cout << "Process base: 0x" << std::hex << dwProcBase << std::dec << std::endl;
    std::cout << "\nRepeatedly overwriting process memory\nClose this once you see that Internal Studio has activated" << std::endl;

    BYTE val = TRUE;
    while (true)
    {
        WriteMemory(hProcess, (DWORD64)Addresses::intCheck1 + dwProcBase, &val);
        WriteMemory(hProcess, (DWORD64)Addresses::intCheck2 + dwProcBase, &val);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return EXIT_SUCCESS;
}