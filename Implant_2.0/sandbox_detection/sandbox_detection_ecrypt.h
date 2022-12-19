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
#include "xor/xor.h"

/* Libraries */
using namespace std;
int KEY = 0x24;

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
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, xor_string("lev`sevax`awgvmtpmkjxw]WPAIxfmkw", KEY).c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        char buffer[256];
        DWORD bufferSize = sizeof(buffer);
        if (RegQueryValueExA(hKey, xor_string("w]WPAIiEJQBEGPQVAV", KEY).c_str(), nullptr, nullptr, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
        {
            std::string manufacturer(buffer);
            if (manufacturer.find(xor_string("riSEVA", KEY).c_str()) != std::string::npos)
            {
                return true;
            }
        }
        if (RegQueryValueExA(hKey, xor_string("w]WPAItVK@QGPjEIA", KEY).c_str(), nullptr, nullptr, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
        {
            
            std::string model(buffer);
            if (model.find(xor_string("rMVPQEHiEGLMJA", KEY).c_str()) != std::string::npos || model == xor_string("rmvpqehfk|", KEY).c_str())
            {
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
    
	return !(exists_test(xor_string("w]WPAIvKKPxw]WPAIx@VMRAVWxLCBW\nW]W", KEY).c_str()) == 0 || exists_test(xor_string("w]WPAIvKKPxw]WPAIx@VMRAVWxRILCBW\nW]W", KEY).c_str())
		|| exists_test(xor_string("w]WPAIvKKPxw]WPAIx@VMRAVWxTVHAPL\nW]W", KEY).c_str()) || exists_test(xor_string("w]WPAIvKKPxw]WPAIx@VMRAVWxTVHBW\nW]W", KEY).c_str()) || exists_test(xor_string("w]WPAIvKKPxw]WPAIx@VMRAVWxTVHIKQWA\nW]W", KEY).c_str()) || exists_test(xor_string("w]WPAIvKKPxw]WPAIx@VMRAVWxTVHRM@AK\nW]W", KEY).c_str()) || exists_test(xor_string("w]WPAIvKKPxw]WPAIx@VMRAVWxRI\\nW]W", KEY).c_str()));
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
