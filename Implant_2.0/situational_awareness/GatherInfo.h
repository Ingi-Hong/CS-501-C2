#ifndef GATHERINFO_H
#define GATHERINFO_H

/* Standard Header Files */
#include <iostream>
//#include <winsock2.h>
#include <Windows.h>
#include <tchar.h>
#include <winhttp.h>
#include <vector>
#include <string>
#include <utility>
#include <iphlpapi.h>


/* Libraries */
#include "../nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;

/* Functions */
string getComputerName(void);
string getUserName(void);
BOOL CheckWindowsPrivilege(const TCHAR* Privilege);
BOOL IsUserAdmin(void);
std::string checkPrivileges();
string real_ip(void);
string getInterface(void);
json GetAll(void);

/* Test Functions */




#endif