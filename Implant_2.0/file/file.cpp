#include "file.h"

std::string readingfiles(void){
    std::string results;
    // bool diditwork = std::filesystem::create_directories("/Users/mochi/Desktop/test_folder");

    
    // std::cout << true << std::endl;
    // std::cout << diditwork << std::endl;
    // std::ofstream("/Users/mochi/Desktop/test_folder/file1.txt");
    // std::ofstream("/Users/mochi/Desktop/test_folder/file2.txt");
    TCHAR myPath[ 4096 ];
    SHGetFolderPath(NULL, 
                             CSIDL_COMMON_DOCUMENTS, 
                             NULL, 
                             0, 
                             myPath);
    
    std::cout << myPath << std::endl;
    for(auto& p: std::filesystem::directory_iterator(myPath)){
        std::cout << p << '\n';
        //results = p;
    }
        
    // std::filesystem::remove_all("sandbox");
    //  std::filesystem::remove_all("sandbox2");

    return results;
}

int main(){
    readingfiles();
    return 0;
}