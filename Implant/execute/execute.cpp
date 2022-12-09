#include "execute.h"

//needs to read into file -> in order to go line by line
void execute(char* program_and_arg, char* outfile){

    // cmd line from program, args, and outfile (by concatenating a string)
    char* cmd = (char*)malloc (strlen(program_and_arg) + strlen(outfile) + 33);

    // Received help during office hours on this command line
    strcpy(cmd, "C:\\Windows\\System32\\cmd.exe /c ");

    strcat(cmd, program_and_arg);
    strcat(cmd, " >> ");
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

void executecommandfromfile(void){

    HANDLE fileCreation = CreateFileA("ExecuteMe.txt", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_READONLY, NULL);
   
    DWORD dwFileSize = GetFileSize(fileCreation, NULL);

    char *buffer = (char*)malloc(dwFileSize + 1);

    if (buffer == NULL) {
        printf("Memory Allocated failed \n");
        CloseHandle(fileCreation);
        free(buffer);
        return;
    }

    buffer[dwFileSize] = '\0';
    if(!(ReadFile(fileCreation, buffer, dwFileSize, NULL, NULL))){
        printf("File is not being read properly");
    }else{
        //THIS NEEDS TO BE WRITTEN INTO A FILE
       char* resultfile = "result.txt";
       execute(buffer, resultfile);
    }
    
    CloseHandle(fileCreation);
    free(buffer);
    return;
    
}


