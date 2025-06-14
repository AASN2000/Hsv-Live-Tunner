#include "UserInput.h"
#include <Windows.h>

bool isKeyDown(int vKey) {
    // GetAsyncKeyState returns a short, and the high-order bit is 1 if the key is down.
    return GetAsyncKeyState(vKey) & 0x8000;
}