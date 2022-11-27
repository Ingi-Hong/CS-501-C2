/**post request, try initial communcation with c2, when working->
every 10 secs check for tasks->if tasks exit dispatch->append to dictionary->post to payload**/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SLEEP 10

// Random Implant ID in hex using rand()
// https://stackoverflow.com/questions/33325814/how-to-loop-generate-random-256bit-hex-in-c-or-c
char* random_id(){
    char random_hex[32+1];
    char * random_hexptr = random_hex;

    srand(time(0));
    for(int i = 0; i < 32; i++){
        sprintf(random_hex+i, "%x", rand() % 32);
    }
    
    return random_hexptr;
}







int main(int argc, char* argv[]){

    // char blah[33];
    // strcpy(blah, random_id());
    // printf("%s\n", blah);

   // register_();



    return 0;
}

