#include "aes_gcm.h"
#include <vector>


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
    pPaddingInfo.pbNonce = (PUCHAR)nonce;
    pPaddingInfo.cbNonce = (ULONG)nonceLen;
    pPaddingInfo.pbMacContext = (PUCHAR)macTag;
    pPaddingInfo.cbMacContext = (ULONG)macTagLen;

    NTSTATUS Decryption = BCryptDecrypt(hKey, data, dataLen, &pPaddingInfo, NULL, 0, plaintext, ptBufferSize, &cbOutput, 0);
    if (!NT_SUCCESS(Decryption)){
        printf("Decryption ERROR: %d\n", GetLastError());
    }
}

void AESGCM::Encrypt(BYTE* nonce, size_t nonceLen, BYTE* data, size_t dataLen) {
    // BCryptEncrypt - function encrypts a block of data
    ULONG *cbOutput = (ULONG *) malloc(sizeof(ULONG));

    PBCRYPT_AUTHENTICATED_CIPHER_MODE_INFO pPaddingInfo = (PBCRYPT_AUTHENTICATED_CIPHER_MODE_INFO) malloc(sizeof(BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO));
    std::vector<BYTE> authTag(authTagLengths.dwMinLength);
    pPaddingInfo->pbNonce = (PUCHAR)nonce;
    pPaddingInfo->cbNonce = (ULONG)nonceLen;
    pPaddingInfo->pbTag   = &authTag[0];
    pPaddingInfo->cbTag   = authTag.size();

    NTSTATUS Encryption = BCryptEncrypt(hKey, data, dataLen, pPaddingInfo, (PUCHAR)nonce, (ULONG)nonceLen, ciphertext, ctBufferSize, cbOutput, 0);
    
    if (!NT_SUCCESS(Encryption)){
        printf("ENCRYPTION ERROR: %d\n", GetLastError());
    }
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
