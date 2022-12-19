#include <iostream>
#include <string>

//xor a string
std::string xor_string(std::string& s) {
    std::string result = s;
    for (int i = 0; i < s.length(); i++) {
        result[i] = s[i] ^ 0x42;
    }
    return result;
}

int main(){
    std::string s = "\n'..-b-0.&c";
    std::string result = xor_string(s);
    std::cout << result << std::endl;
}
