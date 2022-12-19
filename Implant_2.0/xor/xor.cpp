#include "xor.h"

//xor a string
std::string xor_string(std::string s, int key) {
    std::string result = s;
    for (int i = 0; i < s.length(); i++) {
        result[i] = s[i] ^ key;
    }
    return result;
}

