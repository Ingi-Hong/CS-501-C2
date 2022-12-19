#include "xor.h"
using namespace std;

int KEY = 0x42;
//XORs a string with a key
std::string xor_string(std::string s, int key = KEY) {
    std::string result = s;
    for (int i = 0; i < s.length(); i++) {
        result[i] = s[i] ^ key;
    }
    return result;
}

//XORs a wstring with a key
std::wstring xor_wstring(wchar_t* s, int key = KEY) {
    std::wstring result = s;
    for (int i = 0; i < wcslen(s); i++) {
        result[i] = s[i] ^ key;
    }
    return result;
}
