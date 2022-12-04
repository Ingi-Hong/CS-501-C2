#include "aes_gcm.h"


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


}


void AESGCM::Decrypt(BYTE* nonce, size_t nonceLen, BYTE* data, size_t dataLen, BYTE* macTag, size_t macTagLen) {
    // BCryptDecrypt - function decrypts a block a data
    ULONG cbOutput;
    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO pPaddingInfo;
    BCRYPT_INIT_AUTH_MODE_INFO(pPaddingInfo);
    pPaddingInfo.pbNonce = (PUCHAR)nonce;
    pPaddingInfo.cbNonce = (ULONG)nonceLen;
    //pPaddingInfo.pbMacContext = (PUCHAR)macTag;
    //pPaddingInfo.cbMacContext = (ULONG)macTagLen;
    //Add iv???
    //Is this using block padding by passing 0 as dwflags https://learn.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptdecrypt. IF so bad
    NTSTATUS Decryption = BCryptDecrypt(hKey, data, dataLen, &pPaddingInfo, NULL, 0, plaintext, ptBufferSize, &cbOutput, 0);

}

void AESGCM::Encrypt(BYTE* nonce, size_t nonceLen, BYTE* data, size_t dataLen) {
    // BCryptEncrypt - function encrypts a block of data
    ULONG cbOutput;

    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO pPaddingInfo;
    pPaddingInfo.pbNonce = (PUCHAR)nonce;
    pPaddingInfo.cbNonce = (ULONG)nonceLen;
//the size of the plaintext specified in the cbInput parameter must be a multiple of the algorithm's block size,, datalen must be multiple of 96 (block size)
//need an iv, 96 random bytes (block size)
    NTSTATUS Encryption = BCryptEncrypt(hKey, data, dataLen, &pPaddingInfo, NULL, 0, ciphertext, ctBufferSize, &cbOutput, 0);

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
