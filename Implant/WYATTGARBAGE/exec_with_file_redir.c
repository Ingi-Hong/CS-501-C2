#include <stdbool.h>
#include <windows.h>
#include <stdio.h>
#include "printfile.h"


void createProc(char * program, char *args, char* outfile, int argc){
    LPSTR cmdBuffer = (char *)malloc(1000 * sizeof(char));
    if (cmdBuffer == NULL){
        printf("malloc failed");
        exit(1);
    }
    // printf("pre cmd buffer \n");

    int j = sprintf(cmdBuffer, "cmd.exe /C ");
     j += sprintf(cmdBuffer + j, program );
     j += sprintf(cmdBuffer+ j, " ");
    j += sprintf(cmdBuffer + j, args );
    j += sprintf(cmdBuffer+ j, " ");
    j += sprintf(cmdBuffer + j, " > ");
    j += sprintf(cmdBuffer + j, outfile);
    j += sprintf(cmdBuffer + j, "\0");


    // Values needed for CreateProcessA
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    // Dead squirrels
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    // Dead squirrels 
    ZeroMemory(&pi, sizeof(pi));
    int retVal;

    // //your solution here!

    retVal = CreateProcessA(    // No module name (use command line)
        NULL,        // Command line
        cmdBuffer,
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to TRUE binheritance
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi ) ;          // Pointer to PROCESS_INFORMATION structure
    
    if (retVal == 0){
        printf("Create process failed\n");
        exit(0);
    }
    WaitForSingleObject( pi.hProcess, INFINITE );
    // printf("process completed \n");

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
    free(cmdBuffer);
}

int main(int argc, char* argv[]){
    

    if (argc != 4){
        printf("Usage: %s program.exe \"args and args \" outfile.txt\n", argv[0]);
        return 0;
    }
    char* program = argv[1];
    char* args = argv[2];
    char* outfile = argv[3];
    
    createProc(program, args, outfile, argc);
    PrintFileContents(outfile);
    return 0;
}