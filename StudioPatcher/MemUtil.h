#pragma once

#include <iostream>
#include <Windows.h>

BOOL ReadMemory(HANDLE hProcess, DWORD64 dwAddress, BYTE* value, SIZE_T* bytesRead);
BOOL WriteMemory(HANDLE hProcess, DWORD64 dwAddress, BYTE* value);