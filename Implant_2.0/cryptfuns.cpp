#include <windows.h>
#include <stdio.h>
#include <map>
#include <iostream>
#include <fstream>
#include <math.h>
#include <filesystem>
#define privkeysz   (ULONG)283
#define pubkeysz    (ULONG)155
/*void encfile(infile,outfile,pubkey){

}

char* decbuffer(PUCHAR encbuffer){
    

}
*/




BCRYPT_KEY_HANDLE makersakeypair(){
    //Returns handle to rsa keypair
    BCRYPT_ALG_HANDLE ahandle;
    NTSTATUS openalgo= BCryptOpenAlgorithmProvider(&ahandle, BCRYPT_RSA_ALGORITHM, NULL, 0);
    printf("OpenAlgo Status: %x \n",openalgo);
    BCRYPT_KEY_HANDLE rsakey = new BCRYPT_KEY_HANDLE;
    NTSTATUS keygen=BCryptGenerateKeyPair(ahandle,&rsakey,1024,0);
    printf("Keygen Status: %x \n",keygen);
    ULONG keysize;
    NTSTATUS finalizekp= BCryptFinalizeKeyPair(rsakey,0);
    printf("FinalizeKeyPair Status: %x \n",finalizekp);
    return rsakey;
}

  

BCRYPT_KEY_HANDLE rsakeyimport(PUCHAR key,ULONG keysize,LPCWSTR keytype){
    BCRYPT_ALG_HANDLE rsahandle;
    BCryptOpenAlgorithmProvider(&rsahandle, BCRYPT_RSA_ALGORITHM, NULL, 0);
    BCRYPT_KEY_HANDLE impkey = new BCRYPT_KEY_HANDLE;  
    NTSTATUS importstatus=BCryptImportKeyPair(rsahandle, NULL, keytype, &impkey, key, keysize, BCRYPT_NO_KEY_VALIDATION);  
    printf("KeyImport Status: %x \n",importstatus);
    return impkey;
}

PUCHAR getrsapubkey(BCRYPT_KEY_HANDLE rsakey){
    ULONG bytescopied;
    ULONG keysize;
    NTSTATUS keyexportsz=BCryptExportKey(rsakey,NULL,BCRYPT_RSAPUBLIC_BLOB,NULL,0,&keysize,0);
    printf("pubKeyExportSz Status: %x \n",keyexportsz);
    PUCHAR keyport=(PUCHAR)malloc(keysize);
    NTSTATUS keyexport=BCryptExportKey(rsakey,NULL,BCRYPT_RSAPUBLIC_BLOB,keyport,keysize,&bytescopied,0);
    printf("pubKeyExport Status: %x \n",keyexport);
    printf("pubKeysize: %lu \n",keysize);
    return keyport;
}

PUCHAR getrsaprivkey(BCRYPT_KEY_HANDLE rsakey){
    ULONG bytescopied;
    ULONG keysize;
    NTSTATUS keyexportsz=BCryptExportKey(rsakey,NULL,BCRYPT_RSAPRIVATE_BLOB,NULL,0,&keysize,0);
    printf("privKeyExportSz Status: %x \n",keyexportsz);
    PUCHAR keyport=(PUCHAR)malloc(keysize);
    NTSTATUS keyexport=BCryptExportKey(rsakey,NULL,BCRYPT_RSAPRIVATE_BLOB,keyport,keysize,&bytescopied,0);
    printf("privKeyExport Status: %x \n",keyexport);
    printf("privKeysize: %lu \n",keysize);
    return keyport;
}


PUCHAR rsaencbuffer(PUCHAR buffer, ULONG buffersz, BCRYPT_KEY_HANDLE key, ULONG encbuffersz){
    //Returns encrypted buffer
    PUCHAR encbuffer = (PUCHAR)HeapAlloc(GetProcessHeap(), 0, encbuffersz);
    if (encbuffer == NULL)
    {
        printf("failed to allocate memory for blindedFEKBuffer\n");
    }
    ULONG bytescopied;
    NTSTATUS keyencryptstatus=BCryptEncrypt(key,buffer,buffersz, NULL, NULL, 0, encbuffer, encbuffersz, &bytescopied,BCRYPT_PAD_PKCS1);
    printf("KeyEncrypt Status: %x \n",keyencryptstatus);
    printf("Bytes Copied: %lu \n",bytescopied);
    return encbuffer;
}

ULONG rsaencbuffersz(PUCHAR buffer, ULONG buffersz, BCRYPT_KEY_HANDLE key){
    //Returns encrypted buffer
    ULONG encbuffersz;
    NTSTATUS keyencryptszstatus = BCryptEncrypt(key,buffer,buffersz,NULL,NULL,0,NULL,0,&encbuffersz,BCRYPT_PAD_PKCS1);
    printf("KeyEncryptSize Status: %x \n size: %u\n",keyencryptszstatus,encbuffersz);
    return encbuffersz;
}


