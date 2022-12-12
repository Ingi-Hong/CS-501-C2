#include "file.h"

// This method was grabbed from Feiyu's Post.cpp (Just added recusrive iterator)
std::vector<std::string> getFileNamesFromPath(std::string path) {
	std::vector<std::string> file_names;
	for (const auto& entry : std::filesystem::recursive_directory_iterator(path)){
		file_names.push_back(entry.path().string());
        std::cout << entry.path().string() << std::endl;
    }
	return file_names;
}

// int main(int argc, char* argv[]){
//      std::string path = "C:/users/mochi/Documents/Projects";
//    // getFileNamesFromPath(path);
//      //std::string path = path;
            
//     return 0;
// }