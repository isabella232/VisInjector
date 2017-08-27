#include "Utils.h"
#include <TlHelp32.h>
#include <algorithm>
#include <Wtsapi32.h>
#pragma comment(lib, "Wtsapi32.lib")

DWORD getPIDFromName(WCHAR* name)
{
	WTS_PROCESS_INFO* pWPIs = NULL;
	DWORD dwProcCount = 0;
	if (WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, NULL, 1, &pWPIs, &dwProcCount))
	{
		for (DWORD i = 0; i < dwProcCount; i++)
		{
			if (!wcscmp(pWPIs[i].pProcessName, name))
			{
				DWORD PID = pWPIs[i].ProcessId; //Copy PID to return
				WTSFreeMemory(pWPIs); //Free memory
				return PID; //Return the first matching PID we find
			}
		}
	}

	//Free memory
	if (pWPIs)
		WTSFreeMemory(pWPIs);
	
	return NULL; //No PID found
}

bool createRemoteThreadInject(const char* name, const char* dllPath, DWORD PID)
{
	//PROCESS_ALL_ACCESS not necessary
	HANDLE hHandle = OpenProcess(PROCESS_CREATE_THREAD |
		PROCESS_QUERY_INFORMATION |
		PROCESS_VM_OPERATION |
		PROCESS_VM_WRITE |
		PROCESS_VM_READ,
		FALSE,
		PID);
	if (!hHandle)
		return false;

	HANDLE dllPathAddr = VirtualAllocEx(hHandle,
		0,
		strlen(dllPath) + 1,
		MEM_RESERVE | MEM_COMMIT,
		PAGE_EXECUTE_READWRITE);
	if (!dllPathAddr)
		return false;

	WriteProcessMemory(hHandle,
		dllPathAddr,
		(LPVOID)dllPath,
		strlen(dllPath) + 1,
		NULL);

	LPVOID loadLibAddr = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryA");
	if (!loadLibAddr)
		return false;

	CreateRemoteThread(hHandle, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibAddr, dllPathAddr, 0, NULL);
	CloseHandle(hHandle);
	VirtualFreeEx(hHandle, dllPathAddr, 0, MEM_RELEASE);

	return true; //Injected successfully
}

bool compareProcessList(WTS_PROCESS_INFOW first, WTS_PROCESS_INFOW second)
{
	std::wstring wfirst(first.pProcessName);
	std::wstring wsecond(second.pProcessName);
	std::string sfirst(wfirst.begin(), wfirst.end());
	std::string ssecond(wsecond.begin(), wsecond.end());
	std::transform(sfirst.begin(), sfirst.end(), sfirst.begin(), ::toupper);
	std::transform(ssecond.begin(), ssecond.end(), ssecond.begin(), ::toupper);
	
	return (sfirst < ssecond);
}