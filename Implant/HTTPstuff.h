#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <windows.h>
#include <winhttp.h>
#include <future>
#include <cmath>

std::string makeHttpRequest(std::string fqdn, int port, std::string uri, int implant_id);

//LPSTR makePostRequest(LPCWSTR servername, LPCWSTR subdirectory, const char *postdata);

//LPSTR makePostRequest(LPCWSTR servername, LPCWSTR subdirectory, const char *postdata);