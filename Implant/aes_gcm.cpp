#include "aes_gcm.h"
#include <vector>
#include <iostream>


AESGCM:: ~AESGCM() {
    Cleanup();
}

// Freebie: initialize AES class
AESGCM::AESGCM(BYTE key[AES_256_KEY_SIZE]) {
    hAlg = 0;
    hKey = NULL;

    // create a handle to an AES-GCM provider
    nStatus = ::BCryptOpenAlgorithmProvider(
        &hAlg,
        BCRYPT_AES_ALGORITHM,
        NULL,
        0);
    if (!NT_SUCCESS(nStatus))
    {
        printf("**** Error 0x%x returned by BCryptOpenAlgorithmProvider\n", nStatus);
        Cleanup();
        return;
    }
    if (!hAlg) {
        printf("Invalid handle!\n");
    }
    nStatus = ::BCryptSetProperty(
        hAlg,
        BCRYPT_CHAINING_MODE,
        (BYTE*)BCRYPT_CHAIN_MODE_GCM,
        sizeof(BCRYPT_CHAIN_MODE_GCM),
        0);
    if (!NT_SUCCESS(nStatus)) {
        printf("**** Error 0x%x returned by BCryptGetProperty ><\n", nStatus);
        Cleanup();
        return;
    }


    nStatus = ::BCryptGenerateSymmetricKey(
        hAlg,
        &hKey,
        NULL,
        0,
        key,
        AES_256_KEY_SIZE,
        0);
    if (!NT_SUCCESS(nStatus)) {
        printf("**** Error 0x%x returned by BCryptGenerateSymmetricKey\n", nStatus);
        Cleanup();
        return;
    }
    DWORD cbResult = 0;
    nStatus = ::BCryptGetProperty(
        hAlg,
        BCRYPT_AUTH_TAG_LENGTH,
        (BYTE*)&authTagLengths,
        sizeof(authTagLengths),
        &cbResult,
        0);
    if (!NT_SUCCESS(nStatus)) {
        printf("**** Error 0x%x returned by BCryptGetProperty when calculating auth tag len\n", nStatus);
    }

    //std::vector<BYTE> authTag(authTagLengths.dwMinLength);


}


void AESGCM::Decrypt(BYTE* nonce, size_t nonceLen, BYTE* data, size_t dataLen, BYTE* macTag, size_t macTagLen) {
    // BCryptDecrypt - function decrypts a block a data
    ULONG cbOutput;
    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO pPaddingInfo;
    BCRYPT_INIT_AUTH_MODE_INFO(pPaddingInfo);
    std::vector<BYTE> authTag(authTagLengths.dwMinLength); //should this be the same???

    pPaddingInfo.pbNonce = (PUCHAR)nonce;
    pPaddingInfo.cbNonce = (ULONG)nonceLen;
    //pPaddingInfo.pbMacContext = (PUCHAR)macTag;
    //pPaddingInfo.cbMacContext = (ULONG)macTagLen;
    pPaddingInfo.pbTag = (PUCHAR)macTag;
    pPaddingInfo.cbTag   = (ULONG)macTagLen;
    //pPaddingInfo.cbAAD = 0;
    //pPaddingInfo.cbData = 0;
    //pPaddingInfo.dwFlags = BCRYPT_AUTH_MODE_CHAIN_CALLS_FLAG;
    //pPaddingInfo.pbAuthData = NULL;
    //pPaddingInfo.cbAuthData = 0;
    //pPaddingInfo.dwFlags  &= ~BCRYPT_AUTH_MODE_CHAIN_CALLS_FLAG;
    //Add iv???
    //std::vector<BYTE> contextIV(AES_BLOCK_SIZE);

    //Is this using block padding by passing 0 as dwflags https://learn.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptdecrypt. IF so bad
    NTSTATUS GetSize = BCryptDecrypt(hKey, data, dataLen, &pPaddingInfo, NULL, 0, NULL, 0, &cbOutput, 0);
    if (!NT_SUCCESS(GetSize)){
        printf("**** Error 0x%x returned by BCryptDecrypt when calculating auth tag len\n", nStatus);
    }
   //cbOutput = dataLen;
    plaintext = (BYTE *)malloc(cbOutput*sizeof(BYTE) + 1) ;
    ptBufferSize = cbOutput;
    NTSTATUS Decryption = BCryptDecrypt(hKey, data, dataLen, &pPaddingInfo, NULL,0, plaintext, ptBufferSize, &cbOutput, 0);
    if (!NT_SUCCESS(Decryption)){
         printf("**** Error 0x%x returned by BCryptDecrypt when calculating auth tag len\n", nStatus);
    }
    printf("WE DECRYPTED????");
}

