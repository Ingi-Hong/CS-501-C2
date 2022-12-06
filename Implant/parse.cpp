#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include <vector>

using json = nlohmann::json;

    
    // This takes in - assuming the json file is all set (tasking.json) can be exchanged for json toParse (the input)
    // and ouputs a vector of strings of commands to be executed
    // This is assuming that execute will be the next call
    //INPUT NEEDS TO BE CHANGED TO json toParse
    //UNLESS IT CAN JUST TAKE IN A JSON FILE!!!
std::vector<std::string> parseTasks(){
    std::ifstream ifs("tasking.json");
    //std::ifstream ifs(toParse);
    json test = json::parse(ifs);
    //https://json.nlohmann.me/api/basic_json/count/#version-history
    auto count = test.count("Tasks");
    auto counting = 0;
    std::vector<std::string> commands;
    std::vector<std::string> args;

    std::ofstream exefile;
    exefile.open("ExecuteMe.txt");;
    while(counting < count){
        std::string cmdline = test.at("Tasks")[counting].at("command");
        std::string argline = test.at("Tasks")[counting].at("args");
        commands.push_back(cmdline);
        commands.push_back(argline);
        std::cout << cmdline << argline << std::endl;
        exefile << cmdline << argline << std::endl;
        counting+=1;
    }

    
    exefile.close();

    return commands;
    

}


//Testing Purposes
// int main(int argc, char* argv[]){

//     parseTasks();
//     return 0;
// }

