#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>
#include "sqlite3.h"
#include "..\nlohmann\json.hpp"

using json = nlohmann::json;

BYTE * getPassword(BYTE * key, BYTE * encrypted_password, DWORD encryp_len);
std::stringstream get_chrome_pass(BYTE *key, sqlite3* db);
json getAllResults(sqlite3* db);
std::string hexStr(const uint8_t *data, int len);