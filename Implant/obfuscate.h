
#include<windows.h>
#include <stdio.h>



void hide(char inpString[]){
    char xorKey = '9';
    int len = strlen(inpString);
    for (int i = 0; i < len; i++){
        inpString[i] = inpString[i] ^ xorKey;
    }
}


typedef BOOL (WINAPI * _CreateProcessA)(
   LPCSTR                lpApplicationName,
   LPSTR                 lpCommandLine,
     LPSECURITY_ATTRIBUTES lpProcessAttributes,
       LPSECURITY_ATTRIBUTES lpThreadAttributes,
               BOOL                  bInheritHandles,
        DWORD                 dwCreationFlags,
      LPVOID                lpEnvironment,
       LPCSTR                lpCurrentDirectory,
                  LPSTARTUPINFOA        lpStartupInfo,
               LPPROCESS_INFORMATION lpProcessInformation
);

typedef HMODULE(WINAPI * _LoadLibraryA)( //wait im dumb this isnt possible (not for first call)
 LPCSTR lpLibFileName
);