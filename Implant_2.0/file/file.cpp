#include "file.h"

// This method was grabbed from Feiyu's Post.cpp (Just added recusrive iterator)
std::string getFileNamesFromPath(std::string path) {

	std::string result;
	for (const auto& entry : std::filesystem::recursive_directory_iterator(path)){
	
		result+=entry.path().string();
		result+="NEXT";
        
    }

	return result;
}

// int main(int argc, char* argv[]){
//      std::string path = "C:/users/mochi/Desktop/test_folder";
//    	std::vector<std::string> storage = getFileNamesFromPath(path);

//      //std::string path = path;
            
//     return 0;
// }