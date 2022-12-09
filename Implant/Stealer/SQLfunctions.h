#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>
#include "sqlite3.h"

BYTE * getPassword(BYTE * key, BYTE * encrypted_password, DWORD encryp_len);
std::stringstream get_chrome_pass(BYTE *key, sqlite3* db);