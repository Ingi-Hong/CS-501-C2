#include "../nlohmann/json.hpp"
#include "parse.h"

using json = nlohmann::json;

    /*
    parseTasks - Currently takes in no parameters and assumes there is a JSON file to parse called tasking.json
    If there is no JSON file - it can be changed to take an input object of json.

    The json format to follow is in tasking.json at the moment.

    This method will create a txt file called ExecuteMe which is assumed to be used in a separate execution command
    */

// std::vector<std::string> if it needs a return
void parseTasks(std::string jsonfile){
    
    // Reads in the JSON using ifstream
    std::ifstream ifs(jsonfile);

    // Parses the input JSON
    json test = json::parse(ifs);

    // https://json.nlohmann.me/api/basic_json/count/#version-history
    // Gets the size of the object
    int count = (int) test.size();

    // counter
    int counting = 0;

    // two separate vectors for commands and args
    std::vector<std::string> commands;
    std::vector<std::string> args;

    // Writes in the JSON using ofstream (Creates a ExecuteMe.txt)
    std::ofstream exefile;
    exefile.open("ExecuteMe.txt");
    std::vector<int> task_ids;
    // While loop will count through the JSON objects
    while(counting < count){

        if(test[counting][4]=="untouched"){
        std::string cmdline = test[counting][2];
        int task_id = test[counting][0];

        std::string delimiter = " ";
        
        //https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
        size_t pos = 0;
        std::string command;

        pos = cmdline.find(delimiter);
        command = cmdline.substr(0, pos);
            
        cmdline.erase(0, pos + delimiter.length());
    
        std::string argline = cmdline;

        // // Pushes them to the std::vector if necessary
        // commands.push_back(cmdline);
        // commands.push_back(argline);

        // // Outputs commandline and arglines to the file
        // std::cout << cmdline << argline << std::endl;
        exefile << task_id << " " << command << " " << argline << std::endl;


        counting+=1;
        }
    }

    
    // Closes files
    exefile.close();
    ifs.close();

    //return commands;
    return;

}


//Testing Purposes
// int main(int argc, char* argv[]){

//     parseTasks();
//     return 0;
// }

