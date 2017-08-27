#include <Windows.h>
#include <string>
#include <Wtsapi32.h>
#pragma comment(lib, "Wtsapi32.lib")
#pragma once

DWORD getPIDFromName(WCHAR* name);
bool createRemoteThreadInject(const char* name, const char* dllPath, DWORD PID);
bool compareProcessList(WTS_PROCESS_INFOW first, WTS_PROCESS_INFOW second);