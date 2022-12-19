#include "xor.h"

int main(){
    std::string s = "\n'..-b-0.&c";
    std::string result = xor_string(s, 0x42);
    std::cout << result << std::endl;
}