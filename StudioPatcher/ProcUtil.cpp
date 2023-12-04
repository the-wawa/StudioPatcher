#include "ProcUtil.h"

HANDLE GetProcessFromName(const wchar_t* procName)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        std::cout << "Unable to get a process snapshot, error code: " << GetLastError() << std::endl;
        return NULL;
    }

    HANDLE result = NULL;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe32))
    {
        std::cout << "Unable to get first process, error code: " << GetLastError() << std::endl;
        goto finish;
    }

    do
    {
        if (_wcsicmp(pe32.szExeFile, procName) == 0)
        {
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
            if (!hProcess)
            {
                std::cout << "Unable to open handle to process (PID: " << pe32.th32ProcessID << "), error code: " << GetLastError() << std::endl;
                continue;
            }

            result = hProcess;
            goto finish;
        }
    } while (Process32Next(hSnapshot, &pe32));

finish:
    CloseHandle(hSnapshot);
    return result;
}

std::vector<HMODULE> GetModuleList(HANDLE hProcess)
{
    std::vector<HMODULE> vec;

    DWORD dwProcId = GetProcessId(hProcess);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcId);
    if (!hSnapshot)
    {
        std::cout << "Unable to get a process snapshot, error code: " << GetLastError() << std::endl;
        goto finish;
    }

    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);

    if (!Module32First(hSnapshot, &me32))
    {
        std::cout << "Unable to get first module for process (PID: " << dwProcId << "); error code: " << GetLastError() << std::endl;
        goto finish;
    }

    do {
        if (me32.th32ProcessID == dwProcId)
        {
            vec.emplace_back(me32.hModule);
        }
    } while (Module32Next(hSnapshot, &me32));

finish:
    if (hSnapshot)
        CloseHandle(hSnapshot);

    return vec;
}

DWORD64 GetProcessBase(HANDLE hProcess)
{
    char processName[MAX_PATH];
    DWORD dwPath = MAX_PATH;
    if (!QueryFullProcessImageNameA(hProcess, PROCESS_NAME_NATIVE, processName, &dwPath))
    {
        std::cout << "Unable to query process image name, error code: " << GetLastError() << std::endl;
        return 0;
    }

    std::vector<HMODULE> vec = GetModuleList(hProcess);
    for (HMODULE hModule : vec)
    {
        char moduleName[MAX_PATH];
        GetModuleFileNameExA(hProcess, hModule, moduleName, MAX_PATH);
        if (strstr(moduleName, processName) == 0)
        {
            return (DWORD64)hModule;
        }
    }

    std::cout << "Unable to find a module which matches the process name" << std::endl;

    return 0;
}