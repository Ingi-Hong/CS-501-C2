#include "file.h"

/* Gets all the files in a directory path
This method was grabbed from Feiyu's Post.cpp (Just added recusrive iterator)
https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator
*/ 
std::string getFileNamesFromPath(std::string path) {

	std::string result;
	for (const auto& entry : std::filesystem::recursive_directory_iterator(path)){
	
		result+=entry.path().string();
		result+=xor_string("[PMA",0x15);
        
    }

	return result;
}