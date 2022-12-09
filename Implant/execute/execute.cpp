#include "execute.h"

void execute(char* program, char* args, char* outfile){

    // cmd line from program, args, and outfile (by concatenating a string)
    char* cmd = (char*)malloc (strlen(program) + strlen(args) + strlen(outfile) + 33);

    // Received help during office hours on this command line
    strcpy(cmd, "C:\\Windows\\System32\\cmd.exe /c ");

    strcat(cmd, program);
    strcat(cmd, " ");
    strcat(cmd, args);
    strcat(cmd, " > ");
    strcat(cmd, outfile);

    // Values needed for CreateProcessA
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    // Dead squirrels
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    // Dead squirrels 
    ZeroMemory(&pi, sizeof(pi));

    // var: to check if the process is created or not.
    BOOL var;

    // https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessa
    var = CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
    if(!var){

        // Error checking for createprocess
        printf( "CreateProcess failed (%d).\n", GetLastError() );
    }else{

        // Waits for the process to finish before ending the program
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    // Cleans up cmd's allocated memory
    free(cmd);

    // Output
    return;
}