#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

HANDLE GetProcessFromName(const wchar_t* procName);
std::vector<HMODULE> GetModuleList(HANDLE hProcess);
DWORD64 GetProcessBase(HANDLE hProcess);