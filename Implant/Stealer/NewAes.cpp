// TestAES_GCM_256_C.cpp : Defines the entry point for the console application.
//


#pragma once

#pragma warning(disable : 4996)

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#ifndef CRYPTOPP_DEFAULT_NO_DLL
#define CRYPTOPP_DEFAULT_NO_DLL 1
#endif

#ifndef CRYPTOPP_ENABLE_NAMESPACE_WEAK
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#endif


#ifdef _DEBUG

#ifndef x64
#pragma comment(lib, "./Crypto5.6.1/Win32/Output/Debug/cryptlib.lib")
#else
#pragma comment(lib, "./Crypto5.6.1/x64/Output/Debug/cryptlib.lib")
#endif
#else
//#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#ifndef x64
#pragma comment(lib, "./Crypto5.6.1/Win32/Output/Release/cryptlib.lib")
#else
#pragma comment(lib, "./Crypto5.6.1/x64/Output/Release/cryptlib.lib")
#endif
#endif


// Crypto++ Include

#include "cryptopp560/pch.h"
#include "cryptopp560/files.h"
#include "cryptopp560/default.h"
//#include "cryptopp560/rsa.h"
#include "cryptopp560/base64.h"
#include "cryptopp560/osrng.h"


//AES
#include "cryptopp560/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "cryptopp560/cryptlib.h"
using CryptoPP::BufferedTransformation;
using CryptoPP::AuthenticatedSymmetricCipher;

#include "cryptopp560/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::AuthenticatedEncryptionFilter;
using CryptoPP::AuthenticatedDecryptionFilter;


#include "cryptopp560/aes.h"
using CryptoPP::AES;

#include "cryptopp560/gcm.h"
using CryptoPP::GCM;
using CryptoPP::GCM_TablesOption;

#include <iostream>
#include <string>
#include <bitset>
#include <time.h>
#include <windows.h> 

USING_NAMESPACE(CryptoPP)
USING_NAMESPACE(std)



#define CRYPTOGRAPHIC_KEY_LENGTH			1024
#define SALT_BYTE_LENGTH					41
#define MAC_SIZE							16

static inline RandomNumberGenerator& PSRNG(void);
static inline RandomNumberGenerator& PSRNG(void)
{
	static AutoSeededRandomPool rng;
	rng.Reseed();
	return rng;
}

bool encrypt_aes256_gcm(const char *aesKey, const char *aesIV, const char *inPlainText, char **outEncryptedBase64, int &dataLength);
bool decrypt_aes256_gcm(const char *aesKey, const char *aesIV, const char *inBase64Text, char **outDecrypted, int &dataLength);
void Base64Decode(const std::string &inString, std::string &outString);
void HexDecode(const std::string &inString, std::string &outString);

static std::string m_ErrorMessage;


int main(int arg, char** argv)
{

	//using above code these key and iv was generated
	std::string hexKey = "2192B39425BBD08B6E8E61C5D1F1BC9F428FC569FBC6F78C0BC48FCCDB0F42AE";
	std::string hexDecodedKey;
	HexDecode(hexKey, hexDecodedKey);

	std::string hexIV = "E1E592E87225847C11D948684F3B070D";
	std::string hexDecodedIv;
	HexDecode(hexIV, hexDecodedIv);


	std::string plainText = "Test encryption and decryption";
	std::string encryptedWithJava = "A/boAixWJKflKviHp2cfDl6l/xn1qw2MsHcKFkrOfm2XOVmawIFct4fS1w7wKw==";

	printf("%s%s\n", "Plain Text: " , plainText.c_str());

	//encrypt - result base64 encoded string
	char *outEncryptedText=NULL;
	int outDataLength=0;
	bool bR = encrypt_aes256_gcm(hexDecodedKey.c_str(), hexDecodedIv.c_str(), plainText.c_str(), &outEncryptedText, outDataLength);
	printf("%s%s\n", "Encrypted base64 encoded: " , outEncryptedText);

	//decrypt - result plain string
	char* outDecryptedText=NULL;
	int outDecryptedDataLength=0;
	bR = decrypt_aes256_gcm(hexDecodedKey.c_str(), hexDecodedIv.c_str(), encryptedWithJava.c_str(), &outDecryptedText, outDecryptedDataLength);
	printf("%s%s\n", "Decrypted Text Encrypted by Java: " , outDecryptedText);

	if (plainText == outDecryptedText)
	{
		printf("%s\n", "Test Passed");
	}
	else
	{
		printf("%s\n", "Test Failed");
	}
	

	return 0;
}

