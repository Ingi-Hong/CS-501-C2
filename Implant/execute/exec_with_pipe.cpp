#include <windows.h>
#include <stdio.h>
#include <minwinbase.h>
#include "..\nlohmann\json.hpp"

using json = nlohmann::json;
#define BUF_SIZE 4096

std::string exec(char* program, char* args){
    LPSTR cmdBuffer = (char *)malloc(1000 * sizeof(char));
    if (cmdBuffer == NULL){
        printf("malloc failed");
        exit(1);
    }
    int j = sprintf(cmdBuffer, "cmd.exe /C ");
    j += sprintf(cmdBuffer + j, program );
    j += sprintf(cmdBuffer+ j, " ");
    j += sprintf(cmdBuffer + j, args);
    j += sprintf(cmdBuffer + j, "\0");
    
    // Declare handles for StdOut
    HANDLE hStdOutRead, hStdOutWrite;
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    // Prevent dead squirrels 
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
     si.dwFlags |= STARTF_USESTDHANDLES;

    CreatePipe(&hStdOutRead, &hStdOutWrite, &sa,BUF_SIZE);
    si.hStdInput = NULL;
    si.hStdError = hStdOutWrite;
    si.hStdOutput = hStdOutWrite;
   
    int retVal = CreateProcessA(    // No module name (use command line)
        NULL,        // Command line
        cmdBuffer,
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        TRUE,          // Set handle inheritance to TRUE binheritance
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi ) ;          // Pointer to PROCESS_INFORMATION structure
    
    if (retVal == 0){
        printf("Create process failed\n");
        exit(0);
    }
    char * buff = (char *)malloc(BUF_SIZE);
    DWORD bytes;
    LPDWORD lpExitCode;
    DWORD lpTotalBytesAvail;
    int ind = 1;
    while( (WaitForSingleObject(pi.hProcess, 0) == WAIT_TIMEOUT) || (ind ==1) ){ //check to see if child process is alive
        ind = 0;
        //GetExitCodeProcess(pi.hProcess, lpExitCode);
        PeekNamedPipe(hStdOutRead, NULL, 0, NULL, &lpTotalBytesAvail, NULL);
        //printf("avail: %d\n", lpTotalBytesAvail);
        while ((long)lpTotalBytesAvail > 0){
            //while this value is positive we have something to read
            ReadFile(hStdOutRead, buff, BUF_SIZE, &bytes, NULL);
            buff[bytes] = '\0';
            
            printf("%s", buff);
            lpTotalBytesAvail = lpTotalBytesAvail - bytes;
        }
    }
    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
    CloseHandle(hStdOutRead);
    CloseHandle(hStdOutWrite);

    //free(buff);
    free(cmdBuffer);
    return std::string(buff);
}

int main(int argc, char* argv[]){

}