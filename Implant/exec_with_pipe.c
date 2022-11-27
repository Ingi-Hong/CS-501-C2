#include <windows.h>
#include <stdio.h>
#include <minwinbase.h>

#define BUF_SIZE 4096

int main(int argc, char* argv[]){

    if (argc != 3){
        printf("Usage: %s program.exe \"args and args \"", argv[0]);
        return 0;
    }
    // arse args
    char* program = argv[1];
    char* args = argv[2];

    // create buffer for cmdline argument
    // //your solution here!
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
    
    
    //printf(cmdBuffer);
    
    
    
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
    //SetHandleInformation(hStdOutWrite, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT );
    //i dont need to modify the read end i dont think
    //SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0); //0 = not inheritable?

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
     si.dwFlags |= STARTF_USESTDHANDLES;

    // TODO: ensure that the child processes can inherit our handles!
    // //your solution here!

    // TODO: Create a pipe  object and share the handle with a child processes 
    // //your solution here!
    
    CreatePipe(&hStdOutRead, &hStdOutWrite, &sa,BUF_SIZE);
    //printf("POST PIPE ERA\n");
    // TODO: Set
    // set startupinfo handles
    si.hStdInput = NULL;
    si.hStdError = hStdOutWrite;
    si.hStdOutput = hStdOutWrite;
   

 
    // Create the child Processes and wait for it to terminate!
    // //your solution here!

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
    //WaitForSingleObject( pi.hProcess, INFINITE );
    //`PeekNamedPipe` 
    char * buff = malloc(BUF_SIZE);
    DWORD bytes;
    LPDWORD lpExitCode;
    DWORD lpTotalBytesAvail;
    //GetExitCodeProcess(pi.hProcess, lpExitCode);
    //printf("entering while loop\n");
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
    //what it do baby

    

    // TODO: perform any cleanup necessary! 
    // The parent processes no longer needs a handle to the child processes, the running thread, or the out file!
    // //your solution here!
    // Finally, print the contents from the pipe!
    //

    free(buff);
    free(cmdBuffer);
    return 0;
}