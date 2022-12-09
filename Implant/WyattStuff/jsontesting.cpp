#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unordered_map> // want to include standard c++ stl 
#include <iostream>
#include <windows.h>
#include "..\nlohmann\json.hpp"

using json = nlohmann::json;
using string = std::string;

void ArrayTest(){
    json b;
    json c;
    b["key"] = 3;
    b["ooga"] = 10;
    c["hi"] = 9;
    c["fi fi fo fum"] = 123;
    std::vector<json> data; 
    data.push_back(b);
    data.push_back(c);

   json a = json(data);
    std::cout << a.dump();
}
int main(int argc, char* argv[]){
    ArrayTest();
}