PUCHAR rsadecbuffer(PUCHAR buffer, ULONG buffersz,BCRYPT_KEY_HANDLE key, ULONG decbuffersz){
    //Returns decrypted buffer 
    PUCHAR decbuffer = (PUCHAR)HeapAlloc(GetProcessHeap(), 0, decbuffersz);
    if (decbuffer == NULL)
    {
        printf("failed to allocate memory for blindedFEKBuffer\n");
    }
    ULONG bytescopied;
    NTSTATUS keydecryptstatus=BCryptDecrypt(key,buffer,buffersz, NULL, NULL, 0, decbuffer, decbuffersz, &bytescopied, BCRYPT_PAD_PKCS1);
    
    printf("KeyDecrypt Status: %x \n",keydecryptstatus);
    printf("Bytes Copied: %lu \n",bytescopied);
    return decbuffer;
}

ULONG rsadecbuffersz(PUCHAR buffer, ULONG buffersz,BCRYPT_KEY_HANDLE key){
    //Returns decrypted buffer size
    ULONG decbuffersz;
    NTSTATUS keydecryptszstatus = BCryptDecrypt(key,buffer,buffersz,NULL,NULL,0,NULL,0,&decbuffersz,BCRYPT_PAD_PKCS1);
    printf("KeyDecryptSize Status: %x \n",keydecryptszstatus);
    return decbuffersz;
}

PUCHAR filetobuffer(const char* infile,int size){
    //Returns buffer from filepath
    std::ifstream FileText;
    FileText.open(infile,std::ios::in|std::ios::binary|std::ios::ate);
    //int size=FileText.tellg();
    FileText.seekg(0,std::ios::beg);
    char* oData=0;
    oData = new char[ size +1];
    FileText.read(oData,size);
    oData[size]='\0';
    FileText.close();
    printf("Filesize: %i\n",size);
    return (PUCHAR)oData;   
}
ULONG getfilesz(const char* infile){
    std::uintmax_t size = std::filesystem::file_size(infile);
    return ULONG(size);
}

void buffertofile(const char* outfile,PUCHAR buffer,int size){
    std::ofstream WriteFile(outfile,std::ofstream::binary);
    WriteFile.write(reinterpret_cast<const char*>(buffer),size);
    WriteFile.close();
}




int main(){
    /*
    Pubkey Size: 283
    Privkey Size: 155
    */

   //Creates rsa keypair, writes public and private keys to files.
    BCRYPT_KEY_HANDLE rsakp=makersakeypair();
    PUCHAR privkeyblob=getrsaprivkey(rsakp);
    PUCHAR pubkeyblob=getrsapubkey(rsakp);
    buffertofile("privatekey.txt",privkeyblob,283);
    buffertofile("publickey.txt",pubkeyblob,155);
    printf("%lu \n",getfilesz("privatekey.txt"));
    printf("%lu \n",getfilesz("publickey.txt"));
    
    //Import keys
    /*
    ULONG prblobsz=getfilesz("privatekey.txt");
    PUCHAR privkeyblob=filetobuffer("privatekey.txt",prblobsz);    
    ULONG pbblobsz=getfilesz("publickey.txt");
    PUCHAR pubkeyblob=filetobuffer("publickey.txt",pbblobsz);
    BCRYPT_KEY_HANDLE privkey=rsakeyimport(privkeyblob,privkeysz,BCRYPT_RSAPRIVATE_BLOB);
    BCRYPT_KEY_HANDLE pubkey=rsakeyimport(pubkeyblob,pubkeysz,BCRYPT_RSAPUBLIC_BLOB);
    
    //Import plaintext,Encrypt works a little fuzzy
    ULONG plaintextsz=getfilesz("aes.txt");
    PUCHAR plaintext=filetobuffer("aes.txt",plaintextsz);
    ULONG encbuffersz=rsaencbuffersz(plaintext,plaintextsz,pubkey);
    PUCHAR encbuffer=rsaencbuffer(plaintext,plaintextsz,pubkey,encbuffersz);
    buffertofile("encrypted_aes_key.txt",encbuffer,encbuffersz);
    ULONG decbuffersz=rsadecbuffersz(encbuffer,encbuffersz,privkey);
    PUCHAR decbuffer=rsadecbuffer(encbuffer,encbuffersz,privkey,decbuffersz);
    buffertofile("aes_attempt.txt",decbuffer,decbuffersz);
    printf("%lu",getfilesz("encrypted_aes_key.txt"));
   */
   /* Decrypt from file works perfectly
    ULONG encfilebuffersz=getfilesz("encrypted.txt");
    PUCHAR encfilebuffer=filetobuffer("encrypted.txt",encfilebuffersz);
    ULONG decbuffersz=rsadecbuffersz(encfilebuffer,encfilebuffersz,privkey);
    PUCHAR decbuffer=rsadecbuffer(encfilebuffer,encfilebuffersz,privkey,decbuffersz);
    buffertofile("decrypted.txt",decbuffer,decbuffersz);
    
    */
    
    //buffertofile("encrypted.txt",encbuffer,encbuffersz);
    //buffertofile("privkey.txt",privkey,)
}