#ifndef GATHERINFO_H
#define GATHERINFO_H

/* Standard Header Files */
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <utility>

/* Libraries */
#include "../nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;

/* Functions */
string getComputerName(void);
string getUserName(void);
BOOL CheckWindowsPrivilege(const TCHAR* Privilege);
BOOL IsUserAdmin(void);
vector<pair<string, bool>> checkPrivileges();
json GetAll(void);

/* Test Functions */




#endif