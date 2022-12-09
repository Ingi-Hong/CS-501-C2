#ifndef SANDBOX_DETECTION_H
#define SANDBOX_DETECTION_H
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <algorithm>
#include <string>

bool vmCheck();
bool sandboxTimeCheck();
bool vmDriverCheck();

#endif