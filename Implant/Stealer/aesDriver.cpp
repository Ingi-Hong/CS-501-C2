#include "aes_gcm.h"
#include <vector>


BYTE * getPassword(BYTE * key, BYTE * encrypted_password){
    std::vector<BYTE>VEC_IV(encrypted_password + 3, encrypted_password + 15 );
    std::vector<BYTE>cipher(encrypted_password + 15, encrypted_password + 15 + 23);
    std::vector<BYTE> VEC_tag(encrypted_password + 15 + 23, encrypted_password); //check all indices
    BYTE * textIV = VEC_IV.data() ;
    BYTE * ciphertext = cipher.data();
    BYTE * tag = VEC_tag.data();
    auto box = new AESGCM(key);

    //ctBufferSize = (DWORD)cipher.size();
    box->Decrypt(textIV, sizeof(textIV), box->ciphertext, (DWORD)cipher.size(), box->tag, box->authTagLengths.dwMinLength );
    for(size_t i=0; i< box->ptBufferSize; i++){
        printf("%c", (char) box->plaintext[i]);
    }
    BYTE * ret = (BYTE *)malloc(cipher.size() + 1);
    for(size_t i=0; i< box->ptBufferSize; i++){
        ret[i] = box->plaintext[i];
        printf("%c", (char) box->plaintext[i]);
    }
    printf("\n");
    delete box;
    printf("Goodbye!\n");
    return ret;
}
