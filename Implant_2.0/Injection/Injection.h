#ifndef INJECTION_H
#define INJECTION_H

/* Standard Header Files */
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <TCHAR.H>

bool InjectDll(const wchar_t*, const char*);
void createProcess(std::string);

#endif
