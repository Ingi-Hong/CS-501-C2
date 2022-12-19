#ifndef FILE_H
#define FILE_H

/* Standard Header Files */
#include <windows.h>
#include <stdio.h>
#include <ShlObj.h>
#include <Lmcons.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdbool.h>
#include <vector>
#include "../xor/xor.h"

#include "../xor/xor.h"

/* Functions */
std::string getFileNamesFromPath(std::string path);

#endif