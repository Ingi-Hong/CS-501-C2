#ifndef WINDOWSSCHEDULER_H
#define WINDOWSSCHEDULER_H

/* Standard Header Files*/
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <comdef.h>
#include <wincred.h>

// Include the task header file.
#include <taskschd.h>

/* Libraries */
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "credui.lib")

using namespace std;

/* Functions */
bool AddTaskToWindowsSchdualer(wstring path);

/* Define */
#define _WIN32_DCOM

/* References */
//https://learn.microsoft.com/en-us/windows/win32/taskschd/time-trigger-example--c---

#endif