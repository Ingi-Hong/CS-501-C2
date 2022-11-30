#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unordered_map> // want to include standard c++ stl 
#include <iostream>
#include <windows.h>


/* start of implant functions*/

void  FileUpload(std::string args){}

void FileDownload(std::string args){}

void SitAware(std::string args){
}

void Stealer(std::string args){
    printf("DOING THIEVERY");
}

//void *(*start_routine) (void *)
/*Task Dispatch*/
using pfunc = void (*)(std::string);
std::unordered_map<std::string, pfunc > dispatch_table;

void DispatchTableInit(){
    dispatch_table["SitAware"] = Stealer;
    dispatch_table["FileDown"] = FileDownload;
    dispatch_table["FileUp"] = FileUpload;
    dispatch_table["Stealer"] = Stealer;
    }


//these functions are listd above but maybe we should break it up
//tasks are lists of tasks structured [cmd: , args:, taskid: ]
void tasks_dispatch(char ** tasks);
//create results arr and return

void task_do(char *task);
//get cmd, args, id
//choose execute
// read output from file and return res



void tasking();

/* other helper functions necessary*/

void readFile(char * Path, char * buf);


int main(int argc, char* argv[]){

    DispatchTableInit();
    pfunc p = dispatch_table["Stealer"];
    std::string args = "";
    (*p)(args);
    



    return 0;
}