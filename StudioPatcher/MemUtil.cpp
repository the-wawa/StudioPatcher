#include "MemUtil.h"

BOOL ReadMemory(HANDLE hProcess, DWORD64 dwAddress, BYTE* value, SIZE_T* bytesRead)
{
	DWORD dwOldProt;
	if (!VirtualProtectEx(hProcess, (void*)dwAddress, sizeof(*value), PAGE_EXECUTE_READ, &dwOldProt))
	{
		std::cout << "Unable to update protection for memory page before reading, error code: " << GetLastError() << std::endl;
		return FALSE;
	}

	BOOL status = TRUE;
	if (!ReadProcessMemory(hProcess, (void*)dwAddress, value, sizeof(*value), bytesRead))
	{
		std::cout << "Unable to read memory page, error code: " << GetLastError() << std::endl;
		status = FALSE;
	}
	//else
	//{
	//	std::cout << "Successfully read " << bytesRead << " bytes" << std::endl;
	//}

	if (!VirtualProtectEx(hProcess, (void*)dwAddress, sizeof(*value), dwOldProt, &dwOldProt))
	{
		std::cout << "Unable to revert protection for memory page after reading, error code: " << GetLastError() << std::endl;
	}

	return status;
}

BOOL WriteMemory(HANDLE hProcess, DWORD64 dwAddress, BYTE* value)
{
	DWORD dwOldProt;
	if (!VirtualProtectEx(hProcess, (void*)dwAddress, sizeof(*value), PAGE_EXECUTE_READWRITE, &dwOldProt))
	{
		std::cout << "Unable to update protection for memory page before writing, error code: " << GetLastError() << std::endl;
		return FALSE;
	}

	BOOL status = TRUE;
	SIZE_T bytesWritten;
	if (!WriteProcessMemory(hProcess, (void*)dwAddress, value, sizeof(*value), &bytesWritten))
	{
		std::cout << "Unable to write to memory page, error code: " << GetLastError() << std::endl;
		status = FALSE;
	}
	//else
	//{
	//	std::cout << "Successfully wrote " << bytesWritten << " bytes" << std::endl;
	//}

	if (!VirtualProtectEx(hProcess, (void*)dwAddress, sizeof(*value), dwOldProt, &dwOldProt))
	{
		std::cout << "Unable to revert protection for memory page after writing, error code: " << GetLastError() << std::endl;
	}

	return status;
}