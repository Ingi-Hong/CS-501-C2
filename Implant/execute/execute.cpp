#include "execute.h"

//needs to read into file -> in order to go line by line
void execute(char* program_and_arg, char* outfile){


    // cmd line from program, args, and outfile (by concatenating a string)
    char* cmd = (char*)malloc (strlen(program_and_arg) + strlen(outfile) + 33);

    // Received help during office hours on this command line
    strcpy(cmd, "C:\\Windows\\System32\\cmd.exe /c ");
        std::string program_and_arg2;
        program_and_arg2.assign(program_and_arg);
        std::string program;
        std::string task_id;
        std::string delimiter = " ";
       
        size_t pos = 0;
        std::string command;

        pos = program_and_arg2.find(delimiter);
        task_id = program_and_arg2.substr(0, pos);
        program_and_arg2.erase(0, pos + delimiter.length());
        program = program_and_arg2;

    int num = std::stoi(task_id);
    char* spacenum = (char*) malloc(3);
    sprintf(spacenum, "%d ", num);

    char* cmd2 = (char*) malloc (strlen(outfile) + 40);
    sprintf(cmd2,"C:\\Windows\\System32\\cmd.exe /c (echo|set /p=%s) >> %s",spacenum,outfile);
   
    char* program2 = const_cast<char*>(program.c_str());
    strcat(cmd, program2);
    strcat(cmd, " >> ");
    strcat(cmd, outfile);
   
    // Values needed for CreateProcessA
    STARTUPINFOA si2;
    PROCESS_INFORMATION pi2;
    // Dead squirrels
    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);
    // Dead squirrels 
    ZeroMemory(&pi2, sizeof(pi2));

    // var: to check if the process is created or not.
    BOOL var;

    // https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessa
    var = CreateProcessA(NULL, cmd2, NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2);
    if(!var){

        // Error checking for createprocess
        printf( "CreateProcess failed (%d).\n", GetLastError() );
    }else{

        // Waits for the process to finish before ending the program
        WaitForSingleObject(pi2.hProcess, INFINITE);
        CloseHandle(pi2.hProcess);
        CloseHandle(pi2.hThread);
    }
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
        // Dead squirrels
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    // Dead squirrels 
    ZeroMemory(&pi, sizeof(pi));
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
    free(cmd2);

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


