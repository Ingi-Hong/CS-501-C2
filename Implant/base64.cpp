#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>


//hint: use CRYPT_STRING_BASE64 :-)


std::string  b64Encode(std::vector<BYTE> binaryData){
    // note that this will convert your std::string into a c string. 
    BYTE* rawData = binaryData.data();
    // your code here
    DWORD bytes;
    //bytes will store bytes written
    CryptBinaryToStringA(rawData, binaryData.size(), CRYPT_STRING_BASE64,NULL, &bytes);
    //printf("bytes written \n: %d", bytes);
    char * tempBuff = (char *)malloc(bytes);
    CryptBinaryToStringA(rawData, binaryData.size(), CRYPT_STRING_BASE64, tempBuff, &bytes);
    std::string returnBuff(tempBuff);

    // Hint: you should make two calls to ::CryptBinaryToStringA 
    // One to get the right size for the buffer
    // Then one to copy the data over
    // std::vector<BYTE> is a perfectly fine container for raw binary  data 
    // as it is allocated in contiguous chunks of memory 
    // you can also easily convert it to raw data via returnBuff.data()

    //change me
    free(tempBuff);
    return returnBuff;
}


std::vector<BYTE> b64Decode(std::string strInput){
    // as before you should make two calls to ::CryptStringToBinaryA 
    char * toDecrypt = new char[strInput.length() + 1];
    strcpy(toDecrypt, strInput.c_str());
    
    DWORD bytes;
    BYTE * retBuff;
    CryptStringToBinaryA( toDecrypt , strInput.length() ,CRYPT_STRING_BASE64, NULL,& bytes,NULL,NULL ) ;   
    CryptStringToBinaryA(toDecrypt , strInput.length() ,CRYPT_STRING_BASE64, retBuff ,&bytes,NULL,NULL);
    std::vector<BYTE> ByteVector(retBuff, retBuff + bytes );
    return ByteVector;
}

int main(int argc,  char* argv[]){
    if(argc !=3){
        std::cout << "Incorrect number of arguments" 
        << std::endl 
        << "Usage: ./" << argv[0] 
        << "<encode|decode> data" 
        << std::endl;
        return 0;
    }

    std::string action = std::string(argv[1]);
    std::string dataString = std::string(argv[2]);

   

    if( action == "decode"){
        // in this case, we assume the raw data happens to also be a string
        auto resultVector = b64Decode(dataString);

        // You may assume the resulting bytes are all in the ascii range
        std::string resultStr(resultVector.begin(), resultVector.end());
        // note needs to be none null 
        std::cout << resultStr << std::endl;

    } else if( action == "encode"){
         // note this removes the null terminator 
        std::vector<BYTE> stringData(dataString.begin(), dataString.end());

        std::cout << b64Encode(stringData ) << std::endl;
    } else{
        std::cout << "Wrong action: use either decode of encode" 
        << std::endl;
        
        return 0;
    }
    return 0;
}