#include <windows.h>
#include <chrono>
#include <random>
#include <filesystem>
#include <fstream>




char* xorcrypt(char *text,int textsize,char *key){
    //xors a character buffer. encrypt and decrypt are the same.
    for(int i=0;i<textsize;i++){
        text[i]=text[i]^key[i%32];
    }
    return text;
}

char* newsymkey(){
    //returns random 32 bit symmetric key (random 32 bit string)
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    int buf[8];
    for(int i=0;i<8;i++){
        buf[i]=rng(); 
        //printf("%x\n",buf[i]);
    }
    char *symkey=new char[32];
    memcpy(symkey,&buf,32);  
    return symkey;
}



int main(){
    return;
}
