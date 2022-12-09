#include <iostream>
#include <comdef.h>
#include <comutil.h>
#include <Wbemidl.h>
#include <algorithm>
#include <string>
#include <fstream>


using namespace std;

#pragma comment(lib, "wbemuuid.lib")

bool vmCheck() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        char buffer[256];
        DWORD bufferSize = sizeof(buffer);
        if (RegQueryValueExA(hKey, "SystemManufacturer", nullptr, nullptr, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
        {
            std::string manufacturer(buffer);
            if (manufacturer.find("VMware") != std::string::npos)
            {
                std::cout << "manufacturer: " << manufacturer << std::endl;
                return true;
            }
        }
        if (RegQueryValueExA(hKey, "SystemProductName", nullptr, nullptr, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
        {
            
            std::string model(buffer);
            if (model.find("Virtual Machine") != std::string::npos || model == "VIRTUALBOX")
            {
                std::cout << "model: " << model << std::endl;
                return true;
            }
        }
        RegCloseKey(hKey);
    }
    return FALSE;
}

bool exists_test(const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
};

bool vmDriverCheck() {
	struct stat buffer;
    //https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
    
	return !(exists_test("%SystemRoot%\\System32\\drivers\\hgfs.sys") == 0 || exists_test("%SystemRoot%\\System32\\drivers\\vmhgfs.sys")
		|| exists_test("%SystemRoot%\\System32\\drivers\\prleth.sys") || exists_test("%SystemRoot%\\System32\\drivers\\prlfs.sys") || exists_test("%SystemRoot%\\System32\\drivers\\prlmouse.sys") || exists_test("%SystemRoot%\\System32\\drivers\\prlvideo.sys") || exists_test("%SystemRoot%\\System32\\drivers\\vmx86.sys"));
}

bool sandboxTimeCheck() {
	int currentTime = GetTickCount64();
	Sleep(6000);
	int sleepTimer = GetTickCount64() - 6000 - currentTime;
	if (sleepTimer >= 100) {
		return true;
	}
	return false;
}

int main(int argc, char** argv)
{
	struct stat buffer;
	if (vmCheck() || vmDriverCheck() || sandboxTimeCheck()) {
		MessageBoxW(NULL, L"Running in vm", L"Not good", MB_OK | MB_ICONERROR);
		exit(0);
	}
}