void printBinary(int num)
{
	std::vector<int> arr;
	int n = num;
	int bit;
	while (n > 0)
	{
		bit = n % 2;
		arr.push_back(bit);
		n /= 2;
	}

	for (std::vector<int>::reverse_iterator it = arr.rbegin(); it!=arr.rend(); ++it) 
	{
		printf("%i", *it);
	}
	printf("\n");
}

bool encrypt_aes256_gcm(const char *aesKey, const char *aesIV, const char *inPlainText, char **outEncryptedBase64, int &dataLength)
{
	bool bR = false;
	//const int TAG_SIZE = 12;
	std::string outText;
	std::string outBase64;

	if (strlen(aesKey)>31 && strlen(aesIV)>15)
	{
		try
		{
			GCM< AES >::Encryption aesEncryption;
			aesEncryption.SetKeyWithIV(reinterpret_cast<const BYTE*>(aesKey), AES::MAX_KEYLENGTH, reinterpret_cast<const BYTE*>(aesIV), AES::BLOCKSIZE);
			StringSource(inPlainText, true, new AuthenticatedEncryptionFilter(aesEncryption, new StringSink(outText)
			) // AuthenticatedEncryptionFilter
			); // StringSource

			CryptoPP::Base64Encoder *base64Encoder = new CryptoPP::Base64Encoder(new StringSink(outBase64), false);
			base64Encoder->PutMessageEnd(reinterpret_cast<const unsigned char *> (outText.data()), outText.length());
			delete base64Encoder;

			dataLength = outBase64.length();
			if (outBase64.length()>0)
			{
				if (*outEncryptedBase64) free(*outEncryptedBase64);
				*outEncryptedBase64 = (char*)malloc(dataLength + 1);
				memset(*outEncryptedBase64, '\0', dataLength + 1);
				memcpy(*outEncryptedBase64, outBase64.c_str(), dataLength);

				bR = true;
			}
			else
			{
				m_ErrorMessage.append("_encryptAES256 -> Encryption Failed");
			}

		}
		catch (CryptoPP::InvalidArgument& e)
		{
			m_ErrorMessage.append(e.what());
		}
		catch (CryptoPP::Exception& e)
		{
			m_ErrorMessage.append(e.what());
		}
	}
	else
	{
		m_ErrorMessage.append("_encryptAES256 -> AES Key or IV cannot be empty");
	}

	outText.clear();
	outBase64.clear();

	return bR;
}


bool decrypt_aes256_gcm(const char *aesKey, const char *aesIV, const char *inBase64Text, char **outDecrypted, int &dataLength)
{
	bool bR = false;
	std::string outText;
	std::string pszDecodedText;
	Base64Decode(inBase64Text, pszDecodedText);

	if (strlen(aesKey)>31 && strlen(aesIV)>15)
	{
		try
		{
			GCM< AES >::Decryption aesDecryption;
			aesDecryption.SetKeyWithIV(reinterpret_cast<const BYTE*>(aesKey), AES::MAX_KEYLENGTH, reinterpret_cast<const BYTE*>(aesIV), AES::BLOCKSIZE);
			AuthenticatedDecryptionFilter df(aesDecryption, new StringSink(outText));

			StringSource(pszDecodedText, true,
				new Redirector(df /*, PASS_EVERYTHING */)
			); // StringSource

			bR = df.GetLastResult();


			dataLength = outText.length();
			if (outText.length()>0)
			{
				if (*outDecrypted) free(*outDecrypted);
				*outDecrypted = (char*)malloc(dataLength + 1);
				memset(*outDecrypted, '\0', dataLength + 1);
				memcpy(*outDecrypted, outText.c_str(), dataLength);

				bR = true;
			}
			else
			{
				m_ErrorMessage.append("_decryptAES256 -> Decryption Failed");
			}
		}
		catch (CryptoPP::HashVerificationFilter::HashVerificationFailed& e)
		{
			m_ErrorMessage.append(e.what());
		}
		catch (CryptoPP::InvalidArgument& e)
		{
			m_ErrorMessage.append(e.what());
		}
		catch (CryptoPP::Exception& e)
		{
			m_ErrorMessage.append(e.what());
		}
	}
	else
	{
		m_ErrorMessage.append("_encryptAES256 -> AES Key or IV cannot be empty");
	}

	return bR;
}

void Base64Decode(const std::string &inString, std::string &outString)
{
	StringSource(inString, true, new Base64Decoder(new StringSink(outString)));
}

void HexDecode(const std::string &inString, std::string &outString)
{
	StringSource(inString, true, new HexDecoder(new StringSink(outString)));
}