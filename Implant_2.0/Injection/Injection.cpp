#include "Injection.h"

using namespace std;



//https://gist.github.com/gnh1201/1ab1127da8d7cb6b658d9345dfa4432c
//https://github.com/ZeroMemoryEx/Dll-Injector/blob/master/DLL-Injector/Dll-Injector.cpp
DWORD GetProcessByName(CONST TCHAR* processName) {
    HANDLE processALL = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32W processInfo = { 0 };
    processInfo.dwSize = sizeof(PROCESSENTRY32W);
    do
    {
        if (_tcscmp(processInfo.szExeFile, processName) == 0)
        {
            CloseHandle(processALL);
            return processInfo.th32ProcessID;
        }
    } while (Process32Next(processALL, &processInfo));
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
    DWORD dword = GetProcessByName(processName);
    if (dword == 0)
    {
        MessageBoxA(NULL, "Process not found", "Error", 0);
        return;
    }
    HMODULE hmodule = GetProcessModuleHandle(dword, dllName);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dword);
    if (hProcess == NULL)
    {
        MessageBoxA(NULL, "Not able to open process", "Error", 0);
        return;
    }
    HMODULE k32 = GetModuleHandle(TEXT("Kernel32.dll"));
    LPVOID loadADD = GetProcAddress(k32, "FreeLibrary");

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadADD, (LPVOID)hmodule, 0, NULL);
    CloseHandle(hThread);
    CloseHandle(hProcess);
}

void createProcess(std::string path) {
    STARTUPINFOA si = { sizeof(STARTUPINFO) };
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    PROCESS_INFORMATION pi;
    CreateProcessA(path.c_str(), NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    std::wstring s = L"cmd.exe";
    std::string b = "C:\\Users\\53444\\Downloads\\Simple-DLL-Injection-master\\C++\\x64\\Release\\InjectedDLL.dll";
    createProcess("C:\\Windows\\System32\\cmd.exe");
    InjectDll(s.c_str(), b.c_str());
    UnLoadDll(L"cmd.exe", L"InjectedDLL.dll");
}
