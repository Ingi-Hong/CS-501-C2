#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include <vector>

//using namespace nlohmann;
using json = nlohmann::json;
    //TODO
    //char* toParse is unknown right now
    
std::vector<std::string> parseTasks(json toParse){
    std::ifstream ifs("tasking.json");
    //std::ifstream ifs(toParse);
    json test = json::parse(ifs);
    //https://json.nlohmann.me/api/basic_json/count/#version-history
    auto count = test.count("Tasks");
    auto counting = 0;
    std::vector<std::string> commands;
    std::vector<std::string> args;


    while(counting < count){
        std::string cmdline = test.at("Tasks")[counting].at("command");
        std::string argline = test.at("Tasks")[counting].at("args");
        commands.push_back(cmdline);
        commands.push_back(argline);
        counting+=1;
    }
    //going to try to write this to a file, command by command
    
    return commands;
    
    // This exe needs to be tested later. As of right now the code compiles.
    // This takes in - assuming the json file is all set (tasking.json) can be exchanged for json toParse (the input)
    // and ouputs a vector of strings of commands to be executed
    // This is assuming that execute will be the next call
}