void AESGCM::Encrypt(BYTE* nonce, size_t nonceLen, BYTE* data, size_t dataLen) {
    // BCryptEncrypt - function encrypts a block of data
    //https://stackoverflow.com/questions/30720414/how-to-chain-bcryptencrypt-and-bcryptdecrypt-calls-using-aes-in-gcm-mode
    ULONG cbOutput;

    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO pPaddingInfo;
    BCRYPT_INIT_AUTH_MODE_INFO(pPaddingInfo);
    std::vector<BYTE> authTag(authTagLengths.dwMinLength);
    tag=(BYTE*)malloc(authTagLengths.dwMaxLength);
    //authTag = authTag(authTagLengths.dwMinLength);
    pPaddingInfo.pbNonce = (PUCHAR)nonce;
    pPaddingInfo.cbNonce = (ULONG)nonceLen;
    pPaddingInfo.pbTag   = (PUCHAR)tag;
    pPaddingInfo.cbTag   =(ULONG) authTagLengths.dwMinLength;
    pPaddingInfo.dwFlags=0;
    pPaddingInfo.cbMacContext=0;
    pPaddingInfo.pbMacContext=NULL;
    //pPaddingInfo.cbAAD = 0;
    //pPaddingInfo.cbData = 0;
    //pPaddingInfo.dwFlags = BCRYPT_AUTH_MODE_CHAIN_CALLS_FLAG;
    pPaddingInfo.pbAuthData = NULL;
    pPaddingInfo.cbAuthData = 0;


//the size of the plaintext specified in the cbInput parameter must be a multiple of the algorithm's block size,, datalen must be multiple of 96 (block size)
//need an iv, 96 random bytes (block size)
    NTSTATUS EncryptionLenStatus = BCryptEncrypt(hKey, data, dataLen, &pPaddingInfo, NULL, 0, NULL, 0, &ctBufferSize, 0);
    if (!NT_SUCCESS(EncryptionLenStatus)){
         printf("**** Error 0x%x returned by BCryptEncrypt when calculating auth tag len\n", nStatus);
    }
    std::cout<<ctBufferSize;
    
    ciphertext=(BYTE*)malloc(ctBufferSize+1);
    DWORD* byteswritten=(DWORD*)malloc(sizeof(DWORD));
    NTSTATUS Encryption= BCryptEncrypt(hKey,data,dataLen,&pPaddingInfo,NULL,0,ciphertext,ctBufferSize+1,byteswritten,0);
    if (!NT_SUCCESS(EncryptionLenStatus)){
         printf("**** Error 0x%x returned by BCryptEncrypt when calculating auth tag len\n", nStatus);
    }
    std::cout<<*byteswritten;










}

void AESGCM::Cleanup() {
    if (hAlg) {
        ::BCryptCloseAlgorithmProvider(hAlg, 0);
        hAlg = NULL;
    }
    if (hKey) {
        ::BCryptDestroyKey(hKey);
        hKey = NULL;
    }
    if (tag) {
        ::HeapFree(GetProcessHeap(), 0, tag);
        tag = NULL;
    }
    if (ciphertext) {
        ::HeapFree(GetProcessHeap(), 0, ciphertext);
        ciphertext = NULL;
    }
    if (plaintext) {
        ::HeapFree(GetProcessHeap(), 0, plaintext);
        plaintext = NULL;
    }
}
