#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <TCHAR.H>

using namespace std;



//https://gist.github.com/gnh1201/1ab1127da8d7cb6b658d9345dfa4432c
//https://github.com/ZeroMemoryEx/Dll-Injector/blob/master/DLL-Injector/Dll-Injector.cpp
DWORD GetProcessByName(CONST TCHAR* processName) {
    HANDLE processALL = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32W processInfo = { 0 };
    processInfo.dwSize = sizeof(PROCESSENTRY32W);
    do
    {
        if (_tcscmp((TCHAR*)processInfo.szExeFile, processName) == 0)
        {
            CloseHandle(processALL);
            return processInfo.th32ProcessID;
        }
    } while (Process32Next(processALL, (LPPROCESSENTRY32)(&processInfo)));
    CloseHandle(processALL);
    return 0;
}


HMODULE GetProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName) {
    MODULEENTRY32 moduleEntry;
    HANDLE handle = NULL;
    handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (!handle) {
        CloseHandle(handle);
        return NULL;
    }
    ZeroMemory(&moduleEntry, sizeof(MODULEENTRY32));
    moduleEntry.dwSize = sizeof(MODULEENTRY32);
    if (!Module32First(handle, &moduleEntry)) {
        CloseHandle(handle);
        return NULL;
    }
    do {
        if (_tcscmp(moduleEntry.szModule, moduleName) == 0) {
            CloseHandle(handle);
            return moduleEntry.hModule;
        }
    } while (Module32Next(handle, &moduleEntry));
    CloseHandle(handle);
    return 0;
}


bool InjectDll(const wchar_t* processName, const char* dllPath) {
    DWORD dword = GetProcessByName(processName);
    if (dword == 0)
    {
        return false;
    }
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dword);
    if (hProcess == NULL)
    {
        return false;
    }
    LPVOID DLLAddress = VirtualAllocEx(hProcess, NULL, strlen(dllPath), MEM_COMMIT, PAGE_READWRITE);
    if (WriteProcessMemory(hProcess, DLLAddress, dllPath, strlen(dllPath), NULL) == 0)
    {
        return false;
    }
    HMODULE k32 = GetModuleHandle(TEXT("Kernel32.dll"));
    LPVOID loadADD = GetProcAddress(k32, "LoadLibraryA");
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadADD, DLLAddress, 0, NULL);
    CloseHandle(hThread);
    CloseHandle(hProcess);
    return true;
}

void UnLoadDll(const wchar_t* processName, const wchar_t* dllName) {
    DWORD dword = GetProcessByName((TCHAR*)processName);
    if (dword == 0)
    {
        MessageBoxA(NULL, "Process not found", "Error", 0);
        return;
    }
    HMODULE hmodule = GetProcessModuleHandle(dword, (TCHAR*)dllName);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dword);
    if (hProcess == NULL)
    {
        MessageBoxA(NULL, "Not able to open process", "Error", 0);
        return;
    }
    HMODULE k32 = GetModuleHandle(TEXT("Kernel32.dll"));
    FARPROC loadADD = GetProcAddress(k32, "FreeLibrary");

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadADD, (LPVOID)hmodule, 0, NULL);
    CloseHandle(hThread);
    CloseHandle(hProcess);
}
/*
int main() {
    InjectDll(L"cmd.exe", "C:\\Users\\53444\\Downloads\\Simple-DLL-Injection-master\\C++\\x64\\Release\\testlib.dll");
    UnLoadDll(L"cmd.exe", L"testlib.dll");
}
*/

