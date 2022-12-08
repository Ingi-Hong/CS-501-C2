#include "nlohmann/json.hpp"
#include "parse.h"

using json = nlohmann::json;

    /*
    parseTasks - Currently takes in no parameters and assumes there is a JSON file to parse called tasking.json
    If there is no JSON file - it can be changed to take an input object of json.

    The json format to follow is in tasking.json at the moment.

    This method will create a txt file called ExecuteMe which is assumed to be used in a separate execution command
    */

// std::vector<std::string> if it needs a return
void parseTasks(void){
    
    // Reads in the JSON using ifstream
    std::ifstream ifs("tasking.json");

    // Parses the input JSON
    json test = json::parse(ifs);

    // https://json.nlohmann.me/api/basic_json/count/#version-history
    // Gets the size of the object
    auto count = test.at("Tasks").size();

    // counter
    auto counting = 0;

    // two separate vectors for commands and args
    std::vector<std::string> commands;
    std::vector<std::string> args;

    // Writes in the JSON using ofstream (Creates a ExecuteMe.txt)
    std::ofstream exefile;
    exefile.open("ExecuteMe.txt");

    // While loop will count through the JSON objects
    while(counting < count){

        // Gets the command and arg lines in the JSON
        // https://linuxhint.com/parse-json-data-cpp/
        // https://stackoverflow.com/questions/38099308/accessing-elements-from-nlohmann-json
        std::string cmdline = test.at("Tasks")[counting].at("command");
        std::string argline = test.at("Tasks")[counting].at("args");

        // Pushes them to the std::vector if necessary
        commands.push_back(cmdline);
        commands.push_back(argline);

        // Outputs commandline and arglines to the file
        std::cout << cmdline << argline << std::endl;
        exefile << cmdline << " " << argline << std::endl;


        counting+=1;
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

