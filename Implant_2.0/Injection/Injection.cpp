#include "Injection.h"

using namespace std;



//https://gist.github.com/gnh1201/1ab1127da8d7cb6b658d9345dfa4432c
//https://github.com/ZeroMemoryEx/Dll-Injector/blob/master/DLL-Injector/Dll-Injector.cpp
//Search for the process ID by name
DWORD GetProcessByName(CONST TCHAR* processName) {
    //Create a snapshot of all processes
    HANDLE processALL = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32W processInfo = { 0 };
    processInfo.dwSize = sizeof(PROCESSENTRY32W);
    do
    {
        //If the process name matches the one we are looking for, return the process ID
        if (_tcscmp(processInfo.szExeFile, processName) == 0)
        {
            CloseHandle(processALL);
            return processInfo.th32ProcessID;
        }
        //If not, continue searching
    } while (Process32Next(processALL, &processInfo));
    CloseHandle(processALL);
    return 0;
}

//Get the module handle of a process
HMODULE GetProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName) {
    MODULEENTRY32 moduleEntry;
    HANDLE handle = NULL;
    //Create a snapshot of all modules in the process
    handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (!handle) {
        CloseHandle(handle);
        return NULL;
    }
    ZeroMemory(&moduleEntry, sizeof(MODULEENTRY32));
    moduleEntry.dwSize = sizeof(MODULEENTRY32);
    //Get the first module
    if (!Module32First(handle, &moduleEntry)) {
        CloseHandle(handle);
        return NULL;
    }
    do {
        //If the module name matches the one we are looking for, return the module handle
        if (_tcscmp(moduleEntry.szModule, moduleName) == 0) {
            CloseHandle(handle);
            return moduleEntry.hModule;
        }
        //If not, continue searching
    } while (Module32Next(handle, &moduleEntry));
    CloseHandle(handle);
    return 0;
}

//Inject a DLL into a process
bool InjectDll(const wchar_t* processName, const char* dllPath) {
    //Get the process ID
    DWORD dword = GetProcessByName(processName);
    if (dword == 0)
    {
        return false;
    }
    //Open the process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dword);
    if (hProcess == NULL)
    {
        return false;
    }
    //Allocate memory for the DLL path
    LPVOID DLLAddress = VirtualAllocEx(hProcess, NULL, strlen(dllPath), MEM_COMMIT, PAGE_READWRITE);
    if (WriteProcessMemory(hProcess, DLLAddress, dllPath, strlen(dllPath), NULL) == 0)
    {
        return false;
    }
    //Get the address of LoadLibraryA
    HMODULE k32 = GetModuleHandle(TEXT("Kernel32.dll"));
    LPVOID loadADD = GetProcAddress(k32, "LoadLibraryA");
    //Create a remote thread to load the DLL
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadADD, DLLAddress, 0, NULL);
    CloseHandle(hThread);
    CloseHandle(hProcess);
    return true;
}

//Unload a DLL from a process
void UnLoadDll(const wchar_t* processName, const wchar_t* dllName) {
    DWORD dword = GetProcessByName(processName);
    if (dword == 0)
    {
        return;
    }
    HMODULE hmodule = GetProcessModuleHandle(dword, dllName);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dword);
    if (hProcess == NULL)
    {
        return;
    }
    HMODULE k32 = GetModuleHandle(TEXT("Kernel32.dll"));
    LPVOID loadADD = GetProcAddress(k32, "FreeLibrary");

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadADD, (LPVOID)hmodule, 0, NULL);
    CloseHandle(hThread);
    CloseHandle(hProcess);
}

//Create a process
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

//Main function for testing
/*
int main() {
    std::wstring s = L"cmd.exe";
    std::string b = "C:\\Users\\53444\\Downloads\\Simple-DLL-Injection-master\\C++\\x64\\Release\\InjectedDLL.dll";
    createProcess("C:\\Windows\\System32\\cmd.exe");
    InjectDll(s.c_str(), b.c_str());
    UnLoadDll(L"cmd.exe", L"InjectedDLL.dll");
}
*/
