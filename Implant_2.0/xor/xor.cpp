#include "xor.h"
using namespace std;

//XORs a string with a key
std::string xor_string(std::string& s, int key) {
    std::string result = s;
    for (int i = 0; i < s.length(); i++) {
        result[i] = s[i] ^ key;
    }
    return result;
}

//XORs a wstring with a key
std::wstring xor_wstring(wchar_t* s, int key) {
    std::wstring result = s;
    for (int i = 0; i < wcslen(s); i++) {
        result[i] = s[i] ^ key;
    }
    return result;
}
