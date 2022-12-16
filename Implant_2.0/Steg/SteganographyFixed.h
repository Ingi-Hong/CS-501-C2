#ifndef STEGANOGRAPHYFIXED_H
#define STEGANOGRAPHYFIXED_H

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <iostream>

std::string decodeImage(WCHAR* encryptPath);

void createEncodedImage (WCHAR* plainPath, std::string message, WCHAR* encryptPath);

#endif