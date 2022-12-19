#ifndef SANDBOX_DETECTION_H
#define SANDBOX_DETECTION_H

/* Standard Header Files*/
#include <iostream>
#include <comdef.h>
#include <comutil.h>
#include <Wbemidl.h>
#include <algorithm>
#include <string>
#include <fstream>

/* Libraries */
using namespace std;

#pragma comment(lib, "wbemuuid.lib")
//This function is used to check if the system is running in a virtual machine
//This function will check the bios manufacturer and the bios product name
//If the bios manufacturer or the bios product name contains the word "VMware" or "Virtual Machine" or "VIRTUALBOX" then the system is running in a virtual machine
/* Functions */
bool vmCheck() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    HKEY hKey;
    //Read the bios manufacturer and the bios product name from the registry
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        char buffer[256];
        DWORD bufferSize = sizeof(buffer);
        //Check if the bios manufacturer contains the word "VMware"
        if (RegQueryValueExA(hKey, "SystemManufacturer", nullptr, nullptr, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
        {
            std::string manufacturer(buffer);
            //If the bios manufacturer contains the word "VMware" then the system is running in a virtual machine
            if (manufacturer.find("VMware") != std::string::npos)
            {
                std::cout << "manufacturer: " << manufacturer << std::endl;
                return true;
            }
        }
        //Check if the bios product name contains the word "Virtual Machine"
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

//This method is used to check if a file exists
bool exists_test(const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}

//This method will check if the vmware tools/driver files are installed in the system
bool vmDriverCheck() {
	struct stat buffer;
    //https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
    
	return !(exists_test("%SystemRoot%\\System32\\drivers\\hgfs.sys") == 0 || exists_test("%SystemRoot%\\System32\\drivers\\vmhgfs.sys")
		|| exists_test("%SystemRoot%\\System32\\drivers\\prleth.sys") || exists_test("%SystemRoot%\\System32\\drivers\\prlfs.sys") || exists_test("%SystemRoot%\\System32\\drivers\\prlmouse.sys") || exists_test("%SystemRoot%\\System32\\drivers\\prlvideo.sys") || exists_test("%SystemRoot%\\System32\\drivers\\vmx86.sys"));
}

//This method will check if the sleep function is being sandboxed
bool sandboxTimeCheck() {
	int currentTime = GetTickCount64();
	Sleep(6000);
	int sleepTimer = GetTickCount64() - 6000 - currentTime;
	if (sleepTimer >= 100) {
		return true;
	}
	return false;
}


#endif